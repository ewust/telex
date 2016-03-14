/* Copyright 2012, 2013, 2014  University of Michigan

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <sys/types.h>
#include <sys/time.h>
#include <sys/queue.h>
#include <stdlib.h>
#include <stdio.h>

#include <err.h>
#include <event.h>
#include <evhttp.h>
#include <assert.h>

#include <glib.h>
#include <time.h>
#include <syslog.h>

#include "httpd.h"
#include "bronnect.h"

// Adapted from http://tjake.posterous.com/libevent-webserver-in-40-lines-of-c


#define HTTPD_LOG_INTERVAL  5
#define HTTPD_LOG_FILE        "/var/log/telex/bronnect.stats"
FILE *httpd_logfile;
struct bronnect_stats stats;
int port_list[MAX_PORTS];
int port_list_len;

extern GHashTable *connections;


// Verbose
void httpd_handler(struct evhttp_request *req, void *arg)
{
    struct evbuffer *buf;
    GHashTableIter iter;
    GHashTable *port_users;
    gpointer key, value;

    buf = evbuffer_new();
    if (buf == NULL) {
        err(1, "failed to create response buffer");
    }

    evbuffer_add_printf(buf, "<table>\n");
    evbuffer_add_printf(buf, "  <thead>\n");
    evbuffer_add_printf(buf, "    <tr>\n");
    evbuffer_add_printf(buf, "      <td>#</td> <td>telex conn</td> <td>service conn</td>\n");
    evbuffer_add_printf(buf, "      <td>client conn</td> <td>client</td>\n");
    evbuffer_add_printf(buf, "      <td>notblocked</td> <td>s[r w]</td> <td>c[r w]</td>\n");
    evbuffer_add_printf(buf, "      <td>cleanup</td> <td>state</td> <td>key_port</td>\n");
    evbuffer_add_printf(buf, "      <td>created</td> <td>last data</td> <td>cleanup</td> <td>watchdog</td>\n");
    evbuffer_add_printf(buf, "    </tr>\n");
    evbuffer_add_printf(buf, "  </thead><tbody>\n");

    port_users = g_hash_table_new(NULL, NULL);

    g_hash_table_iter_init (&iter, connections);
    while (g_hash_table_iter_next (&iter, &key, &value)) {

        struct ConnectionState *st = key; 
        struct in_addr orig_h = {st->flow_state.orig_h};
        struct in_addr resp_h = {st->flow_state.resp_h};

        evbuffer_add_printf(buf, "    <tr>\n");
        evbuffer_add_printf(buf, "<td>%d</td> <td>%d</td> <td>%d</td>\n", 
                            st->cnum, st->telex_conn, st->service_conn);
        evbuffer_add_printf(buf, "<td>%d</td> <td>%s:%d</td>\n", 
                            st->client_conn,
                            inet_ntoa(orig_h), ntohs(st->flow_state.orig_p));
        evbuffer_add_printf(buf, "<td>%s:%d</td> <td>%d %d</td> <td>%d %d</td>\n",
                            inet_ntoa(resp_h), ntohs(st->flow_state.resp_p),
                            st->service_ready[0], st->service_ready[1],
                            st->client_ready[0], st->client_ready[1]);
        evbuffer_add_printf(buf, "<td>%d</td> <td>%d</td> <td>%d</td>\n",
                            st->cleanup_waiting, st->state, st->key_port);

        char time_buf[100];
#define EV_ADD_TIME(t) \
        if (t) {    \
            strftime(time_buf, sizeof(time_buf), "%F %H:%M:%S", localtime(&t)); \
            evbuffer_add_printf(buf, "<td>%s</td> ", time_buf); \
        } else {    \
            evbuffer_add_printf(buf, "<td> </td> "); \
        } 

        EV_ADD_TIME(st->t_created)
        EV_ADD_TIME(st->t_last_data)
        EV_ADD_TIME(st->t_cleanup)
        EV_ADD_TIME(st->t_watchdog)

        evbuffer_add_printf(buf, "</tr>\n");

        update_port_usage(port_users, st->key_port, st->flow_state.orig_h);
    }

    evbuffer_add_printf(buf, "  </tbody></table>\n");

    /* User/connections per port */
    evbuffer_add_printf(buf, "<a name=\"ports\"></a>\n");
    evbuffer_add_printf(buf, "<table><thead><tr><td>Port</td><td>Users</td><td>Connections</td></tr></thead><tbody>\n");
    g_hash_table_iter_init(&iter, port_users);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        int port = GPOINTER_TO_INT(key);
        struct port_stats *pstats = value;
        evbuffer_add_printf(buf, "<tr><td>%d</td> <td>%d</td> <td>%ld</td></tr>\n",
                port, g_hash_table_size(pstats->users), pstats->connections);
        g_free(pstats);
    }
    evbuffer_add_printf(buf, "</tbody></table>\n");

    evbuffer_add_printf(buf, "<a href=\"/chart\">chart</a><br/>\n"
                "<a href=\"/bw\">bandwidth</a><br/>\n"
                "<a href=\"/total\">total</a><br/>\n");
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
}

