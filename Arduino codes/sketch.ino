const int trigPin = 9;         // HC-SR04 trigger pin
const int echoPin = 10;        // HC-SR04 echo pin
const float binDepth = 21.0;   // Total depth of the bin in cm

void setup() {
  Serial.begin(9600);          // Debugging
  Serial1.begin(9600);         // SIM800L communication (pins 18, 19)
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  delay(1000);
  Serial.println("Initializing SIM800L...");
  sendCommand("AT", 2000);     // Test communication with SIM800L
  sendCommand("AT+CSQ", 2000);// Check signal strength
  sendCommand("AT+CREG?", 2000); // Network registration status

  // Start GPRS connection
  sendCommand("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", 2000);
  sendCommand("AT+SAPBR=3,1,\"APN\",\"your_apn\"", 2000); // Replace "your_apn" with your network's APN
  sendCommand("AT+SAPBR=1,1", 5000);
  sendCommand("AT+SAPBR=2,1", 2000);
}

void loop() {
  int garbageLevel = measureGarbageLevel(); // Get garbage level
  sendHTTPPost("http://Sospeter1609.pythonanywhere.com/update_bin", "level=" + String(garbageLevel));
  delay(60000); // Send data every 60 seconds
}

// Measure garbage level using HC-SR04
int measureGarbageLevel() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  float measuredDistance = duration * 0.034 / 2;
  
  // Ensure the measured distance doesn't exceed the bin depth
  if (measuredDistance > binDepth) measuredDistance = binDepth;
  
  float garbageLevel = (1 - (measuredDistance / binDepth)) * 100;
  int roundedLevel = round(garbageLevel); // Round to the nearest integer
  Serial.print("Garbage Level: ");
  Serial.print(roundedLevel);
  Serial.println(" %");
  
  return roundedLevel;
}

// Function to send AT commands
void sendCommand(String command, int timeout) {
  Serial1.println(command);
  long int time = millis();
  while ((time + timeout) > millis()) {
    while (Serial1.available()) {
      char c = Serial1.read();
      Serial.print(c); // Debug response
    }
  }
}

// Function to send HTTP POST request
void sendHTTPPost(String url, String body) {
  sendCommand("AT+HTTPINIT", 2000);                          // Initialize HTTP service
  sendCommand("AT+HTTPPARA=\"CID\",1", 2000);                // Set bearer profile identifier
  sendCommand("AT+HTTPPARA=\"URL\",\"" + url + "\"", 2000);  // Set URL
  sendCommand("AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"", 2000); // Set content type
  sendCommand("AT+HTTPDATA=" + String(body.length()) + ",10000", 2000); // Set HTTP data
  delay(2000);                                               // Wait for the prompt
  Serial1.print(body);                                       // Send POST body
  delay(1000);
  sendCommand("AT+HTTPACTION=1", 5000);                      // Execute POST request
  sendCommand("AT+HTTPREAD", 5000);                          // Read response
  sendCommand("AT+HTTPTERM", 2000);                          // Terminate HTTP service
}
