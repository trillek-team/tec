# Class - UserLoginEvent
* `UserLoginEvent.username:string` - The username for the login attempt
* `UserLoginEvent.user_id:string` - Set this to the ID of the user, if the login was successful
* `UserLoginEvent.reason:string` - Sent to the client if `reject` was true
* `UserLoginEvent.reject:bool` - Whether to reject the login, for example invalid username
* `UserLoginEvent.authenticated:bool` - If the user was authenticated via the server. Can be overridden to force authentication.
