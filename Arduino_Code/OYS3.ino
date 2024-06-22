#include <WiFi.h>              //Wi-Fi ağına bağlanma, erişim noktası (access point) oluşturma, ağdan veri alma ve gönderme gibi işlemler için kullanılır.
#include <HTTPClient.h>        // HTTP protokolü üzerinden web sunucularına istek göndermek ve yanıt almak için kullanılır.
#include <ArduinoJson.h>       //JSON formatındaki verileri parse etmek ve oluşturmak için kullanılır.
#include <SPI.h>               // harici modül ile iletişim kurabilmek için veri gönderme veri alma işlemleri vs.
#include <MFRC522.h>           //MFRC522 RFID okuyucu modülünü kontrol etmek için kullanılır.
#include <Preferences.h>       // Non-Volatile Memory (kalıcı hafıza) üzerinde anahtar-değer çifti olarak veri saklamak.
#include <Wire.h>              // I2C (Inter-Integrated Circuit) protokolünü kullanarak mikrodenetleyici ile diğer cihazlar arasında iletişim kurmak.
#include <Adafruit_GFX.h>      //Adafruit grafik kütüphanesi, ekranlara grafikler ve metinler çizmek için genel bir yapı sağlar.
#include <Adafruit_SSD1306.h>  //SSD1306 sürücüsünü kullanan OLED ekranları kontrol etmek için özel bir kütüphane.
#include "led_functions.h"     //LED kontrol fonksiyonlarını içeren custom kütüphane

// touch pin define---------------------------------
#define TOUCH_PIN 32

// RFID define---------------------------------------
#define SS_PIN 5
#define RST_PIN 4
#define GREEN_LED_PIN 12
#define RED_LED_PIN 25

