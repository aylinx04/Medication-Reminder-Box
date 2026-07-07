#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <ThreeWire.h>
#include <RtcDS1302.h>

//PINS
const byte BUTTON_PIN = 2;
const byte BUZZER_PIN = A3;

//Medikamenten-LEDs
const byte MORNING_LED = 13;
const byte NOON_LED    = 12;
const byte EVENING_LED = 11;

//ALARMZEITEN
const byte morningHour   = 11;
const byte morningMinute = 40;

const byte noonHour      = 11;
const byte noonMinute    = 41;

const byte eveningHour   = 11;
const byte eveningMinute = 42;

//RTC
ThreeWire myWire(7, 6, 8); // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);

//LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//ALARM
bool alarmActive = false;
unsigned long lastBlink = 0;
int lastAlarmMinute = -1;

enum AlarmType
{
    NONE,
    MORNING,
    NOON,
    EVENING
};

AlarmType currentAlarm = NONE;

void setup()
{
    Serial.begin(9600);

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);

    pinMode(MORNING_LED, OUTPUT);
    pinMode(NOON_LED, OUTPUT);
    pinMode(EVENING_LED, OUTPUT);

    digitalWrite(MORNING_LED, LOW);
    digitalWrite(NOON_LED, LOW);
    digitalWrite(EVENING_LED, LOW);

    lcd.init();
    lcd.backlight();

    Rtc.Begin();

    if (!Rtc.GetIsRunning())
    {
        Rtc.SetIsRunning(true);
    }

    if (!Rtc.IsDateTimeValid())
    {
        Rtc.SetDateTime(RtcDateTime(__DATE__, __TIME__));
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System bereit");

    delay(2000);
    lcd.clear();
}

void loop()
{
    RtcDateTime nowRtc = Rtc.GetDateTime();

    // Uhrzeit nur anzeigen wenn kein Alarm aktiv
    if (!alarmActive)
    {
        lcd.setCursor(0, 0);

        if (nowRtc.Hour() < 10) lcd.print("0");
        lcd.print(nowRtc.Hour());
        lcd.print(":");

        if (nowRtc.Minute() < 10) lcd.print("0");
        lcd.print(nowRtc.Minute());
        lcd.print(":");

        if (nowRtc.Second() < 10) lcd.print("0");
        lcd.print(nowRtc.Second());

        lcd.print("   ");
    }

    //ALARMZEITEN
    bool alarmTime =
        (nowRtc.Hour() == morningHour &&
         nowRtc.Minute() == morningMinute) ||

        (nowRtc.Hour() == noonHour &&
         nowRtc.Minute() == noonMinute) ||

        (nowRtc.Hour() == eveningHour &&
         nowRtc.Minute() == eveningMinute);

    if (alarmTime && lastAlarmMinute != nowRtc.Minute())
    {
        alarmActive = true;
        lastAlarmMinute = nowRtc.Minute();

        if (nowRtc.Hour() == morningHour &&
            nowRtc.Minute() == morningMinute)
        {
            currentAlarm = MORNING;
        }
        else if (nowRtc.Hour() == noonHour &&
                 nowRtc.Minute() == noonMinute)
        {
            currentAlarm = NOON;
        }
        else if (nowRtc.Hour() == eveningHour &&
                 nowRtc.Minute() == eveningMinute)
        {
            currentAlarm = EVENING;
        }

        lcd.clear();
        lcd.setCursor(0, 0);

        if (currentAlarm == MORNING)
        {
            lcd.print("MORGEN");
        }
        else if (currentAlarm == NOON)
        {
            lcd.print("MITTAG");
        }
        else if (currentAlarm == EVENING)
        {
            lcd.print("ABEND");
        }

        lcd.setCursor(0, 1);
        lcd.print("Medikament!");
    }

    //ALARM AKTIV
    if (alarmActive)
    {
        unsigned long nowMillis = millis();

        if (nowMillis - lastBlink >= 300)
        {
            lastBlink = nowMillis;

            static bool blinkState = false;
            blinkState = !blinkState;

            digitalWrite(MORNING_LED, LOW);
            digitalWrite(NOON_LED, LOW);
            digitalWrite(EVENING_LED, LOW);

            if (currentAlarm == MORNING)
            {
                digitalWrite(MORNING_LED, blinkState);
            }
            else if (currentAlarm == NOON)
            {
                digitalWrite(NOON_LED, blinkState);
            }
            else if (currentAlarm == EVENING)
            {
                digitalWrite(EVENING_LED, blinkState);
            }

            if (blinkState)
            {
                tone(BUZZER_PIN, 1000);
            }
            else
            {
                noTone(BUZZER_PIN);
            }
        }

        // Taster gedrückt
        if (digitalRead(BUTTON_PIN) == LOW)
        {
            delay(30);

            if (digitalRead(BUTTON_PIN) == LOW)
            {
                alarmActive = false;
                currentAlarm = NONE;

                digitalWrite(MORNING_LED, LOW);
                digitalWrite(NOON_LED, LOW);
                digitalWrite(EVENING_LED, LOW);

                noTone(BUZZER_PIN);

                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Einnahme OK");

                while (digitalRead(BUTTON_PIN) == LOW)
                {
                    delay(10);
                }

                delay(1000);
                lcd.clear();
            }
        }
    }

    delay(100);
}