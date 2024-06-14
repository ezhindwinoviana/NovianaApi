#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// Deklarasi Variable dan Konstanta
const char* wifiSSID = "Choirul Amri";
const char* wifiPassword = "fastabiqul1976";
const char* serverAddress = "http://192.168.1.8:5000";  // Alamat local dari server flask

#define DHTPIN 2       // Pin terhubung dengan data sensor DHT (GPIO 2)
#define DHTTYPE DHT11  // sensor DHT22

// Inisialisasi sensor DHT
DHT dht(DHTPIN, DHTTYPE);

void connectWifi();
void postHttp(float temperature, float humidity);

void setup() {
  Serial.begin(9600);
  dht.begin();  // Memulai sensor DHT
  connectWifi();
}

void loop() {
  // Baca data dari sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Cek apakah pembacaan berhasil
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    // Kirim data ke server
    postHttp(temperature, humidity);
  }

  // jeda 10 detik untuk membaca data
  delay(10000);
}

void postHttp(float temperature, float humidity) {
  String url = String(serverAddress) + "/api/data";  // Konsisten dengan endpoint
  HTTPClient http;
  String response;

  StaticJsonDocument<200> buff;
  String jsonParams;
  buff["temp"] = temperature;
  buff["humidity"] = humidity;

  serializeJson(buff, jsonParams);
  Serial.println("Sending data to server: ");
  Serial.println(jsonParams);

  http.begin(url);
  http.addHeader("Content-Type", "application/json"); 
  int httpResponseCode = http.POST(jsonParams);

  if (httpResponseCode > 0) {
    response = http.getString();
    Serial.println("Response from server: ");
    Serial.println(response);
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void connectWifi() {
  Serial.println("Connecting to WiFi");
  WiFi.begin(wifiSSID, wifiPassword);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nWiFi Connected");
  Serial.println("SSID: " + String(WiFi.SSID()));
  Serial.println("Signal strength (RSSI): " + String(WiFi.RSSI()));
  Serial.println("MAC address: " + WiFi.macAddress());
  Serial.println("Local IP: " + WiFi.localIP().toString());
  Serial.println("Gateway IP: " + WiFi.gatewayIP().toString());

  // Cek koneksi internet dengan HTTP request sederhana
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://www.google.com");
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.println("Connected to Internet");
    } else {
      Serial.println("No Internet connection");
    }
    http.end();
  }
}