// Display define------------------------------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// NEÜ LOGO Bitmap
const unsigned char school_logo[] PROGMEM = {
  // 'Yuvarlak Logo, 64x63px
  0x0c, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x36, 0x06, 0x80, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x68,
  0x03, 0x00, 0x1f, 0xf0, 0x1f, 0xf8, 0x00, 0xc8, 0x80, 0x00, 0x7f, 0x00, 0x00, 0xfe, 0x00, 0x38,
  0xe0, 0x01, 0xf8, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x30, 0x03, 0xe0, 0x00, 0x00, 0x07, 0xc0, 0x00,
  0x80, 0x0f, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x07, 0x80, 0x1e, 0x00, 0x01, 0x80, 0x00, 0x78, 0x00,
  0x80, 0x3c, 0x00, 0x03, 0xc0, 0x00, 0x3c, 0x00, 0x00, 0x78, 0x00, 0x0f, 0xf0, 0x00, 0x1e, 0x00,
  0x00, 0xf0, 0x00, 0x1e, 0x78, 0x00, 0x0f, 0x00, 0x01, 0xe0, 0x00, 0x3c, 0x3c, 0x00, 0x07, 0x80,
  0x01, 0xc0, 0x00, 0xf9, 0x9f, 0x00, 0x03, 0x80, 0x03, 0x80, 0x01, 0xe3, 0xc7, 0x80, 0x01, 0xc0,
  0x07, 0x00, 0x03, 0xcf, 0xf3, 0xc0, 0x00, 0xe0, 0x0e, 0x00, 0x03, 0x9e, 0x79, 0xc0, 0x00, 0x60,
  0x0e, 0x00, 0x03, 0x3c, 0x3d, 0xc0, 0x00, 0x70, 0x1c, 0x00, 0x7f, 0x38, 0x1c, 0xfe, 0x00, 0x38,
  0x1c, 0x00, 0xff, 0x38, 0x1c, 0xff, 0x00, 0x38, 0x38, 0x01, 0xff, 0xfe, 0x7c, 0xff, 0x80, 0x18,
  0x38, 0x07, 0xc0, 0x7f, 0xfe, 0xc3, 0xe0, 0x1c, 0x30, 0x0f, 0x1e, 0x07, 0xe6, 0xf8, 0xf0, 0x0c,
  0x70, 0x3e, 0x7f, 0xe3, 0xc6, 0xfe, 0x7c, 0x0c, 0x70, 0x78, 0xff, 0xf3, 0xce, 0xff, 0x3e, 0x0e,
  0x60, 0x73, 0xe6, 0x7b, 0xce, 0xe7, 0x8e, 0x0e, 0x60, 0x73, 0xc6, 0x3f, 0xcc, 0xe3, 0xce, 0x06,
  0xe0, 0x33, 0x87, 0x87, 0xc9, 0xe1, 0xcc, 0x06, 0xe0, 0x3b, 0x83, 0xf0, 0x4f, 0xc1, 0xdc, 0x07,
  0xe0, 0x3b, 0x87, 0xfe, 0xcf, 0xe1, 0x9c, 0x07, 0xe0, 0x19, 0xcf, 0xff, 0xcf, 0xf3, 0x98, 0x07,
  0xe0, 0x1d, 0xdf, 0xf3, 0xcf, 0xfb, 0xb8, 0x07, 0xe0, 0x1d, 0xf8, 0x67, 0xfe, 0x1f, 0x38, 0x07,
  0xe0, 0x0c, 0xf8, 0xcf, 0xf0, 0x1f, 0x30, 0x07, 0xe0, 0x0e, 0x33, 0xdd, 0x87, 0xfc, 0x70, 0x07,
  0xe0, 0x0f, 0xb7, 0x99, 0xff, 0xf1, 0xf0, 0x07, 0xe0, 0x07, 0xe7, 0x3c, 0xfc, 0xc7, 0xe0, 0x07,
  0xe0, 0x01, 0xcc, 0x7e, 0x7e, 0x1f, 0x80, 0x06, 0x60, 0x01, 0xdf, 0xff, 0x3f, 0xfb, 0x80, 0x06,
  0x60, 0x03, 0x9f, 0xff, 0xbf, 0xf9, 0xc0, 0x06, 0x70, 0x03, 0xbf, 0xf9, 0x9f, 0xf9, 0xc0, 0x0e,
  0x70, 0x03, 0xb8, 0x33, 0x9c, 0x1d, 0xc0, 0x0e, 0x30, 0x03, 0x98, 0x39, 0x9c, 0x39, 0x80, 0x0c,
  0x38, 0x01, 0xdc, 0x3c, 0xfc, 0x3b, 0x80, 0x1c, 0x38, 0x01, 0xdc, 0x1e, 0x78, 0x3b, 0x80, 0x18,
  0x1c, 0x01, 0xcf, 0xff, 0x3f, 0xf3, 0x00, 0x38, 0x1c, 0x00, 0xef, 0xfb, 0xbf, 0xf7, 0x00, 0x39,
  0x8e, 0x00, 0xef, 0xf3, 0x9f, 0xf7, 0x00, 0x71, 0xce, 0x00, 0xe0, 0x03, 0xc0, 0x06, 0x00, 0x73,
  0xc7, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x00, 0xe3, 0xe3, 0x80, 0x7f, 0xfe, 0x7f, 0xfe, 0x01, 0xc7,
  0xe3, 0x80, 0x3f, 0xfc, 0x3f, 0xfc, 0x03, 0xc7, 0xf1, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0x8f,
  0xf8, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 0x1f, 0xfc, 0x70, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x3f,
  0xfc, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x7f, 0xfe, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x78, 0x7f,
  0xff, 0x0f, 0x00, 0x00, 0x00, 0x01, 0xf0, 0xff, 0xff, 0xc3, 0xc0, 0x00, 0x00, 0x03, 0xc3, 0xff,
  0xff, 0xe1, 0xf0, 0x00, 0x00, 0x0f, 0x87, 0xff, 0xff, 0xf0, 0x7e, 0x00, 0x00, 0x7e, 0x0f, 0xff,
  0xff, 0xfc, 0x1f, 0xe0, 0x07, 0xf8, 0x3f, 0xff, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xe0, 0x7f, 0xff,
  0xff, 0xff, 0x80, 0xff, 0xff, 0x01, 0xff, 0xff
};


