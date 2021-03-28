# Class - user_login_info
* `user_login_info.username:string` - The username for the login attempt
* `user_login_info.user_id:string` - Set this to the ID of the user, if the login was successful
* `user_login_info.reason:string` - Sent to the client if `reject` was true
* `user_login_info.reject:bool` - Whether to reject the login, for example invalid username
* `user_login_info.authenticated:bool` - If the user was authenticated via the server. Can be overridden to force authentication.
