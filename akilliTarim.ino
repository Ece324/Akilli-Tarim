#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <ESP32Servo.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// WiFi ve Firebase bilgileri
#define WIFI_SSID "Galaxy A53 5G"
#define WIFI_PASSWORD "cghe0424"
#define API_KEY "AIzaSyBZiU94j5DAiwXDftcxdj165lhT-OrrJXI"
#define DATABASE_URL "deneme-2f06c-default-rtdb.firebaseio.com"

// Firebase nesneleri
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Donanım pinleri
#define WATER_SENSOR_PIN 34 // Su seviyesi sensörü pini
#define SERVO_PIN 26        // Servo motor pini
#define DHTPIN 4            // DHT11 sensörü veri pini
#define DHTTYPE DHT11       // DHT11 sensör tipi
#define BUZZER_PIN 27       // Buzzer pini

// Servo, DHT ve LCD nesneleri
Servo servo;
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int waterLevel = 0;             // Su seviyesi değeri
const int WATER_LEVEL_THRESHOLD = 500; // Su seviyesi için eşik değer
float temperature = 0.0;        // Sıcaklık
float humidity = 0.0;           // Nem

int displayMode = 0;            // Gösterim modu
unsigned long lastDisplayChange = 0;

void setup() {
  // Seri iletişim başlat
  Serial.begin(115200);

  // LCD kurulumu
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Baglanti kuruluyor");

  // WiFi bağlantısı
  Serial.println("WiFi'ye baglaniliyor...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    lcd.setCursor(0, 1);
    lcd.print("WiFi...");
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi baglantisi kuruldu!");
  lcd.clear();
  lcd.print("WiFi Baglandi!");

  // Firebase ayarları
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  Firebase.signUp(&config, &auth, "", "");
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  lcd.setCursor(0, 1);
  lcd.print("Firebase baglandi");

  // Servo motor kurulumu
  servo.attach(SERVO_PIN);
  servo.write(0); // Başlangıç pozisyonu

  // Buzzer pini
  pinMode(BUZZER_PIN, OUTPUT);

  // Su seviyesi sensörü pini
  pinMode(WATER_SENSOR_PIN, INPUT);

  // DHT sensör başlatma
  dht.begin();

  delay(2000); // Stabilizasyon için bekleme
}

// Yeni değişken
bool isLidOpen = false; // Kapak durumu: false = kapalı, true = açık

void loop() {
  // Su seviyesini oku
  waterLevel = analogRead(WATER_SENSOR_PIN);
  Serial.print("Okunan su seviyesi: ");
  Serial.println(waterLevel);

  // DHT11 sıcaklık ve nemi oku
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT11 verisi okunamadi!");
    return;
  }

  // Firebase'e verileri gönder
  Firebase.RTDB.setInt(&fbdo, "/waterLevel", waterLevel);
  Firebase.RTDB.setFloat(&fbdo, "/temperature", temperature);
  Firebase.RTDB.setFloat(&fbdo, "/humidity", humidity);

  // LCD'de bilgileri göstermek için mod değiştir
  if (millis() - lastDisplayChange > 5000) { // Her 5 saniyede bir mod değiştir
    displayMode = (displayMode + 1) % 3; // Toplam 3 farklı mod
    lastDisplayChange = millis();
  }

  // LCD bilgilerini modlara göre göster
  lcd.clear();
  if (displayMode == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Sicaklik: ");
    lcd.print(temperature, 1);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Nem: ");
    lcd.print(humidity, 1);
    lcd.print("%");
  } else if (displayMode == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Su Seviyesi:");
    lcd.setCursor(0, 1);
    lcd.print(waterLevel);
  } else if (displayMode == 2) {
    lcd.setCursor(0, 0);
    lcd.print(waterLevel < WATER_LEVEL_THRESHOLD ? "Kapak Acik" : "Kapak Kapali");
    lcd.setCursor(0, 1);
    lcd.print("Su durumu kontrol");
  }

  // Su seviyesi kontrolü ve servo motor
  if (waterLevel < WATER_LEVEL_THRESHOLD && !isLidOpen) {
    // Kapak açılıyor
    Serial.println("Kapak aciliyor...");
    servo.write(0); // Servo motor çalışır
    digitalWrite(BUZZER_PIN, HIGH); // Buzzer çalar
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);
    isLidOpen = true; // Kapak açık
  } else if (waterLevel >= WATER_LEVEL_THRESHOLD && isLidOpen) {
    // Kapak kapanıyor
    Serial.println("Kapak kapanıyor...");
    servo.write(180); // Servo başlangıç pozisyonuna döner
    digitalWrite(BUZZER_PIN, HIGH); // Buzzer çalar
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    isLidOpen = false; // Kapak kapalı
  }

  delay(500); // Döngü için kısa gecikme
}

