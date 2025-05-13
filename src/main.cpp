// Import required libraries
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SimpleKalmanFilter.h>
#include "index_html.h"
#include "data_config.h"
#include <EEPROM.h>
#include "icon.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SH110X.h>
#include "mybutton.h"
#include "Arduino.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "BANLINHKIEN"
#define BLYNK_TEMPLATE_NAME "BANLINHKIEN"
#include <BlynkSimpleEsp32.h>

// AWS IoT Core Configuration
#define AWS_IOT_ENDPOINT "a10h9ylpaegz2k-ats.iot.us-east-1.amazonaws.com"
#define AWS_IOT_PORT 8883
#define AWS_IOT_PUBLISH_TOPIC "esp32/data"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/control"
#define THING_NAME "ESP32_PlantMonitor"  // Use the name you created

// AWS root CA certificate
const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

// Device certificate
const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUfJMb58Mc3+KzckKW788WBkrhF60wDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI1MDUxMzExMzky
MloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMiomSgo84fMMSaOdVlf
vQ7upExjP3H758EjSqpcgR8Flx0HoGaJpD4PPDhJpyu9Mmuz1UvKKo7tby9Y3/re
6Yg2n36AGKQHlDqVb1Yp9rlNgPLtdmh/pr5p/J8Y0KoSMC8hH6eqyvrQ4pm7iOms
gb9ymU8XLIlPhWKlV9YJBV0UZez3cy3N5gxLmXEBs3MWf+LNSlyTkmEFiSoyOa6W
eiQ4rPLdh/PwgKi4VSCj96v/6cJCZsWkAD/R/hnUS2DbcuN8CIDQPmYCdqmSlIza
OtUsqbWHShIFCKazkRNbtNWaZwlxa6XeSYe/Rf9qXz4CjbkhFnc3UteQdSO102JB
eAUCAwEAAaNgMF4wHwYDVR0jBBgwFoAURqSDMV6tn2z+B1gXwdiiDhjqzkMwHQYD
VR0OBBYEFNhAFGdxzZ8xQDAGLEC5TvfoSklEMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCJaiTtwJ8bfAVcylYB7i6rcm7e
cfavq1lEyjIR3hpJKW1I+htqqvkWBc6TcctLMuN8tv+MKxMQJoi0EMIQiG3oNlrL
3Kvc89kEKddaZ12JKVAcbQ+wG1eDkgQuQwmcgRvLybjX+BXDVfCm80rosnIO/gul
3sz7XotNjq06YBtULXRR/hWyRV5QvFMNfpYcVWoWFWMHQE6IP4a4Iv+6m+qA87ei
ktb7cM/fUFwpptmvmeADYujzEoOTIASlgV5MyPESefWYmY7yFTvXPs1W+nSlrx3B
rGQGVBa2PLuSgkFaj7bEJYSR2BdNpx1jas0OoUyQewcdWAShSCpqUG0NjAon
-----END CERTIFICATE-----
)KEY";

// Device private key
const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAyKiZKCjzh8wxJo51WV+9Du6kTGM/cfvnwSNKqlyBHwWXHQeg
ZomkPg88OEmnK70ya7PVS8oqju1vL1jf+t7piDaffoAYpAeUOpVvVin2uU2A8u12
aH+mvmn8nxjQqhIwLyEfp6rK+tDimbuI6ayBv3KZTxcsiU+FYqVX1gkFXRRl7Pdz
Lc3mDEuZcQGzcxZ/4s1KXJOSYQWJKjI5rpZ6JDis8t2H8/CAqLhVIKP3q//pwkJm
xaQAP9H+GdRLYNty43wIgNA+ZgJ2qZKUjNo61SyptYdKEgUIprORE1u01ZpnCXFr
pd5Jh79F/2pfPgKNuSEWdzdS15B1I7XTYkF4BQIDAQABAoIBAQCcbnQYTvSsZY/b
0yvBJVYqkfkcZDNOFWiztWO8NKmlGaKZSxzNU3jruMzlxOXCv5jTRjt+/nj3PDt1
2f+jgKznOE5XPF35Yr84fM9ziPuDstxYhq/M/9dqkw4SqjHipJNT6qrW99uOddJy
rzUplwb/zcOfWR2rXodKRESZ5CZ+TMlq6U7Jj/8aUZS7KR3m1urVyxkHu4PivvWR
2JNI/6JgySqrSZrE4q0+h+Pg8/FmAyUpLY0zyqQSLwAGZ6JOnidPxw2Ty9MEaYtZ
y9oZ0aCAqQz+5qHo4DU/D+fpw5dPYdQZkwwyBBJMO6FrYzTOtvM3GogkiIbPAbNb
ZwyNqNTxAoGBAPuGSYBjdFj3BYMjf9ZzDny1Gniw9AJdIeH+i2qloMgURkj8egf3
S1K3abX+vv9dxBrXj2gG0wpuEmJTnlDxNKNDSdhwsPowcis9rATB6nce3yvE195+
gz/HRWY2k2nzDdBSwT+2hUAJQQ7VHQSYfS5OqppvcopHTFnbtco9UG4fAoGBAMw6
nOdpfdgmXc1I9+1Qk70gj6aV2soWHQ1xNSSwE+HhjR6dKLnGvorVHzrGyHpj5YfI
/ppJaM0b/V4jIOkHNYCcA/GETNk47Y1xrULm9QECjkfEIhP5T6X2eSK8qDgwzv3L
Rp7Ad7NALAOKj+S6bSHGjF/bB+2qBI+KfQUDFU1bAoGAUqQ7sDh9OOHo6L6YOUDS
iL0EdpVFhyVMhbGZ4y+KVVL8CO0Tt1xUXlgvHoIWBVesoFklIDvlyW6DEdWBlfy6
m833DGtyarHSQUzhISsJu5vkHcBuYQdWXnU94yiZZN1cM4+NOT7iJzkjLBlvkFv9
/WIRge6ORsrIVlnvMATXUWcCgYAgu6Q6UuYgpsWSCYfzOV70iSt8/7e5fCHb7bEe
2DnXklcT+Qx1+izAA7ilqwam1ywKv2RPRkTN2xaY5/h2YPlqS5c2NoY6qq0oZnfm
qS3Lo6U6S21K1kK1zvNLZgFwQCJs+w+ukhqdWEO1rZeyiU7EgxYspuZ2aTGh8Ahp
rTzU0QKBgQCFNdhksFm+x0Vab2wvUvrQ7hoihWiFy5tewqA8msUe43PpREQaFwmy
KzL0+oIEjFRthc76MSqwLEhelLmRAVtGQzLlspRWtdQ/dGF0uyGAfAlLjgFgtTpj
6eInhiLAndneH72f+Iaxa0xjfVg80rlGuBk+O3muF/0aFmUTzYiJ0A==
-----END RSA PRIVATE KEY-----
)KEY";

