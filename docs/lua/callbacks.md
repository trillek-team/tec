# Callbacks
* `update(delta)` - Called every game loop with the amount of time passed (delta) since the last call
	* delta:numebr - The time since the last call
* `onClientConnected(event)` - Called when a client connects to the server, but before we send anything
	* event:[client_connection_info](classes/client_connection_info.md)
 
```lua
function onClientConnected(info)
    info.cancel = (info.address ~= "192.168.7.7") -- only allow this super special address
    info.reason = "Sorry! This server doesn't like you"
    print("Client connected:", info.protocol, info.family, info.address .. ":" .. info.port)
end
```

* `onClientDisconnected(event)` - Called when a client is disconnected from the server
	* event:[client_connection_info](classes/client_connection_info.md)
* `onClientJoin(client)` - Called when a client (user) is entering the world such as after login
	* client:[ClientConnection](classes/ClientConnection.md)
* `onClientLeave(client)` - Called when a client (user) is leaving the world such as after logout
	* client:[ClientConnection](classes/ClientConnection.md)
* `onUserLogin(client, user)` - Called when a user is logged in. **ASSIGN A USER BEFORE RETURNING**
	* client:[ClientConnection](classes/ClientConnection.md)
	* user:[User](classes/User.md)

```lua
function onUserLogin(client, user)
	user_list = save.user_list
	user = user_list:FindUser(username)
	client.user = user
end
```

* `onChatCommand(command:string, args:vector)` - Called to handle ChatCommandEvent
	* command:string The command itself
	* args:vector Arguments passed to the command split on space.