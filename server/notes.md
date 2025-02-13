✅ fix/add IP in RPL_WELCOME
✅ part #a,#b -> Only leaves channel a and not b
	(added vecSplit for targets)
✅ nc is currently not working NICK PASS or other commands is not interpreted as commands
	(nc "-C" was forgotten)
✅ join #a,#b -> Needs to create two different channels, creates one channel as #a,#b
	(added vecSplit for targets)

PART needs to send feedback to hexchat/client about user leaving to other users
check if a "hostname" REPLY is necessarry or nick is sufficient in irc

✅ Fix JOIN's auto assumption mode +i for channels
	(added initializer in constructor)
✅ Fix JOIN's return statements (returns if the first channel's invite only in multiple channel join)
	(changed returns to continue, fixed RPL's for name list and topic, minor adjustments)
✅ Fix PART's return statements (returns if not on first channel in multiple channels)
	(changed returns to continue, removed unnecesarry if, rearranged messages)
✅ Add a mechanism to DELETE CHANNELS if there is no user left in commands PART and QUIT
	(added joined channels list to users, added and removed channels from list with join and part,
	added Channel::removeUser() to setver::removeClient() by traversing users channel list)
Change server's general user delete section to call QUIT for deleting users (so that it deletes channels too and sends quit messages)

Add password check for mode +k in JOIN
Fix issue where changing channel limit (MODE +l) jams server and does not enable users JOIN
Check if setting MODE +l 2 in a channel that has 3 users causes issues
