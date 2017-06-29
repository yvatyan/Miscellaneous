__cd.php			[COMMAND]	changes current directory, by storing it in @(current directory)
_download.php	 	[COMMAND]	zipes file to "temporary" folder from @(current directory) and returns it's url
_echo.php		 	[COMMAND]	echos recieved message
_ls.php			 [COMMAND]	returns contents of the @(current directory)
clear_session.php		[EXTERNAL]	deletes "*.props" files and erases files from "temporary" folder
commands.php		[EXTERNAL]	calls needed commands
filesystem.props		[PROPERTY]	stores @(current directory) variable for commands {ls, download}
panel.php			[EXTERNAL]	returns online\offline users ip's
ping.php			[EXTERNAL]	updates "user_ip" file's mod time
readme.txt			[INFO]		this file
rqp_rsp.php		[INTERNAL]	store RQP and RSP functions for use in different scripts