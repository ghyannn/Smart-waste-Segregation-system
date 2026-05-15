#include <ESP32Servo.h>
#include <Stepper.h>

// ---------------- STEPPER ----------------
const int stepsPerRevolution = 2048;
Stepper stepper(stepsPerRevolution, 18, 21, 19, 22);
// ---------------- SERVO ----------------
Servo flapServo;
#define SERVO_PIN 23

// ---------------- SENSORS ----------------
#define IR_SENSOR     34
#define METAL_SENSOR  35
#define RAIN_SENSOR   32

// ---------------- SETTINGS ----------------
int stepSpeed = 12;

int dryPos   = 0;
int wetPos   = 700;
int metalPos = 1400;

int currentPos = 0;

// ---------------- MOVE STEPPER ----------------
void moveTo(int target)
{
  int steps = target - currentPos;
  stepper.step(steps);
  currentPos = target;
}

// ---------------- SERVO ----------------
void openFlap()
{
  Serial.println("Opening flap...");
  flapServo.write(90);
  delay(300);
  flapServo.write(0);
}

// ---------------- SETUP ----------------
void setup()
{
  Serial.begin(115200);
  delay(2000);   // 🔥 IMPORTANT for ESP32

  Serial.println("=== SYSTEM STARTED ===");

  flapServo.attach(SERVO_PIN);
  flapServo.write(0);

  pinMode(IR_SENSOR, INPUT);
  pinMode(METAL_SENSOR, INPUT);

  stepper.setSpeed(stepSpeed);
}

// ---------------- LOOP ----------------
void loop()
{
  int irValue = digitalRead(IR_SENSOR);
  int metal   = digitalRead(METAL_SENSOR);
  int rain    = analogRead(RAIN_SENSOR);

  // 🔥 Continuous debug print
  Serial.print("IR: ");
  Serial.print(irValue);
  Serial.print(" | Metal: ");
  Serial.print(metal);
  Serial.print(" | Rain: ");
  Serial.println(rain);

  // -------- DETECTION --------
  if (irValue == LOW)
  {
    Serial.println("\n>>> OBJECT DETECTED <<<");

    if (metal == LOW)
    {
      Serial.println("METAL");
      moveTo(metalPos);
    }
    else if (rain > 2000)
    {
      Serial.println("dry");
      moveTo(wetPos);
    }
    else
    {
      Serial.println("Wet");
      moveTo(dryPos);
    }

    openFlap();

    delay(2000);  // prevent repeat
  }

  delay(500);
}