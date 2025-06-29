/*
  ========================================
  📌 WiFi + RTC + NTP + Firebase Integration
  ========================================
  Author     : FahmyAlmaliki
  Project    : IoT Alarm System
  Hardware   : ESP32 + RTC DS3231
  Features   :
    - WiFi connection
    - RTC DS3231 read/write
    - NTP time sync (WIB)
    - Firebase Realtime Database trigger
  ========================================
*/

#include <WiFi.h>
#include <Wire.h>
#include <RTClib.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Firebase_ESP_Client.h>

// ========== Konfigurasi WiFi ==========
const char* ssid = "masukkan_SSID_anda";
const char* password = "masukkan_PASSWORD_anda";

// ========== Firebase Configuration ==========
#define API_KEY "masukkan_API_KEY_anda"
#define DATABASE_URL "masukkan_DATABASE_URL_anda"  // contoh: https://your-project.firebaseio.com/

// ========== Objek Global ==========
RTC_DS3231 rtc;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600); // GMT+7 (WIB)

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Koneksi WiFi
  Serial.print("🔌 Menghubungkan ke WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Terhubung ke WiFi");

  // Inisialisasi RTC
  if (!rtc.begin()) {
    Serial.println("❌ RTC tidak terdeteksi!");
    while (1);
  }

  // Firebase setup
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("✅ Login Firebase Anonymous berhasil");
  } else {
    Serial.printf("❌ Gagal login Firebase: %s\n", config.signer.signupError.message.c_str());
  }

  // NTP client
  timeClient.begin();
}

void loop() {
  // Baca waktu dari RTC
  DateTime now = rtc.now();
  Serial.printf("🕒 RTC: %02d/%02d/%04d %02d:%02d:%02d\n",
                now.day(), now.month(), now.year(),
                now.hour(), now.minute(), now.second());

  // Baca perintah kalibrasi dari Firebase
  if (Firebase.RTDB.getBool(&fbdo, "/kalibrasi")) {
    if (fbdo.dataType() == "boolean" && fbdo.boolData()) {
      Serial.println("⚙️  Perintah kalibrasi diterima!");
      sinkronisasiWaktuNTP();

      // Reset flag kalibrasi
      Firebase.RTDB.setBool(&fbdo, "/kalibrasi", false);
    }
  } else {
    Serial.println("❌ Gagal membaca Firebase: " + fbdo.errorReason());
  }

  delay(500);
}

void sinkronisasiWaktuNTP() {
  Serial.println("🌐 Sinkronisasi waktu dari NTP...");
  
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }

  unsigned long epoch = timeClient.getEpochTime();
  rtc.adjust(DateTime(epoch));

  Serial.print("✅ RTC disinkronisasi ke: ");
  Serial.println(timeClient.getFormattedTime());
}
