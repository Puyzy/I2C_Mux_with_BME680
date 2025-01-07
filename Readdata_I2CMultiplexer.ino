#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>


#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 32     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Initialize BME680 on I2C address 0x77
Adafruit_BME680 bme1;
Adafruit_BME680 bme2;

// TCA9548A I2C multiplexer address
#define TCA9548A_ADDR 0x70

// Function to switch between I2C channels
void tcaSelect(uint8_t channel) {
  if (channel > 7) return;  // Multiplexer has 8 channels, 0-7
  Wire.beginTransmission(TCA9548A_ADDR);
  Wire.write(1 << channel);  // Send byte to select channel
  Wire.endTransmission();
}

void setup() {

  Serial.begin(115200);
  // Start I2C communication with the Multiplexer
  Wire.begin();
  // Initialize SSD1306 OLED 1 on channel 0 of the multiplexer
  tcaSelect(0);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  // Clear the buffer
  display.clearDisplay();

  // Initialize BME680 sensor 1 on channel 1 of the multiplexer
  tcaSelect(0);
  if (!bme1.begin()) {
    Serial.println("Could not find BME680 sensor 1!");
    while (1)
      ;
  }
  // Initialize BME680 sensor 2 on channel 2 of the multiplexer
  tcaSelect(1);
  if (!bme2.begin()) {
    Serial.println("Could not find BME680 sensor 2!");
    while (1)
      ;
  }

  // // Set up oversampling and other sensor settings for sensor 1
  // tcaSelect(0);
  // display.clearDisplay();
  // display.setTextSize(2);
  // display.setTextColor(WHITE);
  // display.setCursor(45, 10);
  // // Display static text
  // display.println("0");
  // display.display();
  // delay(2000);
  // display.clearDisplay();

  tcaSelect(0);
  bme1.setTemperatureOversampling(BME680_OS_8X);
  //bme1.setHumidityOversampling(BME680_OS_2X);
  //bme1.setPressureOversampling(BME680_OS_4X);
  bme1.setIIRFilterSize(BME680_FILTER_SIZE_3);

  // Set up oversampling and other sensor settings for sensor 2
  tcaSelect(1);
  bme2.setTemperatureOversampling(BME680_OS_8X);
  //bme2.setHumidityOversampling(BME680_OS_2X);
  //bme2.setPressureOversampling(BME680_OS_4X);
  bme2.setIIRFilterSize(BME680_FILTER_SIZE_3);
}

void loop() {
  // Read data from BME680 sensor 1 on channel 0

  // tcaSelect(0);
  // display.clearDisplay();
  // display.setTextSize(3);
  // display.setTextColor(WHITE);
  // display.setCursor(45, 10);
  // // Display static text
  // display.println("100");
  // display.display();



  tcaSelect(0);
  if (!bme1.performReading()) {
    Serial.println("Failed to perform reading on BME680 sensor 1");
    return;
  }
  Serial.print("Sensor 1 - Temperature: ");
  Serial.print(bme1.temperature);
  Serial.println(" °C");
  // Serial.print(" °C, Humidity: ");
  // Serial.print(bme1.humidity);
  // Serial.print(" %, Pressure: ");
  // Serial.print(bme1.pressure / 100.0);
  // Serial.println(" hPa");

  // Read data from BME680 sensor 2 on channel 1
  tcaSelect(1);
  if (!bme2.performReading()) {
    Serial.println("Failed to perform reading on BME680 sensor 2");
    return;
  }
  Serial.print("Sensor 2 - Temperature: ");
  Serial.print(bme2.temperature);
  Serial.println(" °C");
  //Serial.print(" °C, Humidity: ");
  // Serial.print(bme2.humidity);
  // Serial.print(" %, Pressure: ");
  // Serial.print(bme2.pressure / 100.0);
  // Serial.println(" hPa");

  // Add a delay for readability
  delay(2000);
}