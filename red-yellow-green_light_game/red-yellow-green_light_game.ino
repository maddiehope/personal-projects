// Define pin numbers for the LEDs and button
const int greenLED = 2;
const int yellowLED = 3;
const int redLED = 4;
const int buttonPin = 7;

// Define delay times (in milliseconds)
const int debounceDelay = 50;  // Debounce time for button
const int longPressTime = 1000;  // Time to detect a long press

// Variables to keep track of button and LED states
int currentLED = greenLED;
int lastButtonState = HIGH;
int buttonState;
unsigned long lastDebounceTime = 0;
unsigned long buttonPressStartTime = 0;

// Mode variables
bool isButtonMode = true;  // Start in button mode
unsigned long lastRandomChange = 0;
int randomChangeInterval = 0;

void setup() {
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(currentLED, HIGH);
  Serial.begin(9600);
  Serial.println("Starting in Button Mode");
}

void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        buttonPressStartTime = millis();
      } else {
        long pressDuration = millis() - buttonPressStartTime;
        if (pressDuration > longPressTime) {
          // Long press detected, switch modes
          isButtonMode = !isButtonMode;
          Serial.println(isButtonMode ? "Switched to Button Mode" : "Switched to Random Mode");
        } else if (isButtonMode) {
          // Short press in button mode, change light
          changeLight();
        }
      }
    }
  }

  if (!isButtonMode && (millis() - lastRandomChange > randomChangeInterval)) {
    changeLight();
    randomChangeInterval = random(1000, 3000);  // Random interval between 1 and 5 seconds
    lastRandomChange = millis();
  }

  lastButtonState = reading;
}

void changeLight() {
  digitalWrite(currentLED, LOW);
  if (currentLED == greenLED) {
    currentLED = yellowLED;
    Serial.println("Yellow Light");
  } else if (currentLED == yellowLED) {
    currentLED = redLED;
    Serial.println("Red Light");
  } else {
    currentLED = greenLED;
    Serial.println("Green Light");
  }
  digitalWrite(currentLED, HIGH);
}