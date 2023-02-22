#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>


#define LED 2 // the blue LED pin
#define LIGHTSENSORPIN 39 //Ambient light sensor reading
#define SOILPIN 36  //Soil moisture sensor reading
#define WATERPOWER 17  //Water level sensor VCC
#define WATERPIN 34  //Water level sensor reading
#define BUTTON1 35
#define BUTTON2 32
#define BUTTON3 15

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Create BME280 object
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C


static const unsigned char PROGMEM BOTanical_logo[] = {
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


// variables for calculation
float exp_light = 1000; // expected light over a cycle
float cycle_length = 30; //30 * 2s delay = 1min cycle
float exp_moist = 1000; // threshold for moisture
float avg_light = 0;
float past_average = exp_light; // so its not on at start
float cur_cycle = 0;

// variables for menu
int menu_sizes[] = {6,1,4,1,2,3,1};
int menu_pointer = 0;
int cur_state = 0;
int menu_len = menu_sizes[cur_state];
int pointer_blink = 0;

// variables for the buttons
int last1 = LOW;
int cur1 = LOW;
int last2 = LOW;
int cur2 = LOW;
int last3 = LOW;
int cur3 = LOW;

void setup() {  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(LED, OUTPUT);   // configure pin as an OUTPUT
  pinMode(LIGHTSENSORPIN, INPUT);
  pinMode(SOILPIN, INPUT);
  pinMode(WATERPOWER, OUTPUT);
  pinMode(WATERPIN, INPUT);

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
  delay(2000); // Pause for 2 seconds, let OLED boot up

  // Clear the buffer
  display.clearDisplay();

  // initiate bme280
/*  if (!bme.begin(0x77)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }*/
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
  //Serial.println(strftime_buf);

  // buttons will be LOW(0) or HIGH(1)
  cur1 = digitalRead(BUTTON1); 
  cur2 = digitalRead(BUTTON2);
  cur3 = digitalRead(BUTTON3);

  int pressed1 = LOW;
  int pressed2 = LOW;
  int pressed3 = LOW; 
  if(last1 == LOW && cur1 == HIGH) {
    int pressed1 = HIGH;  // Button 1 was just pressed
    Serial.println("Button 1 pressed");
    
    if (menu_pointer == menu_len-1) {
      menu_pointer = menu_len-1;  // already at the bottom
    } else {
      menu_pointer++;
    }
  }
  if(last2 == LOW && cur2 == HIGH) {
    int pressed2 = HIGH;  // Button 2 was just pressed
    Serial.println("Button 2 pressed");

    if (menu_pointer == 0) {
      menu_pointer = 0;  // already at the top
    } else {
      menu_pointer--;
    }
  }
  if(last3 == LOW && cur3 == HIGH) {
    int pressed3 = HIGH;  // Button 3 was just pressed
    Serial.println("Button 3 pressed");

    change_menu();    
  } 
  last1 = cur1;
  last2 = cur2;
  last3 = cur3;

  float light_reading = analogRead(LIGHTSENSORPIN); //Read light level
  avg_light += light_reading/cycle_length;
  cur_cycle++;

  // Water level sensor
  digitalWrite(WATERPOWER, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  float water_level = analogRead(WATERPIN); // read the analog value from sensor
  digitalWrite(WATERPOWER, LOW);   // turn the sensor OFF

  //Serial.print("The water sensor value: ");
  //Serial.println(water_level);  
  
  // Moisture sensor
  float moist_reading = analogRead(SOILPIN); // read the analog value from sensor
 /* if (moist_reading < exp_moist)
    Serial.print("The soil is DRY (");
  else
    Serial.print("The soil is WET (");
  Serial.print(moist_reading);
  Serial.println(")");*/

  // BME280 readings (works for both I2C and SPI)
  /*
  Serial.print("Temperature = ");  // Convert temperature to Fahrenheit
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");
  
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();*/

  //  Code for OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  // show time
  display.setCursor(75, 0);  
  strftime_buf[19] = '\0';
  display.println(&strftime_buf[11]);
  display.drawRect(73, -10, 100, 20, WHITE);

  switch(cur_state) {
    case 0:
      menu_display(); 
      break;
    case 1:
      overview_display(light_reading);
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
    default:  // show logo and back to menu if not coded or an error occurs
      display.drawBitmap(0, 0, BOTanical_logo, 128, 64, 1);
      cur_state = 0;
      break;             
  }
  
  menu_len = menu_sizes[cur_state];
  Serial.printf("State = %d , menu_size = %d , pointer = %d\n", cur_state, menu_len, menu_pointer);

  if (past_average < exp_light && avg_light < exp_light * cur_cycle/cycle_length) {  // less light then currently expected
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
    
    /*  // Circle with exclamation mark
    display.println("No Light detected!");
    display.fillCircle(64, 40, 16, WHITE);
    display.fillCircle(64, 40, 13, BLACK);
    display.fillRect(62, 30, 4, 10, WHITE);
    display.fillCircle(64, 45, 3, WHITE);*/
  }
  display.display();

  if (cur_cycle == cycle_length) { // reset cycle
    cur_cycle = 0;
    past_average = avg_light;
    avg_light = 0;    
  }

  delay(1000); // set up a delay of 1s
}

void menu_display(){
  // show menu
  char* menu_text[menu_len] = {"Overview", "Sensor Readings", "Notifications", "Already Watered", "Select Plant", "Credits" };
  for (int i = 0; i < menu_len; i++) {
    display.setCursor(15, 5+10*i);
    display.println(menu_text[i]);      
  }    
  // Draw pointer
  if (pointer_blink == 0) {
    display.drawCircle(7, 8+10*menu_pointer, 2, WHITE);
    pointer_blink = 1;
  } else {
    display.fillCircle(7, 8+10*menu_pointer, 2, WHITE);
    pointer_blink = 0;
  }  
}

void overview_display(float light_reading){
  // Display static text
  display.setCursor(0, 20);
  display.print("Light value = ");
  display.println((int) light_reading);
  display.print("Avg value = ");
  display.println((int) avg_light);
  display.print("Cycle = ");
  display.println((int) cur_cycle);
  display.print("C_expected = ");
  display.println(exp_light  * cur_cycle/cycle_length);
}

void sensors_display() {
  // Display static text
  display.setCursor(0, 20);
  display.println("Light sensor");
}

void notifications_display(){
  // Display static text
  display.setCursor(0, 20);
  display.println("Notifications:");
}
 
void water_display() {
  // Display static text
  display.setCursor(0, 20);
  display.println("Have you watered the plant today?\n  Yes\n  No");
}
  
void plant_display(){
  // Display static text
  display.setCursor(0, 20);
  display.println("Choose plant you have.");
}

void credit_display() {
  // Display static text
  display.setCursor(0, 20);
  display.println("Made by:\n  Alexandre Marques\n  Snir Kinog\n  Sahil Singh");
}      

void change_menu() {
  switch(cur_state) {
    case 0:
      cur_state = 1+menu_pointer;
      break;
    case 1:
      cur_state = 0;
      break;  
    case 6:
      cur_state = -1;
      break;      
    default:  // go to invalid state
      cur_state = -1;
      break;             
  }

  menu_pointer = 0;
}
