# Class - client_connection_info
* `client_connection_info.cancel:bool` - Defaults to false, when true, the client will be disconnected
* `client_connection_info.reason:string` - Sent to the client if `cancel` was true
* `client_connection_info.port:number` - The TCP or UDP port number the client is connecting from (not typically useful)
* `client_connection_info.address:string` - The IPv4 or IPv6 address of the client
* `client_connection_info.family:string` - The client address type, either `ipv4` or `ipv6`
* `client_connection_info.protocol:string` - The transport the client is connected with, either `tcp` or `udp`