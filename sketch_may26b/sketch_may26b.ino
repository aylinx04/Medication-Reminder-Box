#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const byte LED_PIN = 13;
const byte BUTTON_PIN = 2;
const byte BUZZER_PIN = A3;

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool alarmActive = false;

unsigned long lastAlarmTime = 0;
unsigned long lastBlink = 0;

const unsigned long alarmInterval = 10000; // 10 Sekunden

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System bereit");
}

void loop() {

  unsigned long now = millis();

  // Alarm nach 10 Sekunden starten
  if (!alarmActive && now - lastAlarmTime >= alarmInterval) {

    alarmActive = true;
    lastAlarmTime = now;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MEDIKAMENT!");
    lcd.setCursor(0, 1);
    lcd.print("Taste druecken");

    Serial.println("ALARM");
  }

  // Alarm aktiv
  if (alarmActive) {

    // LED blinken + Buzzer piepen
    if (now - lastBlink >= 300) {

      lastBlink = now;

      static bool state = false;
      state = !state;

      digitalWrite(LED_PIN, state);

      if (state) {
        tone(BUZZER_PIN, 1000);
      } else {
        noTone(BUZZER_PIN);
      }
    }

    // Button druecken = Alarm stoppen
    if (digitalRead(BUTTON_PIN) == LOW) {

      delay(30); // Entprellung

      if (digitalRead(BUTTON_PIN) == LOW) {
        Serial.println("BUTTON GEDRUECKT");

        alarmActive = false;

        digitalWrite(LED_PIN, LOW);
        noTone(BUZZER_PIN);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Alarm gestoppt");

        Serial.println("Alarm gestoppt");

        // Warten bis Taste losgelassen wird
        while (digitalRead(BUTTON_PIN) == LOW) {
          delay(10);
        }

        delay(500);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Bereit...");
      }
    }
  }
}