// Custom Library
led_functions custom_led_lib_green(GREEN_LED_PIN);
led_functions custom_led_lib_red(RED_LED_PIN);

// Non-Volatile Memory ------------------------------
Preferences preferences;
String ssid = "";
String password = "";
String classID = "";
// send data json --------------------------------------------------
String JsonData = "";

// Touch PIN State-----------------------------------------------------
int touchPINState = -1;
int touchValue = -1;
const int threshold = 25;



// RFID control ----------------------------------------------------
int readsuccess;
char str[32] = "";
String UID_Result = "";
MFRC522 mfrc522(SS_PIN, RST_PIN);

//HTTP send-get control --------------------------------------------
const char* sendDataToServerIPAdress = "http://192.168.148.84:9000/RFID/ID";
const char* sendSaveDataToServerIPAdress = "http://192.168.148.84:9000/RFID/ID-Save";
// const char* getDataToServerIPAdress = "http://172.18.81.118:9000/RFID/info";


void touchPINBegin() {
  pinMode(TOUCH_PIN, INPUT);
}

int touchPINSTATE() {
  touchValue = touchRead(TOUCH_PIN);
  Serial.println(touchRead(T9));
  if (touchValue < threshold) {
    custom_led_lib_green.power_on_led();
  } else {
    custom_led_lib_green.power_off_led();
  }
  return touchValue;
}


// RFID read_______________________________________________________________________
int readRFID() {
  if (!mfrc522.PICC_IsNewCardPresent()) {  //Yeni bir kart mı?
    return 0;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {  //id okundu mu?
    return 0;
  }

  byteArray_to_string(mfrc522.uid.uidByte, mfrc522.uid.size, str);
  UID_Result = str;

  mfrc522.PICC_HaltA();       //kartın çalışmasını durdur
  mfrc522.PCD_StopCrypto1();  //kriptololamayı durdur.

  return 1;
}

void byteArray_to_string(byte array[], unsigned int len, char buffer[]) {  //byte olarak okunan kart id'sini string bir değere döndürüyor.
  for (unsigned int i = 0; i < len; i++) {

    Serial.println(array[i]);
    byte nib1 = (array[i] >> 4) & 0x0F;                              // İlk yarıbayt
    byte nib2 = (array[i] >> 0) & 0x0F;                              // İkinci yarıbayt
    buffer[i * 2 + 0] = nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;  // İlk yarıbaytı ASCII'ye dönüştür
    buffer[i * 2 + 1] = nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;  // İkinci yarıbaytı ASCII'ye dönüştür
  }
  buffer[len * 2] = '\0';  // Stringi sonlandır
}
//________________________________________________________________________________

// HTTP control___________________________________________________________________
void sendDataToServer(const char* url, const char* contentType) {
  StaticJsonDocument<50> doc;
  StaticJsonDocument<300> responseDoc;
  doc["userID"] = UID_Result;
  doc["classID"] = classID;
  serializeJson(doc, JsonData);
  Serial.print("Sending JSON data: ");
  Serial.println(JsonData);

  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", contentType);
  int httpResponseCode = http.POST(JsonData);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    DeserializationError error = deserializeJson(responseDoc, response);

    Serial.println(response);
    if (responseDoc["message"] == "success") {
      custom_led_lib_green.power_time_led(500);
      custom_led_lib_green.power_time_led(500);
      custom_led_lib_green.power_time_led(500);
      Serial.println("Turn on LED");
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);

      if (responseDoc["lessonState"] == "on") {

        if (responseDoc["studentState"] == "off") {
          display.setCursor(0, 0);
          display.print(responseDoc["msg"].as<String>());
          display.display();
          delay(3000);
        } else {
          display.setCursor(15, 25);
          display.print(responseDoc["ogrenci_no"].as<String>() + "\n" + responseDoc["ad"].as<String>() + "\n" + responseDoc["soyad"].as<String>());
          display.display();
          delay(3000);
        }

      } else if (responseDoc["lessonState"] == "off") {
        display.setCursor(20, 25);
        display.print(responseDoc["msg"].as<String>());
        display.display();
        delay(3000);
      }
      display.clearDisplay();
      display.drawBitmap(32, 0, school_logo, 64, 64, SSD1306_WHITE);
      display.display();
    } else {
      Serial.println("Response from server is not 'success'");
    }
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
    showError();
    custom_led_lib_red.power_time_led(500);
    custom_led_lib_red.power_time_led(500);
    custom_led_lib_red.power_time_led(500);
  }
  http.end();
  display.clearDisplay();
  display.drawBitmap(32, 0, school_logo, 64, 64, SSD1306_WHITE);
  display.display();
}

