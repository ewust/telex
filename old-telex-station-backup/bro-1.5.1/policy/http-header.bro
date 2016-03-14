# $Id: http-header.bro 3920 2007-01-14 00:27:55Z vern $

# Prints out detailed HTTP headers.

module HTTP;

export {
	# The following lets you specify headers that you don't want
	# to print out.
	global skip_header: set[string] &redef;

	# For example:
	# 	redef skip_header += { "COOKIE", "SET-COOKIE" };
	# will refrain from printing cookies.
}

event http_header(c: connection, is_orig: bool, name: string, value: string)
	{
	if ( name in skip_header )
		return;

	local s = lookup_http_request_stream(c);

	print http_log, fmt("%.6f %s %s %s: %s",
				network_time(), s$id,
				is_orig ? ">" : "<", name, value);
	}
