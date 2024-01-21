const int trigpin = 13;
const int echopin = 12;
float duration, distance;
float distArray[5] = {0};
int distInd = 0;
unsigned long currentTime = 0;
unsigned long printLastTime = 0;

int motor1pin1 = 2;
int motor1pin2 = 3;

int motor2pin1 = 4;
int motor2pin2 = 5;

int forwardEn = 8;
int ccwEn = 7;
int cwEn = 6;
int doneSignal = A0;

void setup() {
  // put your setup code here, to run once:
   pinMode(motor1pin1, OUTPUT);
   pinMode(motor1pin2, OUTPUT);
   pinMode(motor2pin1, OUTPUT);
   pinMode(motor2pin2, OUTPUT);

   pinMode(9, OUTPUT);
   pinMode(10, OUTPUT);

   pinMode(trigpin, OUTPUT);
   pinMode(echopin, INPUT);

   pinMode(forwardEn, INPUT);
   pinMode(ccwEn, INPUT);
   pinMode(cwEn, INPUT);
   pinMode(doneSignal, OUTPUT);

   Serial.begin(9600);
}

float getDistance(){
    digitalWrite(trigpin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigpin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin, LOW);
    duration = pulseIn(echopin, HIGH);
    distArray[distInd] = duration * 0.0343 / 2;
    int sum = 0;
    for (int i = 0; i < 5; i++){
      sum = sum + distArray[i];
    }
    distance = sum/5;
    distInd = distInd + 1;
    distInd = distInd % 5;
    if (currentTime - printLastTime > 200) {
      Serial.print("Distance: ");
      Serial.println(distance);
    }
    return distance;

}

void driveForward(float t){
  // actuates the motors to drive the robot forward for t seconds
  currentTime = millis();
  unsigned long driveStartTime = millis();
  float timeLeft = t * 1000; // converting to ms

  while (millis() - driveStartTime < timeLeft){
    currentTime = millis();
    //set motor speeds high
    analogWrite(9, 200);
    analogWrite(10, 200);
    //turn motors on
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
    //get distance readings
    getDistance();

    if (distance < 15) timeLeft = timeLeft - (currentTime - driveStartTime); //if pausing since too close, reduce timeLeft
    while (distance < 15) { //ultrasonic sensor reading too close
      //turn motors off
      digitalWrite(motor1pin1, LOW);
      digitalWrite(motor2pin1, LOW);
      //continue sensor readings
      getDistance();
      //update
      // if (distance < 15) Serial.println("too close, waiting");
      driveStartTime = millis();
    }
  }
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);

}

void turnCCW(float t){
  // actuates the motors to turn the robot counterclockwise for t seconds
  currentTime = millis();
  unsigned long driveStartTime = millis();
  float timeLeft = t * 1000; // converting to ms

  while (millis() - driveStartTime < timeLeft){
    currentTime = millis();
    //set motor speeds high
    analogWrite(9, 200);
    analogWrite(10, 200);
    //turn motors on
    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    //get distance readings
    getDistance();

    if (distance < 15) timeLeft = timeLeft - (currentTime - driveStartTime); //if pausing since too close, reduce timeLeft
    while (distance < 15) { //ultrasonic sensor reading too close
      //turn motors off
      digitalWrite(motor1pin1, LOW);
      digitalWrite(motor1pin2, LOW);
      digitalWrite(motor2pin1, LOW);
      digitalWrite(motor2pin2, LOW);
      //continue sensor readings
      getDistance();
      //update
      // if (distance < 15) Serial.println("too close, waiting");
      driveStartTime = millis();
    }
  }
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);

}

void turnCW(float t){
  // actuates the motors to turn the robot clockwise for t seconds
  currentTime = millis();
  unsigned long driveStartTime = millis();
  float timeLeft = t * 1000; // converting to ms

  while (millis() - driveStartTime < timeLeft){
    currentTime = millis();
    //set motor speeds high
    analogWrite(9, 200);
    analogWrite(10, 200);
    //turn motors on
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
    //get distance readings
    getDistance();

    if (distance < 15) timeLeft = timeLeft - (currentTime - driveStartTime); //if pausing since too close, reduce timeLeft
    while (distance < 15) { //ultrasonic sensor reading too close
      //turn motors off
      digitalWrite(motor1pin1, LOW);
      digitalWrite(motor1pin2, LOW);
      digitalWrite(motor2pin1, LOW);
      digitalWrite(motor2pin2, LOW);
      //continue sensor readings
      getDistance();
      //update
      // if (distance < 15) Serial.println("too close, waiting");
      driveStartTime = millis();
    }
  }
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);

}


void loop() {
  // put your main code here, to run repeatedly:
  int fenVal = digitalRead(forwardEn);
  if (fenVal == HIGH) Serial.println("forward command signal high");
  int ccwenVal = digitalRead(ccwEn);
  int cwenVal = digitalRead(cwEn);
  if (fenVal == HIGH){
    driveForward(3);
    digitalWrite(doneSignal, HIGH);
    delay(10);
    digitalWrite(doneSignal, LOW);
  } else if (ccwenVal == HIGH){
    turnCCW(1);
    digitalWrite(doneSignal, HIGH);
    delay(10);
    digitalWrite(doneSignal, LOW);
  } else if (cwenVal == HIGH){
    turnCW(1);
    digitalWrite(doneSignal, HIGH);
    delay(10);
    digitalWrite(doneSignal, LOW);
  }
}
