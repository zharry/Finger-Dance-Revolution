#include <CountUpDownTimer.h>

CountUpDownTimer T(UP, HIGH); 

// False = Low, True = High
bool last[10];
unsigned long lastTime = 0;
unsigned long curTime = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 2; i <= 9; i++)
    pinMode(i, INPUT_PULLUP);
  T.SetStopTime(7200);
  T.StartTimer();
  Serial.println("START!");
}

void loop() {
  T.Timer(); // run the timer
  
  for (int i = 2; i < 10; i++) {
    if (!digitalRead(i) == HIGH) {
      if (!last[i]) {
  unsigned long delta = curTime - lastTime;
  lastTime = curTime;
  curTime = T.ShowMilliSeconds();
        last[i] = true;
        Serial.print(i);
        Serial.print(":");
        Serial.print(curTime);
        Serial.print("+-");
        Serial.println(delta);
      }
    } else
    last[i] = false;
  }
  
  delay(1000/250);
}
