//week5
#include <Arduino.h>

#define BUZZER_PIN  27
#define PWM_CH      0
#define RESOLUTION  10

void playBeat(int freq, int onTime, int offTime) {
  ledcWriteTone(PWM_CH, freq);
  delay(onTime);
  ledcWriteTone(PWM_CH, 0);
  delay(offTime);
}

void setup() {
  ledcSetup(PWM_CH, 2000, RESOLUTION);
  ledcAttachPin(BUZZER_PIN, PWM_CH);

  // --- Beat Pattern ---
  for (int i = 0; i < 4; i++) {
    playBeat(1000, 100, 100);  // Kick
    playBeat(800, 80, 80);     // Snare-like blip
  }

  delay(300);

  for (int i = 0; i < 3; i++) {
    playBeat(1200, 150, 100);  // Accent
    playBeat(900, 80, 80);     // Beat hit
  }

  delay(400);

  // --- Drop-style finish ---
  for (int f = 2000; f >= 400; f -= 100) {
    ledcWriteTone(PWM_CH, f);
    delay(20);
  }
  ledcWriteTone(PWM_CH, 0);
}

void loop() {
  // Repeat beat every few seconds
  delay(1000);

  for (int i = 0; i < 8; i++) {
    playBeat(1000, 120, 100);
    playBeat(800, 80, 80);
  }

  delay(500);
}