// Client instances for AWS IoT
WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);

// Time variables for reconnection logic
unsigned long previousConnectMillis = 0;
unsigned long previousPublishMillis = 0;
const long reconnectInterval = 5000;
const long publishInterval = 10000;

// ==== Đặt prototype các hàm ở đây ====
void TaskButton(void *pvParameters);
void TaskOLEDDisplay(void *pvParameters);
void TaskDHT11(void *pvParameters);
void TaskSoilMoistureSensor(void *pvParameters);
void TaskAutoWarning(void *pvParameters);
void TaskAWSIoT(void *pvParameters); // Replace TaskBlynk with TaskAWSIoT
void connectToAWS();
void messageHandler(char* topic, byte* payload, unsigned int length);
void publishSensorData();
void connectAPMode();
String getJsonData();
void getDataFromClient(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void writeEEPROM();
void buzzerBeep(int numberBeep);
void check_data_and_send_to_blynk(bool autoWarning, int temp, int humi, int soilMoisture);
void check_data_and_send_to_aws(bool autoWarning, int temp, int humi, int soilMoisture);
void myTimer();
void clearOLED();
void clearRectangle(int x1, int y1, int x2, int y2);
void printValueSetup();
void readEEPROM();
void clearEeprom();
void blinkLED(int numberBlink);
void button_press_short_callback(uint8_t button_id);
void button_press_long_callback(uint8_t button_id);

/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-async-web-server-espasyncwebserver-library/
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/
#define BLYNK_PRINT Serial
// You should get Auth Token in the Blynk App.
#define BLYNK_TEMPLATE_ID "BANLINHKIEN"
#define BLYNK_TEMPLATE_NAME "BANLINHKIEN"
#include <BlynkSimpleEsp32.h>
char BLYNK_AUTH_TOKEN[32]   =   "";


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//----------------------- Khai báo 1 số biến Blynk -----------------------
bool blynkConnect = true;
BlynkTimer timer; 
// Một số Macro
#define ENABLE    1
#define DISABLE   0
// ---------------------- Khai báo cho OLED 1.3 --------------------------

#define i2c_Address 0x3C //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G oled = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define OLED_SDA      21
#define OLED_SCL      22

typedef enum {
  SCREEN0,
  SCREEN1,
  SCREEN2,
  SCREEN3,
  SCREEN4,
  SCREEN5,
  SCREEN6,
  SCREEN7,
  SCREEN8,
  SCREEN9,
  SCREEN10,
  SCREEN11,
  SCREEN12,
  SCREEN13
}SCREEN;
int screenOLED = SCREEN0;

bool enableShow = DISABLE;

#define SAD    0
#define NORMAL 1
#define HAPPY  2
int warningTempState = SAD;
int warningHumiState = NORMAL;
int warningSoilMoistureState = HAPPY;

bool autoWarning = 1;
// --------------------- Cảm biến DHT11 ---------------------
#include "DHT.h"
#define DHT11_PIN         26
#define DHTTYPE DHT11
DHT dht(DHT11_PIN, DHTTYPE);
float tempValue = 30;
int humiValue   = 60;
SimpleKalmanFilter tempfilter(2, 2, 0.001);
SimpleKalmanFilter humifilter(2, 2, 0.001);
bool dht11ReadOK = true;
// ----------------------------- Khai báo cảm biến độ ẩm đất ------------------------------------
#define SOIL_MOISTURE 39
int soilMoistureValue = 0;
// Khai báo LED
#define LED           33
// Khai báo BUZZER
#define BUZZER        2
// Khai báo RELAY
#define RELAY         25

uint32_t timeCountBuzzerWarning = 0;
#define TIME_BUZZER_WARNING     300  //thời gian cảnh báo bằng còi (đơn vị giây)
//-------------------- Khai báo Button-----------------------
#include "mybutton.h"
#include "Arduino.h"
#define BUTTON_DOWN_PIN 34
#define BUTTON_UP_PIN     35
#define BUTTON_SET_PIN    32

#define BUTTON1_ID  1
#define BUTTON2_ID  2
#define BUTTON3_ID  3
Button buttonSET;
Button buttonDOWN;
Button buttonUP;
void button_press_short_callback(uint8_t button_id);
void button_press_long_callback(uint8_t button_id);
//------------------------------------------------------------
TaskHandle_t TaskButton_handle      = NULL;
TaskHandle_t TaskOLEDDisplay_handle = NULL;
TaskHandle_t TaskDHT11_handle = NULL;
TaskHandle_t TaskSoilMoistureSensor_handle = NULL;
TaskHandle_t TaskAutoWarning_handle = NULL;

// Forward declaration for readEEPROM
void readEEPROM();
// Forward declarations for functions used in setup
void connectSTA();
void TaskAutoWarning(void *pvParameters);

// XÓA hoặc COMMENT các dòng này nếu đã có trong data_config.h
// String Essid = "";
// String Epass = "";
// String Etoken = "";
// int EtypePlant = 0;
// int EtempThreshold1 = 0;
// int EtempThreshold2 = 0;
// int EhumiThreshold1 = 0;
// int EhumiThreshold2 = 0;
// int EsoilMoistureThreshold1 = 0;
// int EsoilMoistureThreshold2 = 0;
// const char* ssidAP = "ESP32_IOT";
// const char* passwordAP = "12345678";
// String snTemp[] = {...};
// String snHumi[] = {...};
// String snSoilMoisture[] = {...};

void setup() {
  Serial.begin(115200);
  // Đọc data setup từ eeprom
  EEPROM.begin(512);
  readEEPROM();
  
  // Khởi tạo LED
  pinMode(LED, OUTPUT);
  // Khởi tạo RELAY
  pinMode(RELAY, OUTPUT);
  // Khởi tạo BUZZER
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, DISABLE);
  // Khởi tạo OLED
  oled.begin(i2c_Address, true);
  oled.setTextSize(2);
  oled.setTextColor(SH110X_WHITE);
  // Khởi tạo DHT11
  dht.begin();
  // ---------- Đọc giá trị AutoWarning trong EEPROM ----------------
  autoWarning = EEPROM.read(210);
  // Khởi tạo nút nhấn
  pinMode(BUTTON_SET_PIN, INPUT_PULLUP);
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  button_init(&buttonSET, BUTTON_SET_PIN, BUTTON1_ID);
  button_init(&buttonUP, BUTTON_UP_PIN, BUTTON2_ID);
  button_init(&buttonDOWN,   BUTTON_DOWN_PIN,   BUTTON3_ID);
  button_pressshort_set_callback((void *)button_press_short_callback);
  button_presslong_set_callback((void *)button_press_long_callback);

  xTaskCreatePinnedToCore(TaskButton, "TaskButton", 1024 * 10, NULL, 20, &TaskButton_handle, 1);
  xTaskCreatePinnedToCore(TaskOLEDDisplay, "TaskOLEDDisplay", 1024 * 16, NULL, 20, &TaskOLEDDisplay_handle, 1);
  xTaskCreatePinnedToCore(TaskDHT11, "TaskDHT11", 1024 * 10, NULL, 10, &TaskDHT11_handle, 1);
  xTaskCreatePinnedToCore(TaskSoilMoistureSensor, "TaskSoilMoistureSensor", 1024 * 10, NULL, 10, &TaskSoilMoistureSensor_handle, 1);
  xTaskCreatePinnedToCore(TaskAutoWarning, "TaskAutoWarning", 1024 * 10, NULL, 10, &TaskAutoWarning_handle, 1);

  // Kết nối wifi
  connectSTA();
}

