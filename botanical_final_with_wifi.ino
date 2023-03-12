#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

#include "plant_database.h"


#define LED 2 // the blue LED pin
#define LIGHTSENSORPIN 39 //Ambient light sensor reading
#define SOILPIN 36  //Soil moisture sensor reading
#define WATERPOWER 17  //Water level sensor VCC
#define WATERPIN 34  //Water level sensor reading
#define BUTTON1 35
#define BUTTON2 32
#define BUTTON3 15
#define PUMP 5    // to turn pump on

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Create BME280 object
Adafruit_BME280 bme; // I2C


static const unsigned char PROGMEM BOTanical_logo[] = { // bitmap variable
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 
  0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 
  0xfc, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x93, 
  0xf8, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x13, 
  0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x33, 
  0xf1, 0xe0, 0x01, 0xff, 0x00, 0xff, 0x00, 0x00, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x33, 
  0xe3, 0x80, 0x00, 0x7e, 0x00, 0x7c, 0x00, 0x00, 0x47, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0xfe, 0x33, 
  0xe2, 0x03, 0xf0, 0x7c, 0x38, 0x38, 0x38, 0x7f, 0xc7, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xfe, 0x73, 
  0xc4, 0x3f, 0xfc, 0x38, 0x7e, 0x18, 0xf8, 0xff, 0xc7, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xfe, 0x63, 
  0xc6, 0xf3, 0xfe, 0x30, 0xfe, 0x19, 0xf8, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x67, 
  0xc7, 0xe3, 0xfc, 0x31, 0xff, 0x19, 0xf8, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x67, 
  0xc7, 0xe3, 0xfc, 0x61, 0xff, 0x18, 0xf0, 0xff, 0xc7, 0xc3, 0xe7, 0x1c, 0x78, 0x7c, 0x3c, 0x4f, 
  0xcf, 0xc7, 0xf8, 0x63, 0xff, 0x1d, 0xf0, 0xff, 0xc7, 0x80, 0xc4, 0x1c, 0x70, 0x38, 0x0c, 0x4f, 
  0xcf, 0xc7, 0x01, 0xe3, 0xff, 0x1f, 0xf1, 0xff, 0xc7, 0x19, 0xc4, 0x18, 0xe3, 0x31, 0x8c, 0x1f, 
  0xcf, 0xc6, 0x07, 0xe3, 0xff, 0x1f, 0xf1, 0xff, 0xc6, 0x31, 0xc9, 0x18, 0xe7, 0x23, 0x9c, 0x1f, 
  0xcf, 0xc7, 0x03, 0xe3, 0xff, 0x1f, 0xf1, 0xff, 0xc6, 0x31, 0xc3, 0x38, 0xc7, 0x63, 0x1c, 0x3f, 
  0xcf, 0x87, 0xe1, 0xe3, 0xff, 0x1f, 0xf1, 0xff, 0xc4, 0x71, 0xc3, 0x38, 0xc7, 0xe3, 0x1c, 0x7f, 
  0xcf, 0x8f, 0xf1, 0xe3, 0xfe, 0x3f, 0xe1, 0xff, 0xcc, 0x71, 0x86, 0x38, 0xc7, 0xe7, 0x1c, 0x7f, 
  0xcf, 0x8f, 0xf1, 0xe3, 0xfe, 0x3f, 0xe1, 0xff, 0x8c, 0x61, 0x06, 0x30, 0x83, 0x87, 0x18, 0x77, 
  0xcf, 0x8f, 0xf1, 0xe1, 0xfc, 0x7f, 0xe3, 0xff, 0x8e, 0x00, 0x06, 0x00, 0x00, 0x00, 0x02, 0x07, 
  0xcf, 0x8f, 0xe1, 0xf0, 0xf8, 0x7f, 0xe3, 0xff, 0x1e, 0x08, 0x4f, 0x0c, 0x30, 0x30, 0x87, 0x07, 
  0xcf, 0x80, 0x03, 0xf0, 0x00, 0xff, 0xe3, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xcf, 0x80, 0x0f, 0xf8, 0x01, 0xff, 0xe3, 0xfc, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xcf, 0x80, 0x3f, 0xfe, 0x0f, 0xff, 0xe7, 0xf8, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xcf, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

// variables to store readings
float light_reading = 0;
float moist_reading = 0;
float temp_reading = 0;
float press_reading = 0;
float height_reading = 0;
float humid_reading = 0;
float water_level = 0;

// variables for calculation
int cycle_length = 5;//4*24; // 24 * 4*15min delay = 1 day cycle
int cur_unit = 0;      // for determining when to take readings
int cur_cycle = 1;     // for the set of readings
int unit_time = 100;   // 0.1s for cycles (good input responsitivity)
int sensor_sleep = 5000;//15*60*1000;  // 15 minutes -> ms
float unit_length = sensor_sleep / unit_time; // when to read? = 50
unsigned long previous_millis = 2000; // 2s of setup
int error_cycle = 0;

float moist_air = 3000; // initial reading of soil ~2300 when taking pics
float moist_water = 1250;
float exp_light[2] = {1000, 2000}; // expected light over a cycle
float exp_moist[2] = {2500, 2300}; // threshold for moisture
float exp_temp[2] = {15, 25}; // average expected temperature
float exp_humid[2] = {40, 60}; // average expected humidity
float water_min = 1200; // minimum water that should be in container
float avg_light = 0;
float avg_moist = 0;
float avg_temp = 0;
float avg_humid = 0;
float past_light = exp_light[0]; // so its not on at start
float past_moist = exp_moist[0];
float past_temp = exp_temp[0];
float past_humid = exp_humid[0];
int bad_light = 0;
int bad_moist = 0;
int bad_temp = 0;
int bad_humid = 0;

int selected_plant = 0;
int cur_plant = -1;
int already_watered = LOW;
int pump_on = 0;

// variables for menu
int menu_sizes[] = {6,1,5,1,2,plant_len+1,1,1,1,1,1,2,2};
int menu_pointer = 0;
int cur_state = 0;
int next_state = 0;
int menu_len = menu_sizes[cur_state];
int pointer_blink = 0;

// variables for the buttons
int last1 = HIGH;
int cur1 = HIGH;
int last2 = HIGH;
int cur2 = HIGH;
int last3 = HIGH;
int cur3 = HIGH;

// SSID and password of Wifi connection:
const char* ssid = "";
const char* password = "";

// Initialization of webserver and websocket
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void setup() {  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Establishing connection to WiFi with SSID: " + String(ssid));
 
  while (WiFi.status() != WL_CONNECTED) {             // wait until WiFi is connected
    delay(1000);
    Serial.print(".");
  }
  Serial.print("Connected to network with IP address: ");
  Serial.println(WiFi.localIP());  

  pinMode(LED, OUTPUT);   // configure pin as an OUTPUT
  pinMode(LIGHTSENSORPIN, INPUT);
  pinMode(SOILPIN, INPUT);
  pinMode(WATERPOWER, OUTPUT);
  pinMode(WATERPIN, INPUT);
  pinMode(PUMP, OUTPUT);

  pinMode(BUTTON1, INPUT_PULLUP); // button 1 as internal pull-up resistor
  pinMode(BUTTON2, INPUT_PULLUP); // button 2 as internal pull-up resistor
  pinMode(BUTTON3, INPUT_PULLUP); // button 3 as internal pull-up resistor

  // following for oled
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // change from Adafruit splash screen to BOTanical
  display.clearDisplay();
  display.drawBitmap(0, 0, BOTanical_logo, 128, 64, 1);
  display.display();
  delay(1000);  //  delay(1000 - millis());
  display.clearDisplay(); // needs to be here or else crash

  // initiate bme280
  bool status = bme.begin(0x77);
  if (!status){//bme.begin(0x77)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  delay(2000);  //  delay(2000 - millis()); // Pause for total 2 seconds, let OLED and bme boot up

  // Clear the buffer
  display.clearDisplay();

  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS could not initialize");
  }
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {    // define here wat the webserver needs to do
    request->send(SPIFFS, "/index.html", "text/html");           
  });

  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "File not found");
  });

  server.serveStatic("/", SPIFFS, "/");
  
  webSocket.begin();                                  // start websocket
  webSocket.onEvent(webSocketEvent);                  // define a callback function -> what does the ESP32 need to do when an event from the websocket is received? -> run function "webSocketEvent()"

  server.begin();
}

