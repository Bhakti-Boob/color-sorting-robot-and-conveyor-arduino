/* Notes:
    - Conveyor runs by default.
    - When IR detects an object, conveyor is stopped, object is allowed to settle, color is read, pick-and-place executed, then conveyor restarts.
*/

#include <Servo.h>

// Pin definitions
const int IR_PIN     = 13;   // IR sensor digital output 
const int RELAY_PIN  = 12;   // Relay pin for conveyor

// TCS3200 (GY-31) control pins
const int S0 = 4;
const int S1 = 5;
const int S2 = 6;
const int S3 = 7;
const int OUT_PIN = 8;

// Indicator LED
const int LED_R = A0;
const int LED_G = A1;
const int LED_B = A2;

// Servos
Servo servo1; // main arm
Servo servo2; // base rotation
Servo servo3; // gripper 

const uint8_t CONVEYOR_RUN_STATE = HIGH; 
const uint8_t CONVEYOR_STOP_STATE = (CONVEYOR_RUN_STATE == HIGH) ? LOW : HIGH;

const uint16_t SETTLE_MS = 500;        // wait after stopping conveyor so object settles
const uint16_t POST_PICK_DELAY = 500;  // small delay after actions
const uint8_t  READS = 4;              // number of pulses to average per channel

// Thresholds (tuned for lighting and sensor)
const int RED_THRESHOLD   = 23;
const int BLUE_THRESHOLD  = 20;
const int GREEN_THRESHOLD = 20;

// Storage for measured pulse durations 
int Red = 0, Blue = 0, Green = 0;

void setup() {
  Serial.begin(9600);

  // pins
  pinMode(IR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT_PIN, INPUT);

  // Attach servos 
  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(11);

  // Set initial servo positions
  servo1.write(30);   // home and ready position
  servo2.write(90);   // center bin position
  servo3.write(90);   // gripper open

  // TCS3200 frequency scaling 
  // S0 S1 = HIGH HIGH -> 100% 
  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);

  // Start conveyor 
  digitalWrite(RELAY_PIN, CONVEYOR_RUN_STATE);
}

void loop() {
  // Read IR sensor 
  int irVal = digitalRead(IR_PIN);

  // If object is detected, stop the conveyor and process it
  if (irVal == HIGH) { 
    Serial.println("Object detected - stopping conveyor");
    digitalWrite(RELAY_PIN, CONVEYOR_STOP_STATE); // stop conveyor
    delay(50); // small debounce
    // Wait for object to settle
    delay(SETTLE_MS);

    // Read and average color values
    GetColoursAveraged();

    Serial.print("Raw pulses - R: "); Serial.print(Red);
    Serial.print("  G: "); Serial.print(Green);
    Serial.print("  B: "); Serial.println(Blue);

    // Decide color
    if (Red < Blue && Red < Green && Red <= RED_THRESHOLD) {
      Serial.println("Detected: RED");
      indicateColor(LED_R);
      pickAndPlaceForBin(0); 
    }
    else if (Blue < Red && Blue < Green && Blue <= BLUE_THRESHOLD) {
      Serial.println("Detected: BLUE");
      indicateColor(LED_B);
      pickAndPlaceForBin(1);
    }
    else if (Green < Red && Green < Blue && Green <= GREEN_THRESHOLD) {
      Serial.println("Detected: GREEN");
      indicateColor(LED_G);
      pickAndPlaceForBin(2);
    }
    else {
      Serial.println("Color ambiguous!!! No pick!!!");
    }

    // Restart conveyor after handling
    Serial.println("Restarting conveyor");
    digitalWrite(RELAY_PIN, CONVEYOR_RUN_STATE);
    delay(POST_PICK_DELAY);

    // LEDs off
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);

    // Small delay to avoid redetecting the same object immediately
    delay(500);
  }
  // Else, no object detected, conveyor keeps running
  else {
    digitalWrite(RELAY_PIN, CONVEYOR_RUN_STATE);
  }

  // Loop small delay
  delay(50);
}

// Flash one LED and turn others off
void indicateColor(int ledPin) {
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);
  digitalWrite(ledPin, HIGH);
}

// binId: 0 = red bin, 1 = blue bin, 2 = green bin
void pickAndPlaceForBin(int binId) {
  // Servo angles for each bin 
  const int BIN_ANGLE[] = {0, 150, 180}; // servo2 angles for bins 
  const int PICK_ANGLE = 30;             // servo1 over pickup
  const int TRAVEL_ANGLE = 90;           // servo1 travel or home
  const int GRIP_CLOSE = 130;            // servo3 closed
  const int GRIP_OPEN  = 0;              // servo3 open

  // Move to pickup
  servo1.write(PICK_ANGLE);
  delay(800);
  servo3.write(GRIP_CLOSE); // grab
  delay(800);

  // Lift slightly and move to travel
  servo1.write(TRAVEL_ANGLE);
  delay(800);

  // Move selector to bin
  servo2.write(BIN_ANGLE[binId]);
  delay(800);

  // Move to drop pose and release
  servo1.write(30);    // lower into bin area 
  delay(800);
  servo3.write(GRIP_OPEN); // release
  delay(800);

  // Return to safe home positions
  servo1.write(TRAVEL_ANGLE);
  delay(600);
  servo2.write(90); // center
  delay(600);
}

// Read each channel multiple times and average to smooth noisy readings
void GetColoursAveraged() {
  long rSum = 0, gSum = 0, bSum = 0;
  for (uint8_t i = 0; i < READS; ++i) {
    // RED
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    rSum += pulseIn(OUT_PIN, digitalRead(OUT_PIN) == HIGH ? LOW : HIGH, 20000); 

    // BLUE
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    bSum += pulseIn(OUT_PIN, digitalRead(OUT_PIN) == HIGH ? LOW : HIGH, 20000);

    // GREEN
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    gSum += pulseIn(OUT_PIN, digitalRead(OUT_PIN) == HIGH ? LOW : HIGH, 20000);

    delay(20);
  }

  Red   = (int)(rSum / READS);
  Blue  = (int)(bSum / READS);
  Green = (int)(gSum / READS);
}

