# Class - UserList
* `UserList:AddUser(user)` - Saves game data using already assigned filename
	* `user`:[User](User.md)
* `UserList:CreateUser(user_id)` - Makes a user object, assigns user_id, and calls `Add`
	* `user_id:string` - ID of the user to create and add
* `UserList:GetUser(user_id)` - Gets a user from the list
	* `user_id:string` - User id of the user to get
* `UserList:RemoveUser(user_id)` - Remove a user from the list
	* `user_id:string` - User id of the user to remove
* `UserList:HasUser(user_id)` - Finds a user
	* `user_id:string` - User id of the user to find
