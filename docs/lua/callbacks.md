# Callbacks
* `update(delta:number)` - Called every game loop with the amount of time passed (delta) since the last call
* `onChatCommand(command:string, args:vector)` - Called to handle ChatCommandEvent
* `onClientConnected(event)` - Called when a client connects to the server, but before we send anything
	* event - [client_connection_info](classes/client_connection_info.md)
 
```lua
function onClientConnected(info)
    info.cancel = (info.address ~= "192.168.7.7") -- only allow this super special address
    info.reason = "Sorry! This server doesn't like you"
    print("Client connected:", info.protocol, info.family, info.address .. ":" .. info.port)
end
```

* `onClientDisconnected(event)` - Called when a client is disconnected from the server
	* event - [client_connection_info](classes/client_connection_info.md)
