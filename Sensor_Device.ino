#include <Wire.h>  // Wire library - used for I2C communication

// ------------------------- Declare Constants ------------------------- //
#define XDATA3 0x08  // define XDATA3 as 0x08
#define YDATA3 0x0B  // define YDATA3 as 0x0B
#define ZDATA3 0x0E  // define ZDATA3 as 0x0E

#define RANGE_2G 2.048  // define RANGE_2G as decimal 2.048
#define RANGE_4G 4.096  // define RANGE_4G as decimal 4.096
#define RANGE_8G 8.192  // define RANGE_8G as decimal 8.192

#define RANGE_SETTING RANGE_8G  // use the 8G range for more sensitivity

// ------------------------- Declare Global Variables ------------------------- // 
int ADXL355 = 0x1D;  // ADXL355 I2C address

// Setup
void setup() 
{   
  Serial.begin(9600);  // Initiate serial communication
  delay(200);
  Wire.begin();  // Initiate the Wire library
  delay(200);
  reset();  // Reset the device
  delay(200);
  enable();  // Enable the device
  delay(200);
  setRange();  // Set the range
  delay(200);
  Serial.println("Started");  // Print "Started" on the serial monitor
  delay(200);

}

// Main loop
void loop() 
{
  AccelerationData();
}

// function readOrientation
float readOrientation(uint8_t orientation) {
  Wire.beginTransmission(ADXL355);  // begin data transmission from ADXL355
  Wire.write(orientation);  // write orientation register address
  Wire.endTransmission();  // end data transmission
  Wire.requestFrom(ADXL355, 3);  // request 3 bytes from ADXL355

  // Read the three bytes of data
  uint32_t v1 = Wire.read(); 
  uint32_t v2 = Wire.read(); 
  uint32_t v3 = Wire.read(); 

  uint32_t data = (v1 << 16) | (v2 << 8) | v3;  // Combine bytes into a single data value
  data = data >> 4;  // Bit shift data 4 bits to the right
  float value = (float)data;  // Convert data to float
  if (data > 0x7FFFF) value -= 0x100000;  // Adjust for negative values
  return value / 0x7FFFF * RANGE_SETTING;  // Scale to the chosen range
}

// function enable
void enable() { 
  bool deviceFound = false;
  while (!deviceFound) {
    reset(); 
    Wire.beginTransmission(ADXL355);  // begin data transmission from ADXL355
    Wire.write(0x00);  // write to address 0x00
    Wire.endTransmission();  // end data transmission
    Wire.requestFrom(ADXL355, 1);  // request 1 byte from ADXL355
    delay(100);
    if (Wire.read() == 0xAD) {  // if device ID is correct
      Serial.println("Device found");  // print "Device found" on serial monitor
      deviceFound = true;
    } else {
      Serial.println("Device not found, retrying...");  // print "Device not found, retrying..." on serial monitor
      delay(1000);  // wait 1 second before retrying
    }
  }
  Wire.beginTransmission(ADXL355);  // begin data transmission from ADXL355
  Wire.write(0x2D);  // write to POWER_CTL register
  Wire.write(0x00);  // set device to measurement mode
  Wire.endTransmission();  // end data transmission
}

// function setRange
void setRange() {
  Wire.beginTransmission(ADXL355);  // begin data transmission from ADXL355
  Wire.write(0x2C);  // write to RANGE register
  Wire.write(0x81);  // set the range to ±8g
  Wire.endTransmission();  // end data transmission
}

// function AccelerationData
void AccelerationData() {
  Serial.print(readOrientation(XDATA3)); Serial.print(",");  // print X-axis data
  Serial.print(readOrientation(YDATA3)); Serial.print(",");  // print Y-axis data
  Serial.print(readOrientation(ZDATA3));  // print Z-axis data
  Serial.println();  // print a newline
}

// function reset
void reset() {
  Wire.beginTransmission(ADXL355);  // begin data transmission from ADXL355
  Wire.write(0x2F);  // write to reset register
  Wire.write(0x52);  // send reset command
  Wire.endTransmission();  // end data transmission
}

