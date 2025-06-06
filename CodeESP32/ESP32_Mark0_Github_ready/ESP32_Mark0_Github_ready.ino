/*************************************************************
  ESP32 + DHT11 + Blynk + 1.54" e-Ink Display (Heltec)
  Shows Temp & Humidity on Blynk app and e-Ink every 2 minutes
 *************************************************************/

#define BLYNK_PRINT Serial

#include "secrets.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <GxEPD2_BW.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <ArduinoOTA.h>
#include <HTTPClient.h>        // 🆕 For HTTP requests
#include <ArduinoJson.h>       // 🆕 For JSON parsing

// ==== WiFi Credentials ====
char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASSWORD;

// ===== OpenWeatherMap setup ====
const char* city = "Brno";
const char* countryCode = "CZ";
const char* weatherApiKey = WEATHER_API_KEY; // 🆕 Insert your real key

// 🆕 Struct to hold weather info
struct WeatherData {
  float temp;
  float feels_like;
  String description;
};

// 🆕 Fetch weather from OpenWeatherMap
WeatherData fetchWeather() {
  WeatherData data = {0, 0, "Načítám..."};

  String url = "http://api.openweathermap.org/data/2.5/weather?q=" + String(city) + "," + String(countryCode) + "&units=metric&appid=" + weatherApiKey + "&lang=cz";
  HTTPClient http;
  http.begin(url);

  int httpCode = http.GET();
  if (httpCode == 200) {
    String payload = http.getString();

    StaticJsonDocument<1024> doc;
    deserializeJson(doc, payload);

    data.temp = doc["main"]["temp"];
    data.feels_like = doc["main"]["feels_like"];
    data.description = doc["weather"][0]["description"].as<String>();
  } else {
    Serial.println("❌ Failed to fetch weather");
  }

  http.end();
  return data;
}


// ==== DHT Setup ====
#define DHTPIN 4          // GPIO4 for DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ==== E-Ink Setup (Heltec 1.54", 200x200) ====
#define EPD_CS    5   // Chip Select
#define EPD_DC    17  // Data/Command
#define EPD_BUSY  2   // Busy (Changed from GPIO 4)
#define EPD_CLK   18  // Clock (SPI SCK)
#define EPD_MOSI  23  // Data Input (SPI SDI)
#define EPD_RST   -1  // Not used (-1 means it's not connected)
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

// ==== Blynk Timer ====
BlynkTimer timer;

// ✅ Function to get current time as HH:MM:SS string
String getCurrentTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "--:--:--";
  char buf[9];
  strftime(buf, sizeof(buf), "%H:%M:%S", &timeinfo);
  return String(buf);
}

// Funkce na vypocet uptimu

String getUptime() {
  unsigned long uptimeMillis = millis();
  unsigned long hours = uptimeMillis / 3600000;
  unsigned long minutes = (uptimeMillis % 3600000) / 60000;
  unsigned long seconds = (uptimeMillis % 60000) / 1000;
  
  char uptimeString[9]; // HH:MM:SS format
  snprintf(uptimeString, sizeof(uptimeString), "%02lu:%02lu:%02lu", hours, minutes, seconds);
  
  return String(uptimeString);
}

// ==== Function: Read sensor and update Blynk + e-Ink ====
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("❌ Failed to read from DHT sensor!");
    return;
  }

  // 🆕 Fetch weather BEFORE using its values
  WeatherData weather = fetchWeather();

  // Uptime
  String uptime = getUptime();

  // 🕒 Print time and uptime
  String currentTime = getCurrentTime();
  Serial.println("⏰ Čas: " + currentTime);
  Serial.println("⏰ Uptime: " + uptime); // Log uptime

  // 🌡️ Print indoor temp & humidity
  Serial.printf("✅ Uvnitř: %.1f°C, Vlhkost: %.1f%%\n", t, h);

  // 🌤️ Print weather data
  Serial.printf("🌡️  Venku: %.1f°C (Pocitová: %.1f°C)\n", weather.temp, weather.feels_like);
  Serial.print("🌥️  Popis: ");
  Serial.println(weather.description);

  String location = String(city) + ", " + countryCode;

  // ✅ Send to Blynk
  Blynk.virtualWrite(V1, uptime);         // Send uptime to Virtual Pin V1
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V7, weather.temp);
  Blynk.virtualWrite(V8, weather.feels_like);
  Blynk.virtualWrite(V9, weather.description);

  // ✅ Show on e-Ink
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeMonoBold9pt7b);

    display.setCursor(10, 20);
    display.print("Uptime: ");
    display.print(uptime);

    display.setCursor(10, 40);
    display.print("Cas: ");
    display.print(getCurrentTime());

    display.setCursor(10, 70);
    display.print(location);

    display.setCursor(10, 100);
    display.printf("%.1f°C (pocit %.1f°C)", weather.temp, weather.feels_like); 

    display.setCursor(10, 130);
    display.print(weather.description);

    display.setCursor(20, 160);
    display.print("Teplota: ");
    display.print(t, 1);
    display.print(" °C");

    display.setCursor(20, 190);
    display.print("Vlhkost: ");
    display.print(h, 1);
    display.print(" %");
  } while (display.nextPage());

  Serial.println("🖥️  E-Ink display updated.");
}


void setup()
{
  Serial.begin(115200);
  Serial.println("Booting...");

  // Init Blynk + WiFi
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Init DHT sensor
  dht.begin();

  // Init e-Ink display
  display.init();
  display.setRotation(3);

  // ✅ Setup NTP time sync
  // Timezone config (Central Europe, with DST auto-switch)
  setenv("TZ", "CET-1CEST,M3.5.0/2,M10.5.0/3", 1);
  tzset();

  // Start NTP sync
  configTime(3600, 3600, "pool.ntp.org", "time.nist.gov");

  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.println("⏳ Waiting for time...");
    delay(1000);
  }
  Serial.println("🕒 Time synchronized");

  // Start OTA
  ArduinoOTA.setHostname("ESP32-DHT-EINK");
  ArduinoOTA.begin();
  Serial.println("🔄 OTA Ready!");

  // Every 180s = 3min (180000 ms)
  timer.setInterval(180000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
  ArduinoOTA.handle();  // 👈 Required for OTA!
}