// Define the pins connected to the HC-SR04 sensor
const int trigPin = 9; // Pin 9 is used to send the trigger signal
const int echoPin = 10; // Pin 10 is used to receive the echo signal

// Define the total depth of the bin in centimeters
const float binDepth = 50.0; // Adjust this value to match the actual depth of your bin

// Setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 bits per second
  pinMode(trigPin, OUTPUT); // Set trigPin as an output to send the trigger signal
  pinMode(echoPin, INPUT); // Set echoPin as an input to receive the reflected signal
}

// Main loop function runs over and over again forever
void loop() {
  // Ensure the trigger pin is low to start with, clearing any previous signal
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); // Wait for 2 microseconds to stabilize the sensor

  // Send a 10-microsecond pulse to the trigger pin
  digitalWrite(trigPin, HIGH); // Set the trigPin high to send the pulse
  delayMicroseconds(10); // Wait for 10 microseconds for the pulse duration
  digitalWrite(trigPin, LOW); // Set the trigPin low to stop the pulse

  // Measure the time it takes for the echo to be received
  long duration = pulseIn(echoPin, HIGH); // Read the time (in microseconds) for which the echoPin stays high

  // Calculate the distance to the nearest object based on the time and the speed of sound (0.034 cm/us)
  float measuredDistance = duration * 0.034 / 2; // Divide by 2 to get the one-way distance

  // Ensure the measured distance does not exceed the total bin depth
  if (measuredDistance > binDepth) measuredDistance = binDepth; // Cap the distance at the bin depth

  // Calculate the garbage level as a percentage of the bin's total depth
  float garbageLevel = (1 - (measuredDistance / binDepth)) * 100; // Convert to percentage

  // Print the calculated garbage level to the Serial Monitor
  Serial.print("Garbage Level: "); // Print label
  Serial.print(garbageLevel); // Print the calculated garbage level
  Serial.println(" %"); // Print the unit (percentage) and move to the next line

  delay(1000); // Wait for 1 second before the next measurement to avoid continuous reading
}
