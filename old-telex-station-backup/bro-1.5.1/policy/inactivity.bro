# $Id: inactivity.bro 4669 2007-07-30 20:55:24Z vern $

@load port-name

const inactivity_timeouts: table[port] of interval = {
	[smtp] = 30 secs,
	[[http, 8000/tcp, 8080/tcp]] = 30 secs,

	# For interactive services, allow longer periods of inactivity.
	[[telnet, rlogin, ssh, ftp]] = 1 hrs,
} &redef;

function determine_inactivity_timeout(c: connection)
	{
	local service = c$id$resp_p;

	# Determine service (adapted from hot.bro)
	if ( c$orig$state == TCP_INACTIVE )
		{
		# We're seeing a half-established connection. Use the
		# service of the originator if it's well-known and the
		# responder isn't.
		if ( service !in port_names && c$id$orig_p in port_names )
			service = c$id$orig_p;
		}

	if ( service in inactivity_timeouts )
		set_inactivity_timeout(c$id, inactivity_timeouts[service]);
	}

event connection_established(c: connection)
	{
	determine_inactivity_timeout(c);
	}
