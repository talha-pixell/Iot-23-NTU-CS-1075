//TITLE: DHT11 and LDR Sensor Data Display on OLED
//ZAIN RAZA ZAIDI
//23-NTU-CS-B-1096

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>


#define DHTPIN 14        // DHT22 data pin
#define DHTTYPE DHT11   
#define LDR_PIN 34
#define SDA_PIN 21//i2c SDA pin
#define SCL_PIN 22  //i2c SCL pin
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
// --- DHT sensor setup ---
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();

  dht.begin();
  delay(1000);

}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if read failed
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error reading DHT22 sensor!");
    return;
  }
  int adcValue = analogRead(LDR_PIN);
  float voltage = (adcValue / 4095.0) * 3.3;

   // Print values on Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C  |  Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(0, 16);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");
  display.setCursor(0, 32);
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");

  display.setCursor(0,10);
  display.print("LDR ADC: "); display.println(adcValue);
  display.print("Voltage: "); display.print(voltage, 2); display.println(" V");
  display.display();

  Serial.printf("ADC: %d  |  Voltage: %.2f V\n", adcValue, voltage);
  delay(1000);
}