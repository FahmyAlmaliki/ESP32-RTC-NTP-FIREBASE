# 🔔 ESP32 + RTC + Firebase Alarm System

Proyek ini mengintegrasikan **ESP32**, **RTC DS3231**, dan **Firebase Realtime Database** untuk membuat sistem alarm IoT yang dapat disinkronisasi waktunya melalui **NTP** berdasarkan perintah dari Firebase.

---

## 🚀 Fitur

- Koneksi WiFi otomatis
- Pembacaan waktu dari RTC DS3231
- Sinkronisasi waktu melalui NTP (GMT+7 / WIB)
- Pemicu kalibrasi waktu dari Firebase
- Penulisan log waktu ke Serial Monitor

---

## 🛠️ Perangkat Keras

- ESP32 board
- RTC DS3231 module
- Koneksi internet via WiFi

---

## 🔧 Instalasi

### 1. **Library Arduino yang Dibutuhkan**
Pastikan kamu sudah menginstal library berikut melalui **Library Manager**:

- `WiFi.h` (bawaan ESP32)
- `Wire.h` (bawaan Arduino)
- `RTClib` oleh Adafruit
- `WiFiUdp.h` (bawaan ESP32)
- `NTPClient` oleh Fabrice Weinberg
- `Firebase_ESP_Client` oleh Mobizt

---

## ⚙️ Konfigurasi

Edit bagian berikut pada file `main.ino`:

```cpp
const char* ssid = "masukkan_SSID_anda";
const char* password = "masukkan_PASSWORD_anda";

#define API_KEY "masukkan_API_KEY_anda"
#define DATABASE_URL "masukkan_DATABASE_URL_anda" // Contoh: https://your-project-id.firebaseio.com/