void loop() {
  vTaskDelete(NULL);
}

//--------------------Task đo DHT11 ---------------
void TaskDHT11(void *pvParameters) { 
    //delay(10000);
    while(1) {
      int humi =  dht.readHumidity();
      float temp =  dht.readTemperature();
      if (isnan(humi) || isnan(temp) ) {
          Serial.println(F("Failed to read from DHT sensor!"));
          dht11ReadOK = false;
      }
      else if(humi <= 100 && temp < 100) {
          dht11ReadOK = true;
          // humiValue = humifilter.updateEstimate(humi);
          // tempValue = tempfilter.updateEstimate(temp);
          humiValue = humi;
          tempValue = temp;

          Serial.print(F("Humidity: "));
          Serial.print(humiValue);
          Serial.print(F("%  Temperature: "));
          Serial.print(tempValue);
          Serial.print(F("°C "));
          Serial.println();

          if(tempValue < EtempThreshold1)
            warningTempState = SAD;
          else if(tempValue > EtempThreshold2)
            warningTempState = HAPPY;
          else
            warningTempState = NORMAL;

          if(humiValue < EhumiThreshold1 || tempValue > EhumiThreshold2) 
            warningHumiState = NORMAL;
          else
            warningHumiState = HAPPY;
      }
      delay(3000);
    }
}

//---------------- Task đo cảm biến bụi ----------
void TaskSoilMoistureSensor(void *pvParameters) {
    while(1) {
      soilMoistureValue = analogRead(SOIL_MOISTURE);
      soilMoistureValue = 4095 - soilMoistureValue;
      soilMoistureValue = map(soilMoistureValue, 0, 4095, 0, 100);
      Serial.print("soilMoistureValue = ");
      Serial.print(soilMoistureValue);
      Serial.println(" %");
      if(soilMoistureValue < EsoilMoistureThreshold1 || soilMoistureValue > EsoilMoistureThreshold2) 
        warningSoilMoistureState = NORMAL;
      else
        warningSoilMoistureState = HAPPY;

      if(soilMoistureValue < EsoilMoistureThreshold1 - 10)
         digitalWrite(RELAY, ENABLE);
      else if(soilMoistureValue > EsoilMoistureThreshold1 + 10)
         digitalWrite(RELAY, DISABLE);
      delay(1000);
    }
}

// Xóa 1 ô hình chữ nhật từ tọa độ (x1,y1) đến (x2,y2)
void clearRectangle(int x1, int y1, int x2, int y2) {
   for(int i = y1; i < y2; i++) {
     oled.drawLine(x1, i, x2, i, 0);
   }
}

void clearOLED(){
  oled.clearDisplay();
  oled.display();
}

