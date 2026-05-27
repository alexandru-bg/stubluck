#include "audio.h"

const char *songs[] = {
    "NokiaTun:d=4,o=5,b=225:8e6,8d6,f#,g#,8c#6,8b,d,e,8b,8a,c#,e,2a",
    "Beep:d=30,o=5,b=300:f,c,f",
    "Start:d=16,o=6,b=300:c,c",
    "Correct:d=8,o=6,b=260:c,e,g",
    "Wrong:d=8,o=5,b=120:e,d,4c",
    "Cash:d=32,o=6,b=240:c,e,g,c7,e7,g7",
    "Over:d=4,o=5,b=100:e,d,c,2b4",
    "HiScore:d=16,o=6,b=240:c,e,g,c7,8p,c,e,g,c7,8p,4c7",
    "RockShow:d=8,o=5,b=138:a,a,c#6,e6,a6,4e6,c#6,a,b,c#6,4a,p,a,a,c#6,e6,a6,4e6,c#6,b,a,2e6"
};

void waitForBuzzerRelease()
{
  while (digitalRead(BUZZER_PIN) == LOW)
  {
  }
}

void playSound(const char *song)
{
  pinMode(BUZZER_PIN, OUTPUT);
  anyrtttl::blocking::play(BUZZER_PIN, song);
  pinMode(BUZZER_PIN, INPUT_PULLUP);
  waitForBuzzerRelease();
}
