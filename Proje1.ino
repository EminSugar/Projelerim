// D2 pinine bağlı olan LED'i 1sn aralıklarla yakıp söndürme
//-------------------------------------------------
#include <ESP8266WiFi.h>
#include <dht11.h> // dht11 kütüphanesini ekliyoruz.
#define DHT11PIN 4 // DHT11PIN olarak Dijital 2"yi belirliyoruz.

const char *ssid = "Sugar";          // bağlanılacak olan kablosuz ağın SSID si
const char *password = "Emin249.";           // bağlanılacak olan kablosuz ağın şifresi

const char *sunucu_IP = "maker.ifttt.com";           // bağlanılacak olan web sayfasının adresi
String yol = "/trigger/DeneyDolabi/with/key/f6wYsaHkXUt7npgK434QwPnJg-TXud8GsRpM4wwoWuj";            // bağlanılacak olan web sayfasının adresi 

dht11 DHT11;
int sicaklik, nem;

void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  
  pinMode(D5, OUTPUT);
  pinMode(D7, OUTPUT);
  
  Serial.begin(9600);
  delay(1000);

  WiFi.mode(WIFI_STA);                        //ESP8266 istasyon moduna alınıyor.
  WiFi.begin(ssid, password);                 //SSID ve şifre girilierek wifi başlatılıyor.
  Serial.print("Bağlantı kuruluyor");
  while (WiFi.status() != WL_CONNECTED)
  { // Wifi bağlantısı sağlanana kadar bekleniyor.
    delay(500);                               // Bu arada her 500 ms de bir seri ekrana yan yana noktalar yazdırılarak
    Serial.print(".");                        //görsellik sağlanıyor.
  }
  Serial.println("");                         //Bir alt satıra geçiliyor.
  Serial.println("Bağlantı sağlandı...");      //seri ekrana bağlantının kurulduğu bilgisi gönderiliyor.
}
//-------------------------------------------------
void loop() {
  WiFiClient istemci; // istemci nesnesi oluşturuldu

  DHT11.read(DHT11PIN);
  sicaklik = DHT11.temperature;
  Serial.println("Sıcaklık: " + String(sicaklik));
  
  DHT11.read(DHT11PIN);
  nem = DHT11.humidity;
  Serial.println("Nem: " + String(nem));
  delay(3000);
  if (sicaklik >= 28 ||sicaklik <= 25|| nem>=50||nem<=30)
  {
    if (!istemci.connect(sunucu_IP, 80))           // web bağlantısı yap, bağlanamadı mı?
    {
      Serial.println("Baglanti kurulamadi!!!");    // bağlanamadı, hata ver
      return;
    }
     istemci.print(String("GET ") + yol + " HTTP/1.1\r\n" + "Host: " + sunucu_IP + "\r\n" + "Connection: close\r\n\r\n");
    Serial.println("Mail Gönderildi");
  }
   for(int i =0;i<=10; i++)
    {
      if(sicaklik >= 28)
        digitalWrite(D0,HIGH);    // LED'i yaktir
      if(sicaklik <= 25)
        digitalWrite(D1,HIGH);    // LED'i yaktir
      if (nem>=50)
         digitalWrite(D5,HIGH);    // LED'i yaktir   
      if (nem<=40)
         digitalWrite(D7,HIGH);    // LED'i yaktir   
        
        delay(250);
        digitalWrite(D0,LOW);     // LED'i sondur
        digitalWrite(D1,LOW); 
        digitalWrite(D5,LOW); 
        digitalWrite(D7,LOW); 
        delay(250);  
     }
}
