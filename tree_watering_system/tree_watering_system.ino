// This project is for an Automated Christmas Tree Watering System
// I used an Elegoo Mega 2560(Ardunio-like)

// Water Level Detection Module
const int treeWaterSensor = A0;
// Found a 5v submersible pump on Amazon likely used in server cooling systems
// https://www.amazon.com/gp/product/B01LWXV7DE/ref=oh_aui_detailpage_o00_s00?ie=UTF8&psc=1
const int submersiblePump = A3;
// UnltrasonicSensor
const int reserveTankUltrasonicEchoPin = 24;
const int reserveTankUltrasonicTriggerPin = 34;
// Active Buzzer
const int annoyingBuzzer = 46;

int treeWaterSensorValue = 0;

unsigned long previousMillis = 0;
const long interval = 10000;

long duration;
long distance;

void setup()
{
  pinMode(treeWaterSensor, INPUT);
  pinMode(submersiblePump, OUTPUT);
  pinMode(annoyingBuzzer, OUTPUT);
  pinMode(reserveTankUltrasonicEchoPin, INPUT);
  pinMode(reserveTankUltrasonicTriggerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  treeWaterSensorValue = analogRead(treeWaterSensor);
  // Delay without using native delay()
  // This is suggested by many people in Arduino forums
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    checkReserveTankLevel();
    checkTreeTankLevel(treeWaterSensorValue);
  }
}

int checkTreeTankLevel(int treeWaterSensorValue){
  Serial.println("Water Sensor value: ");
  Serial.println(analogRead(treeWaterSensorValue));
  if (treeWaterSensorValue < 1) {
    analogWrite(submersiblePump, 255);
    beepNTimes(2);
  } else {
    analogWrite(submersiblePump, LOW);
    digitalWrite(annoyingBuzzer, LOW);
  }
}

int beepNTimes(int n){
  int count = n;
  while(count > 0 )
  {
    digitalWrite(annoyingBuzzer, HIGH);
    delay(50);
    digitalWrite(annoyingBuzzer, LOW);
    delay(50);
    count = count -1;
  }
}

int checkReserveTankLevel(){
  digitalWrite(reserveTankUltrasonicTriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(reserveTankUltrasonicTriggerPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(reserveTankUltrasonicTriggerPin, LOW);
  duration = pulseIn(reserveTankUltrasonicEchoPin, HIGH);
  // Some arbitrary calculation to get something close measuring in cm
  distance = duration / 58.2;
  Serial.println("Reserve Sensor value: ");
  Serial.println(distance);
  if (distance > 15) {
    digitalWrite(annoyingBuzzer, HIGH);
    analogWrite(submersiblePump, LOW);
  } else {
    digitalWrite(annoyingBuzzer, LOW);
  }
  delay(50);
}
