# Callbacks
* `update(delta)` - Called every game loop with the amount of time passed (delta) since the last call
	* delta:number - The time since the last call
* `onClientConnected(event)` - Called when a client connects to the server, but before we send anything
	* event:[ClientConnection](classes/ClientConnection.md)
 
```lua
function onClientConnected(info)
    info.cancel = (info.address ~= "192.168.7.7") -- only allow this super special address
    info.reason = "Sorry! This server doesn't like you"
    print("Client connected:", info.protocol, info.family, info.address .. ":" .. info.port)
end
```

* `onClientDisconnected(event)` - Called when a client is disconnected from the server
	* event:[ClientConnection](classes/ClientConnection.md)
* `onClientJoin(client)` - Called when a client (user) is entering the world such as after login
	* client:[ClientConnection](classes/ClientConnection.md)
* `onClientLeave(client)` - Called when a client (user) is leaving the world such as after logout
	* client:[ClientConnection](classes/ClientConnection.md)
* `onUserLogin(user_login_info)` - Called when a user is logged in.
	* user_login_info:[UserLoginEvent](classes/UserLoginEvent.md)

```lua
function onUserLogin(user_login_info)
	print("Login by (" .. user_login_info.user_id .. ") " .. user_login_info.username)
end
```

* `onChatCommand(command:string, args:vector)` - Called to handle ChatCommandEvent
	* command:string The command itself
	* args:vector Arguments passed to the command split on space.
* `onPlayerJoin(ip:string, identifier:string)` - Called when a player joins the game
	* ip:string The IP address of the player
	* identifier:string The identifier of the player
* `onPlayerLeave(identifier:string)` - Called when a player leaves the game
	* identifier:string The identifier of the player
* `onPlayerInteraction(identifier:string, entity_id:number, interaction_type:string)` - Called when a player interacts with an object
	* identifier:string The identifier of the player
	* entity_id:number The ID of the entity being interacted with
	* interaction_type:string The type of interaction
* `onEntitySpawning(entity_id:number)` - Called when an entity is spawning
	* entity_id:number The ID of the entity being spawned
* `onTeleport(identifier:string, x:number, y:number, z:number)` - Called to handle teleportation
	* identifier:string The identifier of the player
	* x:number The x-coordinate to teleport to
	* y:number The y-coordinate to teleport to
	* z:number The z-coordinate to teleport to
* `onKick(identifier:string)` - Called to handle kicking a player
	* identifier:string The identifier of the player
