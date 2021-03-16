# Callbacks
* `update(delta:number)` - Called every game loop with the amount of time passed (delta) since the last call
* `onChatCommand(command:string, args:vector)` - Called to handle ChatCommandEvent
* `onClientConnected(event:client_connection_info)` - Called when a client connects to the server, but before we send anything
  * `client_connection_info.cancel:bool` - Defaults to false, when true, the client will be disconnected
  * `client_connection_info.reason:string` - Sent to the client if `cancel` was true
  * `client_connection_info.port:number` - The TCP or UDP port number the client is connecting from (not typically useful)
  * `client_connection_info.address:string` - The IPv4 or IPv6 address of the client
  * `client_connection_info.family:string` - The client address type, either `ipv4` or `ipv6`
  * `client_connection_info.protocol:string` - The transport the client is connected with, either `tcp` or `udp`
 
```lua
function onClientConnected(info)
    info.cancel = (info.address ~= "192.168.7.7") -- only allow this super special address
    info.reason = "Sorry! This server doesn't like you"
    print("Client connected:", info.protocol, info.family, info.address .. ":" .. info.port)
end
```

* `onClientDisconnected(event:client_connection_info)` - Called when a client is disconnected from the server
  * provides the same `client_connection_info` as connect, however, the cancel and reason fields have no effect.