int countSCREEN9 = 0;
// Task hiển thị OLED
void TaskOLEDDisplay(void *pvParameters) {
  while (1) {
      switch(screenOLED) {
        case SCREEN0: // Hiệu ứng khởi động
          for(int j = 0; j < 3; j++) {
            for(int i = 0; i < FRAME_COUNT_loadingOLED; i++) {
              oled.clearDisplay();
              oled.drawBitmap(32, 0, loadingOLED[i], FRAME_WIDTH_64, FRAME_HEIGHT_64, 1);
              oled.display();
              delay(FRAME_DELAY/4);
            }
          }
          screenOLED = SCREEN4;
          break;
        case SCREEN1:   // Hiển thị nhiệt độ 
          for(int j = 0; j < 2 && enableShow == ENABLE; j++) {

            oled.clearDisplay();
            oled.setTextSize(1);
            oled.setCursor(0, 20);
            oled.print("Nhiet do : ");
            oled.setTextSize(2);
            oled.setCursor(0, 32);
            //if(dht11ReadOK == true)
              oled.print(tempValue,1); 
            //else
            //  oled.print("NaN"); 
            oled.drawCircle(54, 32, 3,SH110X_WHITE); 
            oled.print(" C"); 
          
            for(int i = 0; i < FRAME_COUNT_face1OLED && enableShow == ENABLE; i++) {
                  clearRectangle(96, 0, 128, 64);
                  if(warningTempState == SAD)
                    oled.drawBitmap(96, 20, face1OLED[i], 32, 32, 1);
                  else if(warningTempState == NORMAL)
                    oled.drawBitmap(96, 20, face2OLED[i], 32, 32, 1);
                  else if(warningTempState == HAPPY)
                    oled.drawBitmap(96, 20, face3OLED[i], 32, 32, 1);
                  oled.display();
                  delay(FRAME_DELAY);
            }
            oled.display();
            delay(100);
          }
          if( enableShow == ENABLE)
            screenOLED = SCREEN2;
          break;
        case SCREEN2:   // Hiển thị độ ẩm
          for(int j = 0; j < 2 && enableShow == ENABLE; j++) {
            oled.clearDisplay();
            oled.setTextSize(1);
            oled.setCursor(0, 20);
            oled.print("Do am khong khi: ");
            oled.setTextSize(2);
            oled.setCursor(0, 32);
            //if(dht11ReadOK == true)
              oled.print(humiValue); 
            //else
            //  oled.print("NaN");
            oled.print(" %"); 
            
            for(int i = 0; i < FRAME_COUNT_face1OLED && enableShow == ENABLE; i++) {
                  clearRectangle(96, 0, 128, 64);
                  if(warningHumiState == SAD)
                    oled.drawBitmap(96, 20, face1OLED[i], 32, 32, 1);
                  else if(warningHumiState == NORMAL)
                    oled.drawBitmap(96, 20, face2OLED[i], 32, 32, 1);
                  else if(warningHumiState == HAPPY)
                    oled.drawBitmap(96, 20, face3OLED[i], 32, 32, 1);
                  oled.display();
                  delay(FRAME_DELAY);
            }
            oled.display();
            delay(100);
          }
          if( enableShow == ENABLE)
            screenOLED = SCREEN3;
          break;
        case SCREEN3:  // Hiển thị độ ẩm đất
          for(int j = 0; j < 2 && enableShow == ENABLE; j++) {
            oled.clearDisplay();
            oled.setTextSize(1);
            oled.setCursor(0, 20);
            oled.print("Do am dat: ");
            oled.setTextSize(2);
            oled.setCursor(0, 32);
            oled.print(soilMoistureValue); 
            oled.print(" %");  

            for(int i = 0; i < FRAME_COUNT_face1OLED && enableShow == ENABLE; i++) {
              clearRectangle(96, 0, 128, 64);
              if(warningSoilMoistureState == SAD)
                oled.drawBitmap(96, 20, face1OLED[i], 32, 32, 1);
              else if(warningSoilMoistureState == NORMAL)
                oled.drawBitmap(96, 20, face2OLED[i], 32, 32, 1);
              else if(warningSoilMoistureState == HAPPY)
                oled.drawBitmap(96, 20, face3OLED[i], 32, 32, 1);
              oled.display();
              delay(FRAME_DELAY);  
            }
            oled.display();
            delay(100);
          }
          if( enableShow == ENABLE)
            screenOLED = SCREEN1;
          break; 
        case SCREEN4:    // Đang kết nối Wifi
          oled.clearDisplay();
          oled.setTextSize(1);
          oled.setCursor(40, 5);
          oled.print("WIFI");
          oled.setTextSize(1.5);
          oled.setCursor(40, 17);
          oled.print("Dang ket noi..");
      
          for(int i = 0; i < FRAME_COUNT_wifiOLED; i++) {
            clearRectangle(0, 0, 32, 32);
            oled.drawBitmap(0, 0, wifiOLED[i], FRAME_WIDTH_32, FRAME_HEIGHT_32, 1);
            oled.display();
            delay(FRAME_DELAY);
          }
          break;
        case SCREEN5:    // Kết nối wifi thất bại
            oled.clearDisplay();
            oled.setTextSize(1);
            oled.setCursor(40, 5);
            oled.print("WIFI");
            oled.setTextSize(1.5);
            oled.setCursor(40, 17);
            oled.print("Mat ket noi.");
            oled.drawBitmap(0, 0, wifiOLED[FRAME_COUNT_wifiOLED - 1 ], FRAME_WIDTH_32, FRAME_HEIGHT_32, 1);
            oled.drawLine(31, 0 , 0, 31 , 1);
            oled.drawLine(32, 0 , 0, 32 , 1);
            oled.display();
            delay(2000);
            screenOLED = SCREEN9;
          break;
        case SCREEN6:
            oled.clearDisplay();
            oled.setTextSize(1);
            oled.setCursor(40, 5);
            oled.print("WIFI");
            oled.setTextSize(1.5);
            oled.setCursor(40, 17);
            oled.print("Da ket noi.");
            oled.drawBitmap(0, 0, wifiOLED[FRAME_COUNT_wifiOLED - 1], FRAME_WIDTH_32, FRAME_HEIGHT_32, 1);

            oled.setTextSize(1);
            oled.setCursor(40, 34);
            oled.print("AWS IoT");
            oled.setTextSize(1.5);
            oled.setCursor(40, 51);
            oled.print("Dang ket noi..");
            
            for(int i = 0; i < FRAME_COUNT_blynkOLED; i++) {
              clearRectangle(0, 32, 32, 64);
              oled.drawBitmap(0, 32, blynkOLED[i], FRAME_WIDTH_32, FRAME_HEIGHT_32, 1);
              oled.display();
              delay(FRAME_DELAY);
            }

          break;
        case SCREEN7:
            oled.clearDisplay();
            oled.setTextSize(1);
            oled.setCursor(40, 5);
            oled.print("WIFI");
            oled.setTextSize(1.5);
            oled.setCursor(40, 17);
            oled.print("Da ket noi.");
            oled.drawBitmap(0, 0, wifiOLED[FRAME_COUNT_wifiOLED - 1], FRAME_WIDTH_32, FRAME_HEIGHT_32, 1);

            oled.setTextSize(1);
            oled.setCursor(40, 34);
            oled.print("AWS IoT");
            oled.setTextSize(1.5);
            oled.setCursor(40, 51);
            oled.print("Da ket noi.");
            oled.drawBitmap(0, 32, blynkOLED[FRAME_COUNT_wifiOLED/2], FRAME_WIDTH_32, FRAME_HEIGHT_32, 1);
            oled.display();
            delay(2000);
            screenOLED = SCREEN3;
            enableShow = ENABLE;
          break;
        case SCREEN8:   // Đã kết nối Wifi, Mat kết nối Blynk
            oled.clearDisplay();
            oled.setTextSize(1);
            oled.setCursor(40, 5);
            oled.print("WIFI");
            oled.setTextSize(1.5);
            oled.setCursor(40, 17);
            oled.print("Mat ket noi.");
            oled.drawBitmap(0, 0, wifiOLED[FRAME_COUNT_wifiOLED - 1 ], FRAME_WIDTH_32, FRAME_HEIGHT_32, 1);
            oled.drawLine(31, 0 , 0, 31 , 1);
            oled.drawLine(32, 0 , 0, 32 , 1);
            oled.display();
            delay(2000);
            screenOLED = SCREEN9;
          break;
        case SCREEN9:   // Cai đặt 192.168.4.1
            oled.clearDisplay();
            oled.setTextSize(1);
            oled.setCursor(40, 5);
            oled.setTextSize(1);
            oled.print("Ket noi Wifi:");
            oled.setCursor(40, 17);
            oled.setTextSize(1);
            oled.print("ESP32_IOT");

            oled.setCursor(40, 38);
            oled.print("Dia chi IP:");
    
            oled.setCursor(40, 50);
            oled.print("192.168.4.1");

            for(int i = 0; i < FRAME_COUNT_settingOLED; i++) {
              clearRectangle(0, 0, 32, 64);
              oled.drawBitmap(0, 16, settingOLED[i], FRAME_WIDTH_32, FRAME_HEIGHT_32, 1);
              oled.display();
              delay(FRAME_DELAY*2);
            }
            countSCREEN9++;
            if(countSCREEN9 > 10) {
              countSCREEN9 = 0;
              screenOLED = SCREEN1;
              enableShow = ENABLE;
            }

            break;
          case SCREEN10:    // auto : on
            oled.clearDisplay();
            oled.setTextSize(1);
            oled.setCursor(40, 20);
            oled.print("Canh bao:");
            oled.setTextSize(2);
            oled.setCursor(40, 32);
            oled.print("DISABLE"); 
            for(int i = 0; i < FRAME_COUNT_autoOnOLED; i++) {
              clearRectangle(0, 0, 32, 64);
              oled.drawBitmap(0, 16, autoOnOLED[i], FRAME_WIDTH_32, FRAME_HEIGHT_32, 1);
              oled.display();
              delay(FRAME_DELAY);
            }
            clearRectangle(40, 32, 128, 64);
            oled.setCursor(40, 32);
            oled.print("ENABLE"); 
            oled.display();   
            delay(2000);
            screenOLED = SCREEN1;
            enableShow = ENABLE;
            break;
          case SCREEN11:     // auto : off
            oled.clearDisplay();
            oled.setTextSize(1);
            oled.setCursor(40, 20);
            oled.print("Canh bao:");
            oled.setTextSize(2);
            oled.setCursor(40, 32);
            oled.print("ENABLE");
            for(int i = 0; i < FRAME_COUNT_autoOffOLED; i++) {
              clearRectangle(0, 0, 32, 64);
              oled.drawBitmap(0, 16, autoOffOLED[i], FRAME_WIDTH_32, FRAME_HEIGHT_32, 1);
              oled.display();
              delay(FRAME_DELAY);
            }
            clearRectangle(40, 32, 128, 64);
            oled.setCursor(40, 32);
            oled.print("DISABLE"); 
            oled.display();    
            delay(2000);
            screenOLED = SCREEN1;  
            enableShow = ENABLE;
            break;
          case SCREEN12:  // gui du lieu len blynk
            oled.clearDisplay();
            oled.setTextSize(1);
            oled.setCursor(40, 20);
            oled.print("Gui du lieu");
            oled.setCursor(40, 32);
            oled.print("den AWS IoT");
            for(int i = 0; i < FRAME_COUNT_sendDataOLED; i++) {
                clearRectangle(0, 0, 32, 64);
                oled.drawBitmap(0, 16, sendDataOLED[i], FRAME_WIDTH_32, FRAME_HEIGHT_32, 1);
                oled.display();
                delay(FRAME_DELAY);
            } 
            delay(1000);
            screenOLED = SCREEN1; 
            enableShow = ENABLE;
            break;
          case SCREEN13:   // khoi dong lai
            oled.clearDisplay();
            oled.setTextSize(1);
            oled.setCursor(0, 20);
            oled.print("Khoi dong lai");
            oled.setCursor(0, 32);
            oled.print("Vui long doi ..."); 
            oled.display();
            break;
          default : 
            delay(500);
            break;
      } 
      delay(10);
  }
}