// HTTP control save update ______________________________________________________________
void sendSaveDataToServer(const char* url, const char* contentType) {
  StaticJsonDocument<50> doc;
  StaticJsonDocument<300> responseDoc;
  doc["userID"] = UID_Result;
  // doc["classID"] = classID;
  serializeJson(doc, JsonData);
  Serial.print("Sending JSON data: ");
  Serial.println(JsonData);

  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", contentType);
  int httpResponseCode = http.POST(JsonData);

  if (httpResponseCode > 0) {
    custom_led_lib_green.power_time_led(500);
    custom_led_lib_green.power_time_led(500);
    custom_led_lib_green.power_time_led(500);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    DeserializationError error = deserializeJson(responseDoc, response);
    Serial.println(response);
  }
  http.end();
  display.clearDisplay();
  display.drawBitmap(32, 0, school_logo, 64, 64, SSD1306_WHITE);
  display.display();
}


// error Show______________________________________________________________
void showError() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 8);
  display.print(F("Sunucu ile"));
  display.setCursor(0, 30);
  display.print(F("iletisim  kurulamadi"));
  display.display();
}
//________________________________________________________________________________




// OLED screen start______________________________________________________________
void initDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
  display.drawBitmap(34, 0, school_logo, 64, 64, SSD1306_WHITE);
  display.display();
}
//________________________________________________________________________________

// Non-Volatile Memory start and read_____________________________________________
void initPreferences() {
  preferences.begin("oys", false);
  classID = preferences.getString("classID", "undefined");
  ssid = preferences.getString("ssid", "undefined");
  password = preferences.getString("password", "undefined");
  Serial.println(ssid);
  Serial.println(password);
}
//________________________________________________________________________________

// RFID start_____________________________________________________________________
void initRFID() {
  SPI.begin();         // SPI başlatılıyor
  mfrc522.PCD_Init();  // MFRC522 modülü başlatılıyor
  delay(1000);
}
//________________________________________________________________________________

// WIFI start and connect___________________________________________________________________
void connectToWiFi() {
  custom_led_lib_red.power_on_led();
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the Wifi network");
  custom_led_lib_red.power_off_led();
}
//________________________________________________________________________________


void setup() {
  Serial.begin(115200);
  // Kalıcı Bellek Başlatma Okuma
  initPreferences();
  // Wifi başlatma ve bağlanma
  connectToWiFi();
  // RFID başlat
  initRFID();
  // OLED ekran başlatma
  initDisplay();
  // Dokunmatik pin başlat
  touchPINBegin();
}

void loop() {
  // Serial.println("Touch State: " + touchPINState);

  if (WiFi.status() == WL_CONNECTED) {
    custom_led_lib_red.power_off_led();
    readsuccess = readRFID();
    touchPINState = touchPINSTATE();
    if (readsuccess) {
      Serial.println();
      Serial.print("UID : ");
      Serial.println(UID_Result);
      delay(500);
      // getDataToServer(getDataToServerIPAdress);
      touchValue = touchPINSTATE();
      Serial.print(touchValue);
      if (touchValue < threshold) {
        sendSaveDataToServer(sendSaveDataToServerIPAdress, "application/json");
      } else {
        sendDataToServer(sendDataToServerIPAdress, "application/json");
      }
    } else {
      Serial.println("KART OKUTUNUZ");
    }
  } else {
    custom_led_lib_red.power_on_led();
    Serial.println("İnternet bağlantısı yok.");
    connectToWiFi();
  }
  delay(1000);
  JsonData = "";
}
