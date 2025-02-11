Add limit check to JOIN
Add mode +i check to JOIN
Show the existing TOPIC for a new user in channel
Check if errors are handled server bind

TOUPPER (komutlar küçük yazınca gitmiyor) PRIVMSG????
fix/add IP in RPL_WELCOME
PART needs to send feedback to hexchat/client about user leaving to other users
fix NICK's reply by adding a :prefix
check if a "hostname" REPLY is necessarry or nick is sufficient in irc
make function sendMsgToAll()
✅ fix netcat ctrl^C (check for POLLHUP) add server.removeUser()

kanal vb durumlarda ilk girerken /join komutunun başında #olmasa bile otomatik olarak ekleyip giriyoruz bunun yerine #koyma zorunluluğu olsun ki örneğin ##ahmet gibi bir kanal oluşturulmak istendiğinde yani kanalın adı "#ahmet" olduğu durumda yanlışlıkla "ahmet" olarak algılanmasın.

join için +k flagi ve kanal şifresi kontrol edilmesi gerekiyor.
+l flagi ile kullanıcı sınırladıktan sonra kanala giremiyor ama sınır değiştirilince server bugda kalıyor.