//-----------------Kết nối STA wifi, chuyển sang wifi AP nếu kết nối thất bại ----------------------- 
void connectSTA() {
  delay(5000);
  enableShow = DISABLE;
  if (Essid.length() > 1) {  
    Serial.println(Essid);         // Kiểm tra SSID
    Serial.println(Epass);         // Kiểm tra Password
    WiFi.begin(Essid.c_str(), Epass.c_str());
    int countConnect = 0;
    
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);   
      if (countConnect++ == 15) {
        // Wifi connection failed
        screenOLED = SCREEN5;
        digitalWrite(BUZZER, ENABLE);
        delay(2000);
        digitalWrite(BUZZER, DISABLE);
        delay(3000);
        break;
      }
      screenOLED = SCREEN4;
      delay(2000);
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected to WiFi!");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      
      // Show connecting to AWS screen
      screenOLED = SCREEN6;
      delay(2000);
      
      // Connect to AWS IoT Core
      connectToAWS();
      
      // Create AWS IoT task
      xTaskCreatePinnedToCore(TaskAWSIoT, "TaskAWSIoT", 1024*16, NULL, 20, NULL, 1);
      
      return;
    } else {
      // WiFi connection failed, switch to AP mode
      digitalWrite(BUZZER, ENABLE);
      delay(2000);
      digitalWrite(BUZZER, DISABLE);
      screenOLED = SCREEN9;
      connectAPMode();
    }
  } else {
    // No saved WiFi credentials, switch to AP mode
    screenOLED = SCREEN9;
    connectAPMode();
  }
}