void loop() { // put your main code here, to run repeatedly:
  time_t now;
  char strftime_buf[64];
  struct tm timeinfo;

  time(&now);
  // Set time start to 00:00:00 Jan 1st, 1970
  setenv("TZ", "UTC-0", 1);
  tzset();
  localtime_r(&now, &timeinfo);
  strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);

  // buttons will be LOW(0) or HIGH(1)
  int cur1 = digitalRead(BUTTON1); 
  cur2 = digitalRead(BUTTON2);
  cur3 = digitalRead(BUTTON3);

  if(last1 == LOW && cur1 == HIGH) {    
    menu_pointer = (menu_pointer+1)%menu_len;  // make it wrap arround
  }
  if(last2 == LOW && cur2 == HIGH) {
    menu_pointer = (menu_pointer-1+menu_len)%menu_len;  // make it wrap arround
  }
  if(last3 == LOW && cur3 == HIGH) {
    change_menu();    
  } 
  last1 = cur1;
  last2 = cur2;
  last3 = cur3;

  //  Code for OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  // show time
  display.setCursor(75, 0);  
  strftime_buf[19] = '\0';  // make sure to only print hh:mm:ss
  display.println(&strftime_buf[11]);
  display.drawRect(73, -10, 100, 20, WHITE);

  switch(cur_state) {
    case 0:
      menu_display(); 
      break;
    case 1:
      overview_display();
      break;   
    case 2:
      sensors_display();
      break;      
    case 3:
      notifications_display();
      break;      
    case 4:
      water_display();
      break;         
    case 5:
      plant_display();
      break;      
    case 6:
      credit_display();
      break;      
    case 7:
      light_data();
      break;           
    case 8:
      moist_data();
      break;      
    case 9:
      temp_data(); // need to change when bme connected
      break;      
    case 10:
      humid_data();
      break;  
    case 11:
      clear_notifications();
      break;     
    case 12:
      plant_change();
      break;
    default:  // show logo and back to menu if not coded or an error occurs
      display.drawBitmap(0, 0, BOTanical_logo, 128, 64, 1);
      error_cycle++;
      if (error_cycle == 10) {  // show logo for 1s
        cur_state = 0;
        error_cycle = 0;
      }
      break;             
  }
  
  menu_len = menu_sizes[cur_state]; 

  display.display();

  webSocket.loop();

  if (cur_unit == unit_length || cur_unit == 0) {  // take readings
    sensor_readings();
    update_website();
    cur_unit = 1;  
  } else {
    cur_unit++;
    if (pump_on == 10) {  // 10 * 100ms = 1s of watering
      digitalWrite(PUMP, LOW);      // turn pump OFF
    }
    pump_on++;
  }

    // make time cycle constant
    unsigned long millis_now = millis();
    delay(unit_time);  //    delay(unit_time - (millis_now - previous_millis));
    previous_millis = millis();

  if (cur_cycle == cycle_length + 1) { // reset cycle
    cur_cycle = 1;
    already_watered = LOW;
    
    past_light = avg_light;
    past_moist = avg_moist;
    past_temp = avg_temp;
    past_humid = avg_humid;

    if (avg_light < exp_light[0]) {
      if (bad_light > 0) bad_light = -1;
      else bad_light--; 
    } else if (avg_light > exp_light[1]) {
      if (bad_light < 0) bad_light = 1;
      else bad_light++;
    } else {
      bad_light = 0;
    }
    
    if (avg_moist < exp_moist[0]) {
      if (bad_moist > 0) bad_moist = -1;
      else bad_moist--; 
    } else if (avg_moist > exp_moist[1]) {
      if (bad_moist < 0) bad_moist = 1;
      else bad_moist++;
    } else {
      bad_moist = 0;
    }    
    
    if (avg_temp < exp_temp[0]) {
      if (bad_temp > 0) bad_temp = -1;
      else bad_temp--; 
    } else if (avg_temp > exp_temp[1]) {
      if (bad_temp < 0) bad_temp = 1;
      else bad_temp++;
    } else {
      bad_temp = 0;
    }
    
    if (avg_humid < exp_humid[0]) {
      if (bad_humid > 0) bad_humid = -1;
      else bad_humid--; 
    } else if (avg_humid > exp_humid[1]) {
      if (bad_humid < 0) bad_humid = 1;
      else bad_humid++;
    } else {
      bad_humid = 0;
    }    
  }
}

