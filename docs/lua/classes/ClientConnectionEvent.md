# Class - ClientConnectionEvent
* `ClientConnectionEvent.cancel:bool` - Defaults to false, when true, the client will be disconnected
* `ClientConnectionEvent.reason:string` - Sent to the client if `cancel` was true
* `ClientConnectionEvent.port:number` - The TCP or UDP port number the client is connecting from (not typically useful)
* `ClientConnectionEvent.address:string` - The IPv4 or IPv6 address of the client
* `ClientConnectionEvent.family:string` - The client address type, either `ipv4` or `ipv6`
* `ClientConnectionEvent.protocol:string` - The transport the client is connected with, either `tcp` or `udp`