//--------------------------- switch AP Mode --------------------------- 
void connectAPMode() {

  // Khởi tạo Wifi AP Mode, vui lòng kết nối wifi ESP32, truy cập 192.168.4.1
  WiFi.softAP(ssidAP, passwordAP);  

  // Gửi trang HTML khi client truy cập 192.168.4.1
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Gửi data ban đầu đến clientgetDataFromClient
  server.on("/data_before", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = getJsonData();
    request->send(200, "application/json", json);
  });

  // Get data từ client
  server.on("/post_data", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "SUCCESS");
    enableShow = DISABLE;
    screenOLED = SCREEN13;
    delay(5000);
    ESP.restart();
  }, NULL, getDataFromClient);

  // Start server
  server.begin();
}

//------------------- Hàm đọc data từ client gửi từ HTTP_POST "/post_data" -------------------
void getDataFromClient(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  Serial.print("get data : ");
  Serial.println((char *)data);
  
  StaticJsonDocument<512> jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, (char *)data);
  
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }
  
  if (jsonDoc.containsKey("ssid"))
    Essid = jsonDoc["ssid"].as<String>();
  if (jsonDoc.containsKey("pass"))
    Epass = jsonDoc["pass"].as<String>();
  if (jsonDoc.containsKey("token"))
    Etoken = jsonDoc["token"].as<String>();
  if (jsonDoc.containsKey("typePlant"))
    EtypePlant = jsonDoc["typePlant"].as<int>();
  if (jsonDoc.containsKey("tempThreshold1"))
    EtempThreshold1 = jsonDoc["tempThreshold1"].as<int>();
  if (jsonDoc.containsKey("tempThreshold2"))
    EtempThreshold2 = jsonDoc["tempThreshold2"].as<int>();
  if (jsonDoc.containsKey("humiThreshold1"))
    EhumiThreshold1 = jsonDoc["humiThreshold1"].as<int>();
  if (jsonDoc.containsKey("humiThreshold2"))
    EhumiThreshold2 = jsonDoc["humiThreshold2"].as<int>();
  if (jsonDoc.containsKey("soilMoistureThreshold1"))
    EsoilMoistureThreshold1 = jsonDoc["soilMoistureThreshold1"].as<int>();
  if (jsonDoc.containsKey("soilMoistureThreshold2"))
    EsoilMoistureThreshold2 = jsonDoc["soilMoistureThreshold2"].as<int>();
  
  writeEEPROM();
}

// ------------ Hàm in các giá trị cài đặt ------------
void printValueSetup() {
    Serial.print("ssid = ");
    Serial.println(Essid);
    Serial.print("pass = ");
    Serial.println(Epass);
    Serial.print("token = ");
    Serial.println(Etoken);
    Serial.print("EtypePlant = ");
    Serial.println(EtypePlant);
    Serial.print("tempThreshold1 = ");
    Serial.println(EtempThreshold1);
    Serial.print("tempThreshold2 = ");
    Serial.println(EtempThreshold2);
    Serial.print("humiThreshold1 = ");
    Serial.println(EhumiThreshold1);
    Serial.print("humiThreshold2 = ");
    Serial.println(EhumiThreshold2);
    Serial.print("soilMoistureThreshold1 = ");
    Serial.println(EsoilMoistureThreshold1);
    Serial.print("soilMoistureThreshold2 = ");
    Serial.println(EsoilMoistureThreshold2);
    Serial.print("autoWarning = ");
    Serial.println(autoWarning);
}

//-------- Hàm tạo biến JSON để gửi đi khi có request HTTP_GET "/" --------
String getJsonData() {
  StaticJsonDocument<512> jsonDoc;
  
  jsonDoc["ssid"] = Essid;
  jsonDoc["pass"] = Epass;
  jsonDoc["token"] = Etoken;
  jsonDoc["typePlant"] = EtypePlant;
  jsonDoc["tempThreshold1"] = EtempThreshold1;
  jsonDoc["tempThreshold2"] = EtempThreshold2;
  jsonDoc["humiThreshold1"] = EhumiThreshold1;
  jsonDoc["humiThreshold2"] = EhumiThreshold2;
  jsonDoc["soilMoistureThreshold1"] = EsoilMoistureThreshold1;
  jsonDoc["soilMoistureThreshold2"] = EsoilMoistureThreshold2;

  String jsonString;
  serializeJson(jsonDoc, jsonString);
  return jsonString;
}

//-------------------------------------------------------------------------------
//--------------------------------Task Blynk-------------------------------------

//----------------------------- Task auto Warning--------------------------------
void TaskAutoWarning(void *pvParameters) {
    delay(20000);
    while(1) {
      if(autoWarning == 1) {
          check_data_and_send_to_aws(ENABLE, tempValue, humiValue, soilMoistureValue);
      }
      delay(10000);
    }
}

//----------------------- Send send Data value to Blynk every 2 seconds--------
void myTimer() {
    
    Blynk.virtualWrite(V0, tempValue);  
    Blynk.virtualWrite(V1, humiValue);
    Blynk.virtualWrite(V2, soilMoistureValue);
    Blynk.virtualWrite(V4, autoWarning); 
}
//--------------Read button from BLYNK and send notification back to Blynk-----------------------
int checkAirQuality = 0;
BLYNK_WRITE(V3) {
    enableShow = DISABLE;
    checkAirQuality = param.asInt();
    if(checkAirQuality == 1) {
      buzzerBeep(1);
      check_data_and_send_to_blynk(DISABLE, tempValue, humiValue, soilMoistureValue);
      screenOLED = SCREEN12;
    } 
}

//------------------------- check autoWarning from BLYNK  -----------------------
BLYNK_WRITE(V4) {
    enableShow = DISABLE;
    autoWarning = param.asInt();
    buzzerBeep(1);
    EEPROM.write(210, autoWarning);  EEPROM.commit();
    if(autoWarning == 0) screenOLED = SCREEN11;
    else screenOLED = SCREEN10;
}