void sensor_readings() {
  if (already_watered == LOW) {
    if (avg_moist > exp_moist[1] && bad_moist >= 0) { // soil to dry, but not previously overwatered
      digitalWrite(PUMP, HIGH);  // turn the pump ON
      pump_on = 1;
    }
  } else {
    digitalWrite(PUMP, LOW);      // make pump stay OFF
  }

  //Read light level
  light_reading = analogRead(LIGHTSENSORPIN);

  // Water level sensor
  digitalWrite(WATERPOWER, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  water_level = analogRead(WATERPIN); // read the analog value from sensor
  digitalWrite(WATERPOWER, LOW);   // turn the sensor OFF

  // Moisture sensor
  moist_reading = analogRead(SOILPIN); // read the analog value from sensor
  
  // BME280 readings (works for both I2C and SPI)
  temp_reading = bme.readTemperature();
  humid_reading = bme.readHumidity();

  // Calculate averages
  avg_light = avg_light*(cur_cycle-1)/cur_cycle + light_reading/cur_cycle;
  avg_moist = avg_moist*(cur_cycle-1)/cur_cycle + moist_reading/cur_cycle;
  avg_temp = avg_temp*(cur_cycle-1)/cur_cycle + temp_reading/cur_cycle;
  avg_humid = avg_humid*(cur_cycle-1)/cur_cycle + humid_reading/cur_cycle;
  cur_cycle++;  // increment cycle as calculations are done
}

void menu_display(){
  // calculate how many notifs
  int notifs = 0;
  notifs += water_level < water_min;
  notifs += bad_light != 0;
  notifs += bad_moist != 0;
  notifs += bad_temp != 0;
  notifs += bad_humid != 0;
 
  // show menu
  char* menu_text[menu_len] = {"Overview", "Sensor Readings", "Notifications", "Already Watered", "Select Plant", "Credits"};
  for (int i = 0; i < menu_len; i++) {
    display.setCursor(15, 5+10*i);
    display.print(menu_text[i]);      
    if (i == 2 && notifs > 0) {
      display.printf(" (%d)",notifs);
    }
    display.println("");
  }    
  // Draw pointer  
  if (pointer_blink == 0) {
    display.drawCircle(7, 8+10*menu_pointer, 2, WHITE);
    if (cur_unit % 5 == 0) {
      pointer_blink = 1;
    }
  } else {
    display.fillCircle(7, 8+10*menu_pointer, 2, WHITE);
    if (cur_unit % 5 == 0) {
      pointer_blink = 0;
    }
  }  
}

void overview_display(){
  // Display static text
  display.setCursor(0,0);
  if (cur_plant == -1) display.print("Default");
  else display.print(plant_name[cur_plant]) ;
  
  display.setCursor(0, 20);
  if (avg_light < exp_light[0]) {
    display.println("Too little light");    
  } else if (avg_light > exp_light[1]) {
    display.println("Too much light");    
  } else {
    display.println("Good light");
  }
  
  if (avg_moist < exp_moist[0]) {
    display.println("Soil too moist");    
  } else if (avg_moist > exp_moist[1]) {
    display.println("Soil too dry");    
  } else {
    display.println("Good soil moisture");
  }

  if (avg_temp < exp_temp[0]) {
    display.println("Too cold");    
  } else if (avg_temp > exp_temp[1]) {
    display.println("Too hot");    
  } else {
    display.println("Good temperature");
  }

  if (avg_humid < exp_humid[0]) {
    display.println("Too much humidity");    
  } else if (avg_humid > exp_humid[1]) {
    display.println("Too little humidity");    
  } else {
    display.println("Good humidity");
  }
}

void sensors_display() {
  // Display static text
  display.setCursor(0, 0);
  display.println("Options:");

  // show menu
  char* menu_text[menu_len] = {"Light Sensor", "Moisture Sensor", "Temperature sensor", "Humidity sensor", "Back"};
  for (int i = 0; i < menu_len; i++) {
    display.setCursor(15, 15+10*i);
    display.println(menu_text[i]);      
  }    
  // Draw pointer
  if (pointer_blink == 0) {
    display.drawCircle(7, 18+10*menu_pointer, 2, WHITE);
    if (cur_unit % 5 == 0) {
      pointer_blink = 1;
    }
  } else {
    display.fillCircle(7, 18+10*menu_pointer, 2, WHITE);
    if (cur_unit % 5 == 0) {
      pointer_blink = 0;
    }
  }  
}

void notifications_display(){
  // Display static text
  display.setCursor(0, 0);
  display.println("Attention");

  int y = 12;
  if (water_level < water_min) {
    display.setCursor(0, y);
    display.println(" LOW - Refill water");
    y += 10;
  }

  if (bad_light != 0) {
    display.setCursor(0, y);
    if (bad_light > 0) {
      display.printf("strong light ~ %d days", bad_light);
    } else {
      display.printf("weak light ~ %d days", -bad_light);      
    }
    y += 10;
  }  

  if (bad_moist != 0) {
    display.setCursor(0, y);
    if (bad_light > 0) {
      display.printf("dry soil ~ %d days", bad_moist);
    } else {
      display.printf("overwatered ~ %d days", -bad_moist);      
    }
    y += 10;
  }  

  if (bad_temp != 0) {
    display.setCursor(0, y);
    if (bad_temp > 0) {
      display.printf("too cold ~ %d days", bad_temp);
    } else {
      display.printf("too hot ~ %d days", -bad_temp);      
    }
    y += 10;
  }  

  if (bad_humid != 0) {
    display.setCursor(0, y);
    if (bad_light > 0) {
      display.printf("too humid ~ %d days", bad_humid);
    } else {
      display.printf("too arid ~ %d days", -bad_humid);      
    }
  }  
}
 
void water_display() {
  // Display static text
  display.setCursor(0, 17);
  display.println("Have you watered the plant today?");
  display.setCursor(15, 40);
  display.println("Yes (skip cycle)");
  display.setCursor(15, 50);
  display.println("No");
  
  // Draw pointer
  if (pointer_blink == 0) {
    display.drawCircle(7, 43+10*menu_pointer, 2, WHITE);
    if (cur_unit % 5 == 0) {
      pointer_blink = 1;
    }
  } else {
    display.fillCircle(7, 43+10*menu_pointer, 2, WHITE);
    if (cur_unit % 5 == 0) {
      pointer_blink = 0;
    }
  } 
}
  
void plant_display(){
  // Display static text
  display.setCursor(0, 0);
  display.println("Choose from:");
  display.drawRect(1, 15, 126, 48, WHITE);
  
  // show scrolling menu
  char* menu_text[menu_len];
  menu_text[0] = "Back              ";
  for (int i = 0; i < menu_len; i++) {
    menu_text[i+1] = short_name[i];
  }
  
  for (int i = 0; i < 4; i++) { 
    display.setCursor(15, 20+10*i);
    display.println(menu_text[(menu_len + menu_pointer+i-1) % menu_len]);      
  }    
  
  // Draw pointer
  if (pointer_blink == 0) {
    display.drawCircle(7, 33, 2, WHITE);
    if (cur_unit % 5 == 0) {
      pointer_blink = 1;
    }
  } else {
    display.fillCircle(7, 33, 2, WHITE);
    if (cur_unit % 5 == 0) {
      pointer_blink = 0;
    }
  }  

  // Draw scrolling bar
  for (int i = 0; i < 8; i++) {
    display.drawPixel(120, 22+i*5, WHITE);
  }
  display.fillRect(119, 17+(8*menu_pointer*5)/menu_len, 4, 8, WHITE);
}

void credit_display() {
  // Display static text
  display.setCursor(0, 0);
  display.println("Made by:");
  display.setCursor(0, 10);
  display.println("\n  Alexandre Marques\n  Snir Kinog\n  Sahil Singh");
  display.setCursor(0, 52);  
  display.println("ECE 196 WI23");
}      

void light_data() {
  // Display static text
  display.setCursor(0, 20);
  display.printf("Light = %d lux\n",(int) light_reading);
  display.printf("Avg value = %d\n",(int) avg_light);
  display.printf("Cycle = %d / %d\n", (cur_cycle+3)%5+1, cycle_length);
  display.printf("Expected :\n  %d ~ %d\n", (int) exp_light[0], (int) exp_light[1]);
}

void moist_data() {
  // Display static text
  display.setCursor(0, 20);
  display.printf("Moisture = %d\n",(int) moist_reading);
  display.printf("Avg value = %d\n",(int) avg_moist);
  display.printf("Cycle = %d / %d\n", (cur_cycle+3)%5+1, cycle_length);
  display.printf("Expected :\n  %d ~ %d\n", (int) exp_moist[0], (int) exp_moist[1]);
}

void temp_data() {
  // Display static text
  display.setCursor(0, 20);
  display.printf("Temperature = %d C\n", (int) temp_reading);
  display.printf("Avg value = %d\n", (int) avg_temp);
  display.printf("Cycle = %d / %d\n", (cur_cycle+3)%5+1, cycle_length);
  display.printf("Expected :\n  %d ~ %d\n", (int) exp_temp[0], (int) exp_temp[1]);
}

void humid_data() {
  // Display static text
  display.setCursor(0, 20);
  display.printf("Humidity = %d %%\n", (int) humid_reading);
  display.printf("Avg value = %d\n", (int) avg_humid);
  display.printf("Cycle = %d / %d\n", (cur_cycle+3)%5+1, cycle_length);
  display.printf("Expected :\n  %d ~ %d\n", (int) exp_humid[0], (int) exp_humid[1]);
}

void clear_notifications() {
  // Display static text
  display.setCursor(0, 17); 
  display.print("Delete the current\nnotifications?\n(reset daily counter)"); 

  display.drawRect(-1, 47, 130, 20, WHITE);
  display.setCursor(15, 50);
  display.print("Confirm");
  display.setCursor(75, 50);
  display.print("Cancel");

  // Draw pointer
  if (pointer_blink == 0) {
    display.drawCircle(7 + 60*menu_pointer, 53, 2, WHITE);
    if (cur_unit % 5 == 0) {
      pointer_blink = 1;
    }
  } else {
    display.fillCircle(7 + 60*menu_pointer, 53, 2, WHITE);
    if (cur_unit % 5 == 0) {
      pointer_blink = 0;
    }
  }  
}

void plant_change() {
  display.setCursor(0, 0);
  display.println(plant_name[selected_plant]);

  display.setCursor(0, 21);
  display.printf("Light = %d ~ %d\n", plant_light_val[plant_light_level[selected_plant] - 1], plant_light_val[plant_light_level[selected_plant]]);
  display.printf("Temp = %d ~ %d\n", plant_temp_min[selected_plant], plant_temp_max[selected_plant]);
  display.printf("Water = %d ~ %d\n", plant_water_val[plant_water[selected_plant] - 1], plant_water_val[plant_water[selected_plant]]);
        
  display.drawRect(-1, 47, 130, 20, WHITE);
  display.setCursor(15, 50);
  display.print("Confirm");
  display.setCursor(75, 50);
  display.print("Cancel");

  // Draw pointer
  if (pointer_blink == 0) {
    display.drawCircle(7 + 60*menu_pointer, 53, 2, WHITE);
    if (cur_unit % 5 == 0) {
      pointer_blink = 1;
    }
  } else {
    display.fillCircle(7 + 60*menu_pointer, 53, 2, WHITE);
    if (cur_unit % 5 == 0) {
      pointer_blink = 0;
    }
  }  
}

void change_menu() {
  switch(cur_state) {
    case 0:
      cur_state = 1+menu_pointer;
      break;
    case 1:
      cur_state = 0;
      break;
    case 2:
      if (menu_pointer == 4) {
        cur_state = 0;
      } else {
        cur_state = 7+menu_pointer;        
      }
      break;
    case 3:
      cur_state = 11;  
      break;
    case 4:
      cur_state = 0;
      if (menu_pointer == 0) {
        already_watered = HIGH;
      } else {
        already_watered = LOW;
      }
      break;
    case 5:
      if (menu_pointer == 0)
        cur_state = 0;
      else {
        cur_state = 14;
        selected_plant = menu_pointer - 1;
      }
      break;
    case 6:
      cur_state = -1;
      break;
    case 7:
    case 8:
    case 9:
    case 10:
      cur_state = 2;
      break;
    case 11:
      cur_state = 0;
      if (menu_pointer == 0) {
        water_level = 2000;
        bad_light = 0;
        bad_moist = 0;
        bad_temp = 0;
        bad_humid = 0;
      }      
    case 12:
      cur_state = 0;
      if (menu_pointer == 0) {
        cur_plant = selected_plant;
        
        exp_light[0] = plant_light_val[plant_light_level[selected_plant] - 1];
        exp_light[1] = plant_light_val[plant_light_level[selected_plant]];
        
        exp_moist[0] = plant_water_val[plant_water[selected_plant] - 1];
        exp_moist[1] = plant_water_val[plant_water[selected_plant]];
        
        exp_temp[0] = plant_temp_min[selected_plant];
        exp_temp[1] = plant_temp_max[selected_plant];
        
        exp_humid[0] = plant_humidity_min[selected_plant];
        exp_humid[1] = plant_humidity_max[selected_plant];
      }
      break;
    default:  // go to invalid state
      cur_state = -1;
      break;             
  }

  menu_pointer = 0;
}

void update_website() {
    String jsonString = "";                           // create a JSON string for sending data to the client
    StaticJsonDocument<200> doc;                      // create a JSON container
    JsonObject object = doc.to<JsonObject>();         // create a JSON Object
    // temperature 
    object["cur_temp_val"] = temp_reading;            
    object["avg_temp_val"] = avg_temp;
    object["exp_min_temp_val"] = exp_temp[0];
    object["exp_max_temp_val"] = exp_temp[1];
    // humidity
    object["cur_humid_val"] = humid_reading;            
    object["avg_humid_val"] = avg_humid;
    object["exp_min_humid_val"] = exp_humid[0];
    object["exp_max_humid_val"] = exp_humid[1];
    // light 
    object["cur_light_val"] = light_reading;            
    object["avg_light_val"] = avg_light;
    object["exp_min_light_val"] = exp_light[0];
    object["exp_max_light_val"] = exp_light[1];
    // moisture
    object["cur_moist_val"] = moist_reading;            
    object["avg_moist_val"] = avg_moist;
    object["exp_min_moist_val"] = exp_moist[0];
    object["exp_max_moist_val"] = exp_moist[1]; 
    //water level
    object["cur_water_level"] = water_level;
    
    serializeJson(doc, jsonString);                   // convert JSON object to string
//    Serial.println(jsonString);                     
    webSocket.broadcastTXT(jsonString);               // send JSON string to clients
}


void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:                         // if a client is disconnected, then type == WStype_DISCONNECTED
      Serial.println("Client " + String(num) + " disconnected");
      break;
    case WStype_CONNECTED:                            // if a client is connected, then type == WStype_CONNECTED
      Serial.println("Client " + String(num) + " connected");
      break;
    case WStype_TEXT:                                 // if a client has sent data, then type == WStype_TEXT
      // try to decipher the JSON string received
      StaticJsonDocument<200> doc;                    // create a JSON container
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      Serial.println("");
      break;
  }
}
