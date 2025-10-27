// =======================================
// Assignment-01_Task-B (Simplified)
// LED and Buzzer with Short/Long Press
// Embedded IoT System Fall-2025
// =======================================
//
// Name: M Talha Iftikhar
// Reg#: 23-NTU-CS-1075
//

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ======== Pin Definitions ========
#define LED_PIN     18
#define BUZZER_PIN  14
#define BTN_PIN     25

// ======== OLED Setup ========
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

// ======== Variables ========
bool ledState = LOW;
bool buttonPressed = false;
unsigned long pressStartTime = 0;
const unsigned long longPressTime = 1500; // 1.5 seconds debounce for long press

// ======== Display Helper ========
void showMessage(const char* msg) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(msg);
  display.display();
}

// ======== Setup ========
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);  // internal pull-up: LOW = pressed

  digitalWrite(LED_PIN, ledState);
  digitalWrite(BUZZER_PIN, LOW);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;); // Stop if display not found
  }
  showMessage("Ready...");
}

// ======== Main Loop ========
void loop() {
  bool isPressed = (digitalRead(BTN_PIN) == LOW);
  unsigned long currentTime = millis();

  // Button just pressed
  if (isPressed && !buttonPressed) {
    buttonPressed = true;
    pressStartTime = currentTime;
  }

  // Button just released
  if (!isPressed && buttonPressed) {
    unsigned long pressDuration = currentTime - pressStartTime;
    buttonPressed = false; // reset for next cycle

    if (pressDuration >= longPressTime) {
      // ===== Long Press Detected =====
      digitalWrite(LED_PIN, LOW);               // Turn off LED
      tone(BUZZER_PIN, 1000, 200);              // 1kHz beep for 200ms
      showMessage("Long press detected!");
    } else {
      // ===== Short Press Detected =====
      ledState = !ledState;                     // Toggle LED
      digitalWrite(LED_PIN, ledState);
      showMessage("Short press detected!");
    }
  }
}
