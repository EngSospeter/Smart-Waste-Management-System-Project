const int trigPin = 9;
const int echoPin = 10;
const float binDepth = 50.0; // Total depth of the bin in cm

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
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
  
  Serial.print("Garbage Level: ");
  Serial.print(garbageLevel);
  Serial.println(" %");
  
  delay(1000);
}
