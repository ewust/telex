#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/if_ether.h>
#include <linux/if.h>
#include <netpacket/packet.h>
#include <pcap.h> 
#include <unistd.h>
#include <getopt.h>

struct sll {
	uint16_t	pkt_type;
	uint16_t	dev_type;
	uint16_t	halen;
	char		addr[8];
	uint16_t	protocol;
};


//#define DEST_MAC "\x08\x00\x27\x28\xe6\xf0"
#define DEST_MAC "\x08\x00\x27\x28\xe6\xf0"
int out_sock;
char in_mac[7];


int iface_get_id(int sock, const char *dev)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(struct ifreq));
	strncpy(ifr.ifr_name, dev, sizeof(ifr.ifr_name));
	
	if (ioctl(sock, SIOCGIFINDEX, &ifr)==-1)
		return -1;
	
	return ifr.ifr_ifindex;
}

int iface_get_mac(int sock, const char *dev, char *mac)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(struct ifreq));
	strncpy(ifr.ifr_name, dev, sizeof(ifr.ifr_name));

	if (ioctl(sock, SIOCGIFHWADDR, &ifr) == -1)
		return -1;
	
	memcpy(mac, ifr.ifr_addr.sa_data, 6);
	return 0;
}

#if defined(USE_PCAP)
void pcap_callback(u_char *user, const struct pcap_pkthdr *pkt_hdr, const u_char *pkt_data)
{

	if (strncmp(buff, in_mac, 6)==0)
		sendto(out_sock, buff+14, size, 0, (struct sockaddr*)&s, sizeof(s));

}

/* Create and Initialize pcap */
pcap_t * init_pcap(char *dev)
{
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t *pc;

        pc = pcap_create(dev, errbuf);
        if (!pc) {
                fprintf(stderr, "Error: %s\n", errbuf);
                exit(1);
        }

        //set to listen for all packets
        //if (pcap_set_promisc(pc, 1)) {
        //        fprintf(stderr, "Error: could not set promiscuous mode\n");
        //        exit(1);
        //}

        //set to capture full packets
        if (pcap_set_snaplen(pc, 65535)) {
                fprintf(stderr, "Error: could not set snaplen\n");
                exit(1);
        }

        return pc;
}

/* Activate and set callback pcap */
int start_pcap(pcap_t *pc)
{
	//start capturing
	if (pcap_activate(pc)) {
		pcap_perror(pc, "Error: ");
		exit(1);
	}

	//capture capture capture ra ra ra
	return pcap_loop(pc, -1, pcap_callback, NULL);
}
#endif

int init_sock(char *dev, int in_dev)
{
	int ifindex;
	struct sockaddr_ll sll;
	int sock;

	sock = socket(AF_PACKET, in_dev ? SOCK_RAW : SOCK_DGRAM, htons(ETH_P_ALL));
		
	if (sock==-1) {
		perror("Socket fail\n");
		exit(1);
	}
	
	ifindex = iface_get_id(sock, dev);
	if (ifindex==-1) {
		fprintf(stderr, "can't find %s\n", dev);
		exit(1);
	}

	memset(&sll, 0, sizeof(sll));
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifindex;
	sll.sll_protocol = htons(ETH_P_ALL);

	if (bind(sock, (struct sockaddr*)&sll, sizeof(sll))==-1) {
		perror("bind\n");
		exit(1);
	}
	
	return sock;
	
}

void print_usage(char *prog) {
	
	printf("Usage: %s [-d mac] input_dev output_dev\n", prog);
	printf("Interface copy - Copies packets that are sent to input_dev and\n");
	printf("sends out on output_dev\n");
	printf("Optional destination mac address in hex form (01:02:03:04:05:06)\n");
}

//converts a string like 01:de:ad:be:ef:10 to "\x01\xde\xad\xbe\xef\x10"
void parse_mac(char *mac, char *str) 
{
	int a,b,c,d,e,f;
	int n;
	
	n = sscanf(str, "%x:%x:%x:%x:%x:%x", &a, &b, &c, &d, &e, &f);
	if (n != 6) {
		printf("invalid mac address\n");
		exit(-1);
	}
	
	mac[0] = a & 0xff;
	mac[1] = b & 0xff;
	mac[2] = c & 0xff;
	mac[3] = d & 0xff;
	mac[4] = e & 0xff;
	mac[5] = f & 0xff;
}

void parse_args(int argc, char *argv[], char **in_dev, char **out_dev, char *mac)
{
	int c, lidx;
	struct option lopts[] = {{.name="dest-mac", .has_arg=required_argument, .flag=NULL, .val='d'},
				 {0,0,0,0}};

	while ((c = getopt_long_only(argc, argv, "d:", lopts, &lidx)) != -1) {
		switch (c) {
		case 'd':
			parse_mac(mac, optarg);
			break;
		case '?':
			fprintf(stderr, "Unrecognized flag '%c'\n", optopt);
			print_usage(argv[0]);
			exit(-1);
			break;
		default:
			fprintf(stderr, "Unexpected options error\n");
			print_usage(argv[0]);
			exit(-1);
			break;
		}
	}

	if (optind+1 >= argc) {
		print_usage(argv[0]);
		exit(-1);
	}
	
	*in_dev = argv[optind++];
	*out_dev = argv[optind++];
	
}

int main(int argc, char *argv[])
{
	char buff[1501];
	char *in_dev, *out_dev;
	struct sockaddr_ll s;	
	int i;
	int in_sock;
	char mac[6];

	memcpy(mac, DEST_MAC, 6);
	//parse inputs
	parse_args(argc, argv, &in_dev, &out_dev, mac);
#if 0
	if (argc!=3) {
		print_usage(argv[0]);
		exit(1);
	}
	in_dev = argv[1];
	out_dev = argv[2];
#endif 
	//setup sockets
	in_sock = init_sock(in_dev, 1);
	out_sock = init_sock(out_dev, 0);

	//set up destination sockaddr
	memset(&s, 0, sizeof(s));
	s.sll_family = AF_PACKET;
	s.sll_ifindex = iface_get_id(out_sock, out_dev);
	s.sll_protocol = htons(0x0800);
	s.sll_halen = 6;
	memcpy(s.sll_addr, mac, 6);

	//we get the input interface's mac, so we only get
	//packets sent TO this interface (not from)
	//probably a better way, but meh.	
	if (iface_get_mac(in_sock, in_dev, in_mac)) {
		fprintf(stderr, "could not get mac for %s\n", in_dev);
	}
	
	//tell user what we intend to do
	printf("mirroring packets from %s (%d) to %s (%d) (destined to ", 
		in_dev, iface_get_id(in_sock, in_dev),
		out_dev, iface_get_id(out_sock, out_dev));
	for (i=0; i<6; i++) printf("%02x:", (unsigned char)in_mac[i]);
	printf(")\n");	

	//do it
	while (1) {
		size_t size = recv(in_sock, buff, 1500, 0);
		if (memcmp(buff, in_mac, 6)==0)	//packet to our in_sock's MAC addr
			sendto(out_sock, buff+14, size-14, 0, (struct sockaddr*)&s, sizeof(s));
	}
	
	return 0;
}