#define HTTPD_LOG_FMT_ARGS  struct evbuffer *buf, uint32_t utime, char *utime_str, \
                            uint32_t tot_conns, uint32_t unique_clients, uint32_t up_bytes, \
                            uint32_t down_bytes, uint32_t active_conns, uint64_t tot_up, \
                            uint64_t tot_down, uint32_t total_conns, struct port_stats *pstats, \
                            int num_ports



// Convert the log file (time num_connections unique_clients) to
// a javascript 2d array for google charts
void httpd_log_to_js(int time_diff, int every, struct evbuffer *buf, 
        void (*fmt_func)(HTTPD_LOG_FMT_ARGS)) {

    uint32_t utime, now = (uint32_t)time(NULL);
    FILE *fp = fopen(HTTPD_LOG_FILE, "r");
    uint32_t tot_conns, unique_clients;
    uint32_t up_bytes, down_bytes, active_conns;
    uint64_t tot_up, tot_down;
    uint32_t total_connections;
    uint32_t num_ports;
    int count = 0;
 
    while (fscanf(fp, "%d %d %d %d %d %d %ld %ld %d %d", &utime, &tot_conns, &unique_clients,
                  &up_bytes, &down_bytes, &active_conns, &tot_up, &tot_down, &total_connections, 
                  &num_ports) != EOF) {
        int i;
        struct port_stats *pstats = malloc(num_ports * sizeof(struct port_stats));
        for (i=0; i<num_ports; i++) {
            if (fscanf(fp, "%d %ld %ld", &pstats[i].port, &pstats[i].users_size, &pstats[i].connections) == EOF) {
                return;
            }
        }
        if (utime < (now - time_diff)) {
            continue;
        }
        if (++count < every) {
            continue;
        } 
        char outstr[200] = "err";
        time_t utime_t = (time_t)utime;
        struct tm *tmp = localtime(&utime_t);
        if (tmp != NULL) {
            strftime(outstr, sizeof(outstr), "%T", tmp);
        }
        
        fmt_func(buf, utime, outstr, tot_conns, unique_clients, 
                up_bytes, down_bytes, active_conns, tot_up, tot_down, total_connections, pstats, num_ports);
        count = 0;
    }
}

// create the chart header, with columns defined by type type_list and titles as subsequent args
//  eg: httpd_add_google_chart_header(buf, "snn", "Time", "Active", "Unique");
// results in:
//      data.addColumn('string', 'Time');
//      data.addColumn('number', 'Active');
//      data.addColumn('number', 'Unique');
void httpd_add_google_chart_header(struct evbuffer *buf, char *type_list, ...)
{
    evbuffer_add_printf(buf, 
        "<html>\n"
        "<head>\n"
        "<style type=\"text/css\" src=\"https://telex.cc/stats.css\"></style>\n"
        "<script type=\"text/javascript\" src=\"https://www.google.com/jsapi\"></script>\n"
        "<script type=\"text/javascript\">\n"
        "  google.load(\"visualization\", \"1\", {packages:[\"corechart\"]});\n"
        "  google.setOnLoadCallback(drawChart)\n"
        "  function drawChart() {\n"
        "    var data = new google.visualization.DataTable();\n"); 
    va_list vl;
    int i;
    va_start(vl, type_list);

    for (i=0; i<strlen(type_list); i++) {
        char *type = "unknown";
        char *title = va_arg(vl, char*);
        if (type_list[i]=='s') type = "string";
        if (type_list[i]=='n') type = "number";
        evbuffer_add_printf(buf, "    data.addColumn('%s', '%s');\n", type, title);
    }
    va_end(vl);
    
}

