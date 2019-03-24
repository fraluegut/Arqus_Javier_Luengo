
/**************************************************************************
Javier Luengo
21/03/2019
Sheffield
U.K.
 **************************************************************************/
// SD
#include <SPI.h>
#include <SD.h>
//BME280

#include "Seeed_BME280.h"
#include <Wire.h>

// Light
#include <BH1750.h>


//OLED
#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h> 
#include <SPI.h>

#ifdef ADAFRUIT_HALLOWING
  #define TFT_CS        39 
  #define TFT_RST       37 
  #define TFT_DC        38
  #define TFT_BACKLIGHT  7 
#else
  
  // These pins will also work for the 1.8" TFT shield.
  #define TFT_CS        10
  #define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
  #define TFT_DC         8
#endif


// For 1.44" and 1.8" TFT with ST7735 (including HalloWing) use:
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

BME280 bme280;
BH1750 lightMeter(0x23);
const int chipSelect = 4;

void setup(void) {
  Serial.begin(9600);
//SD Card  
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  // OR use this initializer (uncomment) if using a 1.44" TFT:
  tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab

 Wire.begin();

 //BME280
  if(!bme280.init()){
    Serial.println("Device error!");
  }
 //BH1750
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE_2)) {
    Serial.println(F("BH1750 Advanced begin"));
  }
  else {
    Serial.println(F("Error initialising BH1750"));
    
  Serial.println(F("Initialized"));
}
  Serial.println("");
}

void loop() {
  
float pressure;
float lux = lightMeter.readLightLevel();
String dataString = "";

for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
  }
 
  tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.println("Arqus");
  tft.println("");
  tft.println("");
  
  tft.setTextSize(1);  
  tft.print("Temp: ");
  tft.setTextColor(ST77XX_GREEN);
  tft.print(bme280.getTemperature());
  tft.setTextColor(ST77XX_WHITE);
  tft.println(" C");
  tft.println("");

  tft.print("Hum: ");
  tft.setTextColor(ST77XX_GREEN);
  tft.print(bme280.getHumidity());
  tft.setTextColor(ST77XX_WHITE);
  tft.println("%");
  tft.println("");

  tft.print("Light: ");
  tft.setTextColor(ST77XX_GREEN);
  tft.print(lux);
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Lx");
  tft.println("");

  tft.print("Pressure: ");
  tft.setTextColor(ST77XX_GREEN);
  tft.print(pressure = bme280.getPressure());
  tft.setTextColor(ST77XX_WHITE);
  tft.println("Pa");
  tft.println("");

  //get and print temperatures
  Serial.print("Temp: ");
  Serial.print(bme280.getTemperature());
  Serial.println("C");
  
  //get and print atmospheric pressure data
  Serial.print("Pressure: ");
  Serial.print(pressure = bme280.getPressure());
  Serial.println("Pa");

  //get and print altitude data
  Serial.print("Altitude: ");
  Serial.print(bme280.calcAltitude(pressure));
  Serial.println("m");

  //get and print humidity data
  Serial.print("Humidity: ");
  Serial.print(bme280.getHumidity());
  Serial.println("%");

  Serial.print("Lux: ");
  Serial.print(lux);
  Serial.println(" lx");
  Serial.println("");
  delay(1000);
  
  File dataFile = SD.open("MUESTRAS.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(bme280.getTemperature());
    dataFile.print(",");
    dataFile.print(bme280.getPressure());
    dataFile.print(",");
    dataFile.print(bme280.getHumidity());
    dataFile.print(",");
    dataFile.print(lux);
    dataFile.println(" ");
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening muestras.txt");
  }
}
