Bu proje, tarım süreçlerinde su kaynaklarının daha verimli kullanılmasını sağlamak için akıllı bir sulama sistemi geliştirmeyi amaçlamaktadır. Proje, çevresel verileri algılayarak otomatik bir şekilde sulama yapabilen ve kullanıcıya veri gönderebilen bir sistem tasarımını içermektedir.

Sistem Bileşenleri
Sistem, çeşitli sensörler, aktüatörler ve bir mikrodenetleyici modülünden oluşmaktadır:
1.	DHT11 Sensörü: Ortam sıcaklığı ve nem değerlerini ölçmek için kullanılmıştır.
2.	Servo Motor: Sulama vanasını açma ve kapama işlemleri için kullanılmıştır.
3.	Su Seviyesi Sensörü: Su miktarını ölçerek sistemin sulama yapıp yapmayacağına karar vermesini sağlar.
4.	Buzzer: Kapak açma ve kapama durumlarında sesli uyarı vermek için kullanılmıştır.
5.	LCD Ekran: Sıcaklık, nem ve su seviyesi gibi verilerin kullanıcıya görsel olarak iletilmesini sağlar.
6.	ESP32 Mikrodenetleyici: Tüm sistemi kontrol eden ve verileri işleyen ana birimdir. Ayrıca, Firebase entegrasyonu ile uzaktan veri erişimi sağlar.

 Çalışma Prensibi
Sistem, DHT11 sensöründen sıcaklık ve nem değerlerini, su seviyesi sensöründen ise mevcut su miktarını okur. Bu veriler, LCD ekranda görüntülenir ve Firebase’e gönderilir. Eğer su seviyesi belirlenen eşik değerinin altına düşerse, servo motor vasıtasıyla sulama sistemi devreye girer ve buzzer bir uyarı sesi çıkarır. Su seviyesi yeterli olduğunda ise vana kapanır ve buzzer tekrar bir bildirim yapar.

Teknolojiler ve Yazılım
Proje, Arduino IDE programı kullanılarak geliştirilmiştir. WiFi bağlantısı ile Firebase gerçek zamanlı veritabanı üzerinden veri depolama ve izleme yapılmıştır. Kullanıcı, bu verilere internet üzerinden erişebilir.

Proje Sonuçları ve Kazanımları
Proje, tarımda manuel sulama işlemlerinin otomasyonuna olanak tanıyarak su tasarrufu sağlamıştır. Sistem, çevre koşullarına duyarlı ve uzaktan izlenebilir bir yapıya sahiptir. Bu çalışma, tarımsal alanlarda sürdürülebilirlik ve verimliliği artıracak bir çözüm sunmaktadır.
