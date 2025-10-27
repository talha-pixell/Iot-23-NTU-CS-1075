// ================================
// Assignment-01_Task-A (Simplified)
// 3 LEDs and 2 Buttons with OLED Display
// Same functionality, simpler logic
// ================================

// Name: M Talha Iftikhar
// Reg#: 23-NTU-CS-1075

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ======== Pin Definitions ========
#define LED1 19
#define LED2 18
#define LED3 5
#define BTN_MODE 34
#define BTN_RESET 35

// ======== Variables ========
int mode = 0;
int brightness = 0;
int fadeAmount = 15;
bool ledState = LOW;

// For blinking
unsigned long prevMillisBlink = 0;
unsigned long prevMillisFade = 0;

// For debounce
unsigned long lastModePress = 0;
unsigned long lastResetPress = 0;
const unsigned long debounceDelay = 200;

// ======== OLED Display Function ========
void showMode(const char* text) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  int x = (SCREEN_WIDTH - strlen(text) * 12) / 2;
  int y = (SCREEN_HEIGHT - 16) / 2;
  display.setCursor(x, y);
  display.print(text);
  display.display();
}

// ======== Setup ========
void setup() {
  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;); // hang if display not found
  }

  showMode("Mode 0: OFF");

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);

  // Setup PWM for LED3
  ledcSetup(0, 5000, 8);  // channel 0, freq 5kHz, 8-bit
  ledcAttachPin(LED3, 0);
}

// ======== LOOP ========
void loop() {
  unsigned long currentMillis = millis();

  // ---------- Button: MODE ----------
  if (digitalRead(BTN_MODE) == LOW && (currentMillis - lastModePress > debounceDelay)) {
    lastModePress = currentMillis;
    mode++;
    if (mode > 4) mode = 0;
    delay(50); // small delay for stable press
  }

  // ---------- Button: RESET ----------
  if (digitalRead(BTN_RESET) == LOW && (currentMillis - lastResetPress > debounceDelay)) {
    lastResetPress = currentMillis;
    mode = 0;
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    ledcWrite(0, 0);
    showMode("Default");
    delay(200); // give visual feedback delay
  }

  // ---------- LED Modes ----------
  switch (mode) {
    case 0:  // Default OFF
      showMode("Default");
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      ledcWrite(0, 0);
      break;

    case 1:  // Both OFF
      showMode("Both OFF");
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      ledcWrite(0, 0);
      break;

    case 2:  // Blinking
      showMode("Blinking");
      if (currentMillis - prevMillisBlink >= 500) {
        prevMillisBlink = currentMillis;
        ledState = !ledState;
        digitalWrite(LED1, ledState);
        digitalWrite(LED2, !ledState);
      }
      break;

    case 3:  // Both ON
      showMode("Both ON");
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      ledcWrite(0, 0);
      break;

    case 4:  // PWM Fade
      showMode("PWM");
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      if (currentMillis - prevMillisFade >= 2) {
        prevMillisFade = currentMillis;
        ledcWrite(0, brightness);
        brightness += fadeAmount;
        if (brightness <= 0 || brightness >= 255)
          fadeAmount = -fadeAmount;
      }
      break;
  }
}