void httpd_add_google_chart_footer(struct evbuffer *buf, int width, int height, char *title)
{    
    evbuffer_add_printf(buf,
        "    var chart = new google.visualization.LineChart(document.getElementById('chart_div'));\n"
        "    chart.draw(data, {width: %d, height: %d, title: '%s'});\n"
        "  }\n"
        "</script>\n</head>\n"
        "<body><div id=\"chart_div\"></div></body>\n"
        "</html>\n", width, height, title);
}


int httpd_parse_int(struct evhttp_request *req, char *key, int ret)
{
   struct evkeyvalq headers;
    evhttp_parse_query(evhttp_request_uri(req), &headers);
    const char *ret_s = evhttp_find_header(&headers, key);
    if (ret_s) {
        ret = atoi(ret_s);
    }
    return ret;
}

int httpd_parse_time_ago(struct evhttp_request *req)
{
    return httpd_parse_int(req, "t", 60*30);
}

int httpd_parse_every(struct evhttp_request *req)
{
    int ret = httpd_parse_int(req, "e", 1);
    if (ret < 1) ret = 1;
    return ret;
}

int httpd_parse_width(struct evhttp_request *req)
{
    int ret = httpd_parse_int(req, "w", 1200);
    if (ret < 0 || ret > 10000) ret = 1200;
    return ret;
}

int httpd_parse_height(struct evhttp_request *req)
{
    int ret = httpd_parse_int(req, "h", 800);
    if (ret < 0 || ret > 10000) ret = 800;
    return ret;
}

int port_stat_cmp(const void *a, const void *b)
{
    const struct port_stats *p1 = a;
    const struct port_stats *p2 = b;

    if (p1->port < p2->port) {
        return -1;
    }
    return p1->port > p2->port;
}

/* Reads the last line of the file and takes the ports listed there */
void set_port_list(struct evbuffer *buf)
{
    FILE *fp;
    fp = fopen(HTTPD_LOG_FILE, "rb");
    fseek(fp, -MAX_HTTPD_LINE_SIZE, SEEK_END);

    char line[MAX_HTTPD_LINE_SIZE+1];
    while (fgets(line, MAX_HTTPD_LINE_SIZE, fp)); // Gets last line

    // Parse last line just to get the number of ports
    int num_ports;
    int offset;
    sscanf(line, "%*d %*d %*d %*d %*d %*d %*ld %*ld %*d %d %n", &num_ports, &offset); // gcc warning is ok

    // Read through each port and make it a column
    // and add it to the port_list
    assert(num_ports < MAX_PORTS);
    struct port_stats *pstats = malloc(sizeof(struct port_stats) * num_ports);
    int i;
    for (i=0; i<num_ports; i++) {
        int new_offset;
        int port;
        sscanf(&line[offset], "%d %*ld %*ld %n", &port, &new_offset); // gcc warning is ok
        evbuffer_add_printf(buf, "    data.addColumn('number', '%d'); // %d\n", port, new_offset);
        port_list[i] = port;
       
        offset += new_offset;
    }
    free(pstats);
    port_list_len = num_ports;
}

#define HTTPD_CHART_HANDLER_PORT(h_name, title, fmt_fn) \
    void h_name(struct evhttp_request *r, void *a)\
    {\
        struct evbuffer *buf = evbuffer_new();\
        if (buf == NULL) {\
            err(1, "failed to create response buffer");\
        }\
        /* Just set the first column, set_port_list will set the other columns */ \
        httpd_add_google_chart_header(buf, "s", "Time");\
        set_port_list(buf);\
        httpd_log_to_js(httpd_parse_time_ago(r), httpd_parse_every(r), buf, fmt_fn);\
        httpd_add_google_chart_footer(buf, httpd_parse_width(r), httpd_parse_height(r), title);\
        evhttp_send_reply(r, HTTP_OK, "OK", buf);\
    }