//---------------------------Task TaskSwitchAPtoSTA---------------------------
void TaskAWSIoT(void *pvParameters) {
  while (1) {
    // Maintain MQTT connection
    if (!mqttClient.connected()) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousConnectMillis >= reconnectInterval) {
        previousConnectMillis = currentMillis;
        connectToAWS();
      }
    }
    
    // Process MQTT messages
    mqttClient.loop();
    
    // Publish sensor data periodically
    unsigned long currentMillis = millis();
    if (currentMillis - previousPublishMillis >= publishInterval) {
      previousPublishMillis = currentMillis;
      publishSensorData();
    }
    
    delay(10);
  }
}

/*
 * Các hàm liên quan đến lưu dữ liệu cài đặt vào EEPROM
*/
//--------------------------- Read Eeprom  --------------------------------
void readEEPROM() {
    for (int i = 0; i < 32; ++i)       //Reading SSID
        Essid += char(EEPROM.read(i)); 
    for (int i = 32; i < 64; ++i)      //Reading Password
        Epass += char(EEPROM.read(i)); 
    for (int i = 64; i < 96; ++i)      //Reading Password
        Etoken += char(EEPROM.read(i)); 
    if(Essid.length() == 0) Essid = "BLK";

    EtypePlant      = EEPROM.read(199);

    EtempThreshold1 = EEPROM.read(200);
    EtempThreshold2 = EEPROM.read(201);

    EhumiThreshold1 = EEPROM.read(202);
    EhumiThreshold2 = EEPROM.read(203);

    EsoilMoistureThreshold1 = EEPROM.read(204) * 100 + EEPROM.read(205);
    EsoilMoistureThreshold2 = EEPROM.read(206) * 100 + EEPROM.read(207);  

    autoWarning     = EEPROM.read(210);

    printValueSetup();
}

// ------------------------ Clear Eeprom ------------------------

void clearEeprom() {
    Serial.println("Clearing Eeprom");
    for (int i = 0; i < 250; ++i) 
      EEPROM.write(i, 0);
}

// -------------------- Hàm ghi data vào EEPROM ------------------
void writeEEPROM() {
    clearEeprom();
    for (int i = 0; i < Essid.length(); ++i)
          EEPROM.write(i, Essid[i]);  
    for (int i = 0; i < Epass.length(); ++i)
          EEPROM.write(32+i, Epass[i]);
    for (int i = 0; i < Etoken.length(); ++i)
          EEPROM.write(64+i, Etoken[i]);
    EEPROM.write(199, EtypePlant);               // lưu kiểu cây trồng
    
    EEPROM.write(200, EtempThreshold1);          // lưu ngưỡng nhiệt độ 1
    EEPROM.write(201, EtempThreshold2);          // lưu ngưỡng nhiệt độ 2

    EEPROM.write(202, EhumiThreshold1);          // lưu ngưỡng độ ẩm 1
    EEPROM.write(203, EhumiThreshold2);          // lưu ngưỡng độ ẩm 2

    EEPROM.write(204, EsoilMoistureThreshold1 / 100);      // lưu hàng nghìn + trăm bụi 1
    EEPROM.write(205, EsoilMoistureThreshold1 % 100);      // lưu hàng chục + đơn vị bụi 1

    EEPROM.write(206, EsoilMoistureThreshold2 / 100);      // lưu hàng nghìn + trăm bụi 2
    EEPROM.write(207, EsoilMoistureThreshold2 % 100);      // lưu hàng chục + đơn vị bụi 2
    
    EEPROM.commit();

    Serial.println("write eeprom");
    delay(500);
}


//-----------------------Task Task Button ----------
void TaskButton(void *pvParameters) {
    while(1) {
      handle_button(&buttonSET);
      handle_button(&buttonUP);
      handle_button(&buttonDOWN);
      delay(10);
    }
}
//-----------------Hàm xử lí nút nhấn nhả ----------------------
void button_press_short_callback(uint8_t button_id) {
    switch(button_id) {
      case BUTTON1_ID :  
        buzzerBeep(1);
        Serial.println("btSET press short");
        break;
      case BUTTON2_ID :
        buzzerBeep(1);
        Serial.println("btUP press short");
        break;
      case BUTTON3_ID :
        buzzerBeep(1);
        Serial.println("btDOWN press short");
        enableShow = DISABLE;
        check_data_and_send_to_blynk(DISABLE, tempValue, humiValue, soilMoistureValue);
        screenOLED = SCREEN12;
        break;  
    } 
} 
//-----------------Hàm xử lí nút nhấn giữ ----------------------
void button_press_long_callback(uint8_t button_id) {
  switch(button_id) {
    case BUTTON1_ID :
      buzzerBeep(2);  
      enableShow = DISABLE;
      Serial.println("btSET press long");
      screenOLED = SCREEN9;
      clearOLED();
      connectAPMode(); 
      break;
    case BUTTON2_ID :
      buzzerBeep(2);
      Serial.println("btUP press short");
      break;
    case BUTTON3_ID :
      buzzerBeep(2);
      Serial.println("btDOWN press short");
      enableShow = DISABLE;
      autoWarning = 1 - autoWarning;
      EEPROM.write(210, autoWarning);  EEPROM.commit();
      Blynk.virtualWrite(V4, autoWarning); 
      if(autoWarning == 0) screenOLED = SCREEN11;
      else screenOLED = SCREEN10;
      break;  
  } 
} 
// ---------------------- Hàm điều khiển còi -----------------------------
void buzzerBeep(int numberBeep) {
  for(int i = 0; i < numberBeep; ++i) {
    digitalWrite(BUZZER, ENABLE);
    delay(100);
    digitalWrite(BUZZER, DISABLE);
    delay(100);
  }  
}
// ---------------------- Hàm điều khiển LED -----------------------------
void blinkLED(int numberBlink) {
  for(int i = 0; i < numberBlink; ++i) {
    digitalWrite(LED, DISABLE);
    delay(300);
    digitalWrite(LED, ENABLE);
    delay(300);
  }  
}

