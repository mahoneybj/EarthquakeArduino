#include <WiFiS3.h>
#include <WiFiSSLClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

// Pins for breadboard connections
const int redLedPin = 2;
const int yellowLedPin = 3;
const int greenLedPin = 4;
const int buttonPin = 5;
const int buzzerPin = 6;

char ssid[] = "Pig";           // WiFi network details
char pass[] = "TommyInnit72"; 

// Server details
const char server[] = "earthquake-web.azurewebsites.net";
const char* resource = "/api/earthquake-alerts";

WiFiSSLClient client;
unsigned long lastFetchTime = 0;
const unsigned long fetchInterval = 500; // 0.5 seconds

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  delay(5000);

  // Initialize pins
  pinMode(redLedPin, OUTPUT); // Red means alert
  pinMode(yellowLedPin, OUTPUT); // Yellow means error or standby
  pinMode(greenLedPin, OUTPUT); // Green means all sysetms go (Waiting for alert)
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  lcd.init();
  lcd.backlight();

  while (!Serial);
  // Connect to WiFi network
  Serial.print("Attempting to connect to network: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
    digitalWrite(yellowLedPin, HIGH);
  }
  
  Serial.println("Connected to WiFi");
  digitalWrite(greenLedPin, HIGH);
  connectToServer();
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastFetchTime >= fetchInterval) {
    fetchEarthquakeAlerts();
    lastFetchTime = currentTime;
    digitalWrite(greenLedPin, HIGH);
  }
}

void connectToServer() {
  if (client.connect(server, 443)) {
    Serial.println("Connected to server");
  } else {
    Serial.println("ERROR: Connection to server failed");
    digitalWrite(yellowLedPin, HIGH); // Error light
  }
}

void fetchEarthquakeAlerts() {
  if (!client.connected()) {
    Serial.println("Attempting to reconnect to server...");
    connectToServer();
  }

  client.print("GET ");
  client.print(resource);
  client.println(" HTTP/1.1");
  client.println("Host: earthquake-web.azurewebsites.net");
  client.println("Connection: keep-alive"); // Keep alive used so Azure wont kick arduino
  client.println();  // End of HTTP headers

  bool headerPassed = false;
  String jsonResponse = "";
  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 500) { // 0.5 seconds timeout
    if (client.available()) {
      String line = client.readStringUntil('\n');
      // Go through each header
      if (line == "\r") {
        headerPassed = true;
      }
      // If headers are passed, concatenate JSON data
      if (headerPassed && line != "\r") {
        jsonResponse += line;
      }
      timeout = millis(); // Reset timeout timer on activity
    }
  }

  // Check if the response is empty
  if (jsonResponse.length() == 0) {
    Serial.println("Empty response received");
    return;
  }
  Serial.println("Get request sucessful");
  lcd.clear();
  // Parse JSON response
  DynamicJsonDocument doc(2048); 
  DeserializationError error = deserializeJson(doc, jsonResponse);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    digitalWrite(yellowLedPin, HIGH); // Error light
    return;
  }
  digitalWrite(yellowLedPin, LOW);

  
  JsonArray data = doc["data"];
  if (!data.isNull() && data.size() > 0) {
    JsonObject latestAlert = data[data.size() - 1]; // Get the latest alert
    bool active = latestAlert["active"]; // Get active to see if alert is valid
    if (active) {
      float distance = latestAlert["distance"]; // Get distance
      triggerAlert(distance);
    }
  } else {
    Serial.println("No data available"); // Not an error, just that database has no alerts
  }
}

void triggerAlert(float distance) {
  digitalWrite(redLedPin, HIGH); // Turn alert led high
  digitalWrite(yellowLedPin, LOW);
  digitalWrite(greenLedPin, LOW);

  bool acknowledge = false; // Hush button bool
  int timeToImpact = distance / 4.0; // Calculate time to impact in seconds
  bool timeOut = false; // Check to see if coundown has concluded

  // Loop until the button is pressed
  while (!acknowledge) {
    while (!timeOut) {
      for (int i = timeToImpact - 3; i >= 0; i--) { // Time to impact  - average latency of device reciving
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Earthquake Alert!");
        lcd.setCursor(0, 1);
        lcd.print("Impact in: ");
        lcd.print(i);
        lcd.print("s");

        // Sound the buzzer
        tone(buzzerPin, 1000); // Sound the buzzer
        delay(500);            
        noTone(buzzerPin);     // Turn off the buzzer
        delay(500);            

        if(i == (timeToImpact - 4)){
          timeOut = true;
        }
      // Check if the button is pressed
        if (digitalRead(buttonPin) == HIGH) {
          acknowledge = true;
          break;
        }
      }
    }
    tone(buzzerPin, 1000); // Sound the buzzer
    delay(100);           
    noTone(buzzerPin);     // Turn off the buzzer
    delay(100);            
    if (digitalRead(buttonPin) == HIGH) { // Check if buton has been pressed
          acknowledge = true;
          break;
    }
  }

  // Turn off LED and clear LCD after acknowledgment
  digitalWrite(redLedPin, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Alert hushed.");
  delay(1000);

  Serial.println("Earthquake alert hushed.");
}

