event tcp_option(c: connection, is_orig:bool, opt: count, optlen: count, option: string) {
  #print fmt("TCP option: %d length: %d contents: %s", opt, optlen, option);
  if (opt == 8) {
     print fmt("It's a TCP timestamp: %d, %d", extract_tcp_timestamp(option, F),
                                           extract_tcp_timestamp(option, T));
  }
}
