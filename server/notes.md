✅ Add limit check to JOIN
✅ Add mode +i check to JOIN
✅ Show the existing TOPIC for a new user in channel
✅ Check if errors are handled server bind
✅ Fix auto # assumption for CHANNELS

✅ TOUPPER (komutlar küçük yazınca gitmiyor) (PRIVMSG bir standart hexchat komutu değilmiş)
✅ fix/add IP in RPL_WELCOME
PART needs to send feedback to hexchat/client about user leaving to other users
✅ fix NICK's reply by adding a :prefix
check if a "hostname" REPLY is necessarry or nick is sufficient in irc
✅ make function msgAllUsers()
✅ fix netcat ctrl^C (check for POLLHUP) add server.removeUser()
✅ Add QUIT
✅ part #a,#b -> Only leaves channel a and not b

nc is currently not working NICK PASS or other commands is not interpreted as commands
join #a,#b -> Needs to create two different channels, creates one channel as #a,#b

Check password if +k flag is set in JOIN
Fix issue where changing channel limit (mode +l) jams server and does not enable users join
Check if setting +l 2 in a channel that has 3 users causes issues
