1//join dosyasına limit eklenecek.
2//mode +i var olduğu durumda join ile girmemesi lazım.
3//bir kullanıcıyı kanala invite ederken kanalda değil serverda ara.
4//topic yeni bir kullancı girildiğinde önceden oluşturulmuş konuyu görmesi.
5//server da bind ve socket hataları
kanal vb durumlarda ilk girerken /join komutunun başında #olmasa bile otomatik olarak ekleyip giriyoruz bunun yerine #koyma zorunluluğu olsun ki örneğin ##ahmet gibi bir kanal oluşturulmak istendiğinde yani kanalın adı "#ahmet" olduğu durumda yanlışlıkla "ahmet" olarak algılanmasın.

join için +k flagi ve kanal şifresi kontrol edilmesi gerekiyor.
+l flagi ile kullanıcı sınırladıktan sonra kanala giremiyor ama sınır değiştirilince server bugda kalıyor.