#define HTTPD_CHART_HANDLER(h_name, title, fmt_fn, col_fmt, ...) \
    void h_name(struct evhttp_request *r, void *a)\
    {\
        struct evbuffer *buf = evbuffer_new();\
        if (buf == NULL) {\
            err(1, "failed to create response buffer");\
        }\
        httpd_add_google_chart_header(buf, col_fmt, __VA_ARGS__);\
        httpd_log_to_js(httpd_parse_time_ago(r), httpd_parse_every(r), buf, fmt_fn);\
        httpd_add_google_chart_footer(buf, httpd_parse_width(r), httpd_parse_height(r), title);\
        evhttp_send_reply(r, HTTP_OK, "OK", buf);\
    }

// Called by port_users and port_conns, since they do the same thing
// just on different parts of pstats
void httpd_log_fmt_port(struct evbuffer *buf, char *utime_str, struct port_stats *pstats, int num_ports, 
                        uint64_t (*fn)(struct port_stats *pstats))
{
    int i;
    int added = 0;
    evbuffer_add_printf(buf, "     data.addRow(['%s'", utime_str);
    for (i=0; i<num_ports && (i+added)<port_list_len; i++) {
        while (pstats[i].port > port_list[i+added]) {
            evbuffer_add_printf(buf, ",0");
            added++;
            if (i+added > port_list_len) {
                evbuffer_add_printf(buf, "]);\n");
                return;
            }
        }
        if (pstats[i].port != port_list[i+added]) {
            continue;
        }        

        evbuffer_add_printf(buf, ",%ld", fn(&pstats[i]));
    }
    evbuffer_add_printf(buf, "]);\n");
}


// Yo dawg, I heard you like callbacks...so we put a callback in your callback
uint64_t httpd_log_fmt_port_users_cb(struct port_stats *pstats) {
    return pstats->users_size; 
}
void httpd_log_fmt_port_users(HTTPD_LOG_FMT_ARGS)
{
    httpd_log_fmt_port(buf, utime_str, pstats, num_ports, httpd_log_fmt_port_users_cb);
}
HTTPD_CHART_HANDLER_PORT(httpd_port_users_handler, "Unique Telex users",
                    httpd_log_fmt_port_users)


uint64_t httpd_log_fmt_port_conns_cb(struct port_stats *pstats)
{
    return pstats->connections;
}
void httpd_log_fmt_port_conns(HTTPD_LOG_FMT_ARGS)
{
    httpd_log_fmt_port(buf, utime_str, pstats, num_ports, httpd_log_fmt_port_conns_cb);
}
HTTPD_CHART_HANDLER_PORT(httpd_port_conns_handler, "Telex connections",
                        httpd_log_fmt_port_conns)

void httpd_log_fmt_bw_down_tot(HTTPD_LOG_FMT_ARGS)
{
    evbuffer_add_printf(buf, "    data.addRow(['%s',%ld]);\n",
                        utime_str, tot_down/(1024));
}
HTTPD_CHART_HANDLER(httpd_bw_total_down_handler, "Telex Bandwidth (aggregate)",
                    httpd_log_fmt_bw_down_tot, "sn", "Time", "Bandwidth Down (KB)")        


void httpd_log_fmt_bw_up_tot(HTTPD_LOG_FMT_ARGS)
{
    evbuffer_add_printf(buf, "    data.addRow(['%s',%ld]);\n",
                        utime_str, tot_up/(1024));
}
HTTPD_CHART_HANDLER(httpd_bw_total_up_handler, "Telex Bandwidth (aggregate)",
                    httpd_log_fmt_bw_up_tot, "sn", "Time", "Bandwidth Up (KB)")



void httpd_log_fmt_tot_conns(HTTPD_LOG_FMT_ARGS)
{
    evbuffer_add_printf(buf, "    data.addRow(['%s',%d]);\n",
                        utime_str, total_conns);
}
HTTPD_CHART_HANDLER(httpd_total_connections_handler, "Telex connections",
                    httpd_log_fmt_tot_conns, "sn", "Time", "Total connections")



void httpd_log_fmt_bw(HTTPD_LOG_FMT_ARGS)
{
    evbuffer_add_printf(buf, "    data.addRow(['%s',%d,%d]);\n",
                        utime_str, up_bytes/(1024*HTTPD_LOG_INTERVAL),
                        down_bytes/(1024*HTTPD_LOG_INTERVAL));
}
HTTPD_CHART_HANDLER(httpd_bandwidth_handler, "Telex Bandwidth",
                    httpd_log_fmt_bw, "snn", "Time", "Bandwidth Up (KBps)", "Bandwidth Down (KBps)")



