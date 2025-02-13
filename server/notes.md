✅ fix/add IP in RPL_WELCOME
✅ part #a,#b -> Only leaves channel a and not b
✅ nc is currently not working NICK PASS or other commands is not interpreted as commands
✅ join #a,#b -> Needs to create two different channels, creates one channel as #a,#b

PART needs to send feedback to hexchat/client about user leaving to other users
check if a "hostname" REPLY is necessarry or nick is sufficient in irc

Fix JOIN's auto assumption mode +i for channels
Fix JOIN's return statements (returns if the first channel's invite only in multiple channel join)
Fix PART's return statements (returns if not on first channel in multiple channels)
Add a mechanism to DELETE CHANNELS if there is no user left in PART and QUIT
Change server's general user delete section to call QUIT for deleting users (so that it deletes channels too and sends quit messages)
Add password check for mode +k in JOIN

Fix issue where changing channel limit (mode +l) jams server and does not enable users join
Check if setting +l 2 in a channel that has 3 users causes issues