/**
 * @brief Kiểm tra và gửi lên BLYNK
 *
 * @param autoWarning auto Warning
 * @param temp Nhiệt độ hiện tại    *C
 * @param humi Độ ẩm hiện tại        %
 * @param soilMoisture độ ẩm đất hiện tại %
 */
void check_data_and_send_to_blynk(bool autoWarning, int temp, int humi, int soilMoisture) {
  String notifications = "";
  int tempIndex = 0;
  int soilMoistureIndex = 0;
  int humiIndex = 0;
  if(dht11ReadOK ==  true) {
    if(autoWarning == 0) {
      if(temp < EtempThreshold1 )tempIndex = 1;
      else if(temp >= EtempThreshold1 && temp <=  EtempThreshold2)  tempIndex = 2;
      else tempIndex = 3;
      

      if(humi < EhumiThreshold1 ) humiIndex = 1;
      else if(humi >= EhumiThreshold1 && humi <= EhumiThreshold2)   humiIndex = 2;
      else humiIndex = 3;

      if(soilMoisture < EsoilMoistureThreshold1 ) soilMoistureIndex = 1;
      else if(soilMoisture >= EsoilMoistureThreshold1 && soilMoisture <= EsoilMoistureThreshold2)   soilMoistureIndex = 2;
      else soilMoistureIndex = 3;

      
      notifications = snTemp[tempIndex] + String(temp) + "*C . " + snHumi[humiIndex] + String(humi) + "% . " + snSoilMoisture[soilMoistureIndex] + String(soilMoisture) + "% . " ;  
      
      Blynk.logEvent("check_data",notifications);
    } else {
      if(temp < EtempThreshold1 )tempIndex = 1;
      else if(temp >= EtempThreshold1 && temp <= EtempThreshold2)  tempIndex = 0;
      else tempIndex = 3;
      

      if(humi < EhumiThreshold1 ) humiIndex = 1;
      else if(humi >= EhumiThreshold1 && humi <= EhumiThreshold2)   humiIndex = 0;
      else humiIndex = 3;

      if(soilMoisture < EsoilMoistureThreshold1 ) soilMoistureIndex = 1;
      else if(soilMoisture >= EsoilMoistureThreshold1 && soilMoisture <= EsoilMoistureThreshold2)   soilMoistureIndex = 0;
      else soilMoistureIndex = 3;

      if(tempIndex == 0 && humiIndex == 0 && soilMoistureIndex == 0)
        notifications = "";
      else {
        if(tempIndex != 0) notifications = notifications + snTemp[tempIndex] + String(temp) + "*C . ";
        if(humiIndex != 0) notifications = notifications + snHumi[humiIndex] + String(humi) + "% . " ;
        if(soilMoistureIndex != 0) notifications = notifications + snSoilMoisture[soilMoistureIndex] + String(soilMoisture) + "% . " ;
        Blynk.logEvent("auto_warning",notifications);
      }
    }

    Serial.println(notifications);
  }
  
}

void check_data_and_send_to_aws(bool autoWarning, int temp, int humi, int soilMoisture) {
  String notifications = "";
  int tempIndex = 0;
  int soilMoistureIndex = 0;
  int humiIndex = 0;
  
  if (dht11ReadOK == true) {
    // Calculate index values (your existing logic)
    if(autoWarning == 0) {
      if(temp < EtempThreshold1) tempIndex = 1;
      else if(temp >= EtempThreshold1 && temp <= EtempThreshold2) tempIndex = 2;
      else tempIndex = 3;
      
      // Continue with other index calculations...
      
      notifications = snTemp[tempIndex] + String(temp) + "*C . " + 
                      snHumi[humiIndex] + String(humi) + "% . " + 
                      snSoilMoisture[soilMoistureIndex] + String(soilMoisture) + "% . ";
    } else {
      // Your existing auto warning logic...
    }
    
    // Create ArduinoJson v6.x document
    StaticJsonDocument<512> doc;
    
    doc["deviceId"] = THING_NAME;
    doc["temperature"] = temp;
    doc["humidity"] = humi;
    doc["soilMoisture"] = soilMoisture;
    
    if (autoWarning == 0) {
      doc["messageType"] = "check_data";
      doc["message"] = notifications;
    } else {
      if (tempIndex != 0 || humiIndex != 0 || soilMoistureIndex != 0) {
        doc["messageType"] = "auto_warning";
        doc["message"] = notifications;
      }
    }
    
    // Only publish if there's a message to send
    if (notifications.length() > 0) {
      char jsonBuffer[512];
      serializeJson(doc, jsonBuffer);
      mqttClient.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
      Serial.println(notifications);
    }
  }
}

// Kết nối tới AWS IoT Core
void connectToAWS() {
    wifiClient.setCACert(AWS_CERT_CA);
    wifiClient.setCertificate(AWS_CERT_CRT);
    wifiClient.setPrivateKey(AWS_CERT_PRIVATE);

    mqttClient.setServer(AWS_IOT_ENDPOINT, AWS_IOT_PORT);
    // Nếu bạn có hàm messageHandler thì thêm dòng này:
    // mqttClient.setCallback(messageHandler);

    Serial.println("Connecting to AWS IoT Core...");
    while (!mqttClient.connected()) {
        if (mqttClient.connect(THING_NAME)) {
            Serial.println("Connected to AWS IoT Core!");
            mqttClient.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
        } else {
            Serial.print("AWS IoT connection failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    while (time(nullptr) < 100000) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Time synced!");
}

// Gửi dữ liệu cảm biến lên AWS IoT Core
void publishSensorData() {
    StaticJsonDocument<256> doc;
    doc["deviceId"] = THING_NAME;
    doc["temperature"] = tempValue;
    doc["humidity"] = humiValue;
    doc["soilMoisture"] = soilMoistureValue;
    doc["autoWarning"] = autoWarning;

    char jsonBuffer[256];
    serializeJson(doc, jsonBuffer);

    if (mqttClient.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer)) {
        Serial.println("Published to AWS IoT Core:");
        Serial.println(jsonBuffer);
    } else {
        Serial.println("Failed to publish to AWS IoT Core");
    }
}

#include "data_config.h"
// ... các include khác ...