void httpd_log_fmt_chart(HTTPD_LOG_FMT_ARGS)
{
    evbuffer_add_printf(buf, "    data.addRow(['%s',%d,%d]);\n", 
                        utime_str, active_conns, unique_clients);
}
HTTPD_CHART_HANDLER(httpd_chart_handler, "Telex users",
                    httpd_log_fmt_chart, "snn", "Time", "Active connections", "Unique clients")

#if 0
void httpd_log_fmt_ajax(HTTPD_LOG_FMT_ARGS)
{
    evbuffer_add_printf(buf, "%s,%d,%d\n", utime_str, active_conns, unique_clients);
}

void httpd_log_ajax_handler(struct evhttp_request *r, void *a)
{
    struct evbuffer *buf = evbuffer_new();
    if (buf == NULL) {
        err(1, "failed to create response buffer");
    }
    //httpd_add_google_chart_header(buf, col_fmt, __VA_ARGS__);
    httpd_log_to_js(httpd_parse_time_ago(r), buf, fmt_fn);
    httpd_add_google_chart_footer(buf, httpd_parse_width(r), httpd_parse_height(r), title);
    evhttp_send_reply(r, HTTP_OK, "OK", buf);
}
#endif

void httpd_showall_handler(struct evhttp_request *req, void *arg)
{ 
    struct evbuffer *buf = evbuffer_new();
    if (buf == NULL) {
        err(1, "failed to create response buffer");
    }

    int ago = httpd_parse_time_ago(req);

    evbuffer_add_printf(buf, "<html><head>"
                "<style type=\"text/css\">\n"
                "   iframe { width: 610px; height: 420px; }\n"
                "</style></head>\n"
                "<body>\n"
                "<iframe src=\"/chart?t=%d&w=600&h=400\"></iframe>\n"
                "<iframe src=\"/total_down?t=%d&w=600&h=400\"></iframe>\n"
                "<iframe src=\"/total_up?t=%d&w=600&h=400\"></iframe>\n"
                "<iframe src=\"/total_conn?t=%d&w=600&h=400\"></iframe>\n"
                "</body></html>", ago, ago, ago, ago);

    evhttp_send_reply(req, HTTP_OK, "OK", buf);
}

void update_port_usage(GHashTable *port_users, int key_port, unsigned int client_ip)
{ 
    // Unique keyport stats
    struct port_stats *pstats = g_hash_table_lookup(port_users, GINT_TO_POINTER(key_port));
    if (pstats == NULL) {
        pstats = g_malloc(sizeof(struct port_stats));
        pstats->users = g_hash_table_new(NULL, NULL);
        pstats->connections = 0;
        g_hash_table_insert(port_users, GINT_TO_POINTER(key_port), pstats);
    }
    g_hash_table_insert(pstats->users, GUINT_TO_POINTER(client_ip), GINT_TO_POINTER(1));
    pstats->connections++; 
}

// Every HTTPD_LOG_INTERVAL seconds, we log to HTTPD_LOG_FILE (httpd_logfile)
// the number of connections open in various states
void httpd_log(int conn, short type, struct event *ev_log_timer) {
    // HTTPD_LOG_FILE:
    // time()    num_connections    unique_clients(by ip)  since_last_up_bytes
    // since_last_down_bytes active_connections tot_up tot_down tot_cons
    // N port_0 users_0 connections_0 ... port_{N-1} users_{N-1} connections_{N-1}
    
    char syslog_msg[MAX_HTTPD_LINE_SIZE + 2];
    int offset = 0;
  
    GHashTable *unique_clients = g_hash_table_new(NULL, NULL);
    GHashTable *port_users = g_hash_table_new(NULL, NULL); //g_int_hash, g_int_equal);

    // get up/down bytes for this interval
    static uint64_t last_up_bytes=0, last_down_bytes=0;
    uint64_t since_last_up = stats.up_bytes - last_up_bytes;    
    uint64_t since_last_down = stats.down_bytes - last_down_bytes;
    last_up_bytes = stats.up_bytes;
    last_down_bytes = stats.down_bytes;
    
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init (&iter, connections);
    while (g_hash_table_iter_next (&iter, &key, &value)) {
        struct ConnectionState *st = key;
        g_hash_table_insert(unique_clients, GUINT_TO_POINTER(st->flow_state.orig_h), GINT_TO_POINTER(1));
        
        update_port_usage(port_users, st->key_port, st->flow_state.orig_h);
   } 
 
    offset += snprintf(syslog_msg, MAX_HTTPD_LINE_SIZE, "%d %d %d %ld %ld %d %ld %ld %d ", 
            (uint32_t)time(NULL), g_hash_table_size(connections), 
            g_hash_table_size(unique_clients), since_last_up, since_last_down,
            stats.active_connections, stats.up_bytes, stats.down_bytes, stats.total_connections);
    g_hash_table_destroy(unique_clients);


    // Unique keyport stats
    int num_ports = g_hash_table_size(port_users);
    if (offset <= MAX_HTTPD_LINE_SIZE) {
        offset += snprintf(&syslog_msg[offset], (MAX_HTTPD_LINE_SIZE - offset),
                       "%d ", num_ports);
    }

    struct port_stats *pstats = malloc(sizeof(struct port_stats) * num_ports);

    int i = 0;
    g_hash_table_iter_init(&iter, port_users);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        struct port_stats *cur_pstats = value;
        pstats[i].port = GPOINTER_TO_INT(key);
        pstats[i].users_size = g_hash_table_size(cur_pstats->users);
        pstats[i].connections = cur_pstats->connections;
        i++;
        
        g_free(cur_pstats);
    }
    // Sort the list of ports
    qsort(pstats, num_ports, sizeof(struct port_stats), port_stat_cmp);

    // Output
    for (i=0; i<num_ports; i++) {
        if (offset <= MAX_HTTPD_LINE_SIZE) {
            offset += snprintf(&syslog_msg[offset], (MAX_HTTPD_LINE_SIZE - offset), "%d %ld %ld ",
                pstats[i].port, pstats[i].users_size, pstats[i].connections);
        }
    }
    free(pstats);
    g_hash_table_destroy(port_users);
   
    syslog(LOG_INFO, "%s", syslog_msg); 
    
    /* Reset the callback event */
    struct timeval timeout = {HTTPD_LOG_INTERVAL, 0};
    timeout_add(ev_log_timer, &timeout);
}

// Will initiliaze the stats httpd for bronnect, using libevent,
// assuming event_init() has already been called.
void httpd_init(int port)
{
    struct evhttp *httpd;

    if (port < 0 || port > 65535) {
        fprintf(stderr, "httpd_init: invalid port %d\n", port);
        return;
    }

    httpd = evhttp_start("127.0.0.1", port);

    /* Set a callback for requests to "/specific". */
    /* evhttp_set_cb(httpd, "/specific", another_handler, NULL); */
    
    evhttp_set_cb(httpd, "/users", httpd_port_users_handler, NULL);
    evhttp_set_cb(httpd, "/conns", httpd_port_conns_handler, NULL);
    evhttp_set_cb(httpd, "/chart", httpd_chart_handler, NULL);
    evhttp_set_cb(httpd, "/bw", httpd_bandwidth_handler, NULL);
    evhttp_set_cb(httpd, "/total_down", httpd_bw_total_down_handler, NULL);
    evhttp_set_cb(httpd, "/total_up", httpd_bw_total_up_handler, NULL);
    evhttp_set_cb(httpd, "/total_conn", httpd_total_connections_handler, NULL);
    evhttp_set_cb(httpd, "/all", httpd_showall_handler, NULL);

    /* Set a callback for all other requests. */
    evhttp_set_gencb(httpd, httpd_handler, NULL);

    /* Setup syslog */
    openlog("Telex", LOG_NDELAY | LOG_CONS, LOG_USER);

    struct timeval timeout = {HTTPD_LOG_INTERVAL, 0};
    struct event *ev_log_timer = g_slice_new(struct event);
    timeout_set(ev_log_timer, (void(*)(int, short, void*))httpd_log,
                ev_log_timer);
    timeout_add(ev_log_timer, &timeout);
}


