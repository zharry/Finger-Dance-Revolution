#include <CountUpDownTimer.h>

CountUpDownTimer T(UP, HIGH); 

// False = Low, True = High
bool last[10];

// Game Constants
const int doubPoints = 25;
const int fullPoints = 100;
const int halfPoints = 250;
const int noPoints = 500; 
const int ppc = 100; // Points Per Click

// Songs
int testSong[] = {2500, 5000, 7500, 10000, 12500, 15000, 17500};
int nextNote = 0;
int score = 0;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  Serial.println("------");
    
  for (int i = 2; i <= 9; i++)
    pinMode(i, INPUT_PULLUP);
    
  T.SetStopTime(7200);
  T.StartTimer();
}

void loop() {
  digitalWrite(13, HIGH);
  T.Timer(); // run the timer
    
  if (nextNote >= (sizeof(testSong) / sizeof(int))) {
    Serial.println("SO"); // Song Over
    Serial.print("Final Score: ");
    Serial.println(score, DEC);
    while (true)
      delay(1000);

  } else { 
    int curTime = T.ShowMilliSeconds();

    // If the player missed the current note
    if (testSong[nextNote] + noPoints < curTime) {
      Serial.print("MN:"); // Missed Note
      Serial.print(nextNote, DEC);
      Serial.print(" T:"); // Expected Time
      Serial.print(testSong[nextNote], DEC);        
      Serial.print(" @ "); // Current Time
      Serial.println(curTime, DEC);
      nextNote++;
    }
    
    if (!digitalRead(8) == HIGH) {
      if (!last[8]) {
        last[8] = true;
        int delta = -(testSong[nextNote] - curTime);
        int deltaVal = abs(delta);
        
        Serial.print("CN:"); // Current Note
        Serial.print(nextNote, DEC);
        Serial.print(" T:"); // Expected Time
        Serial.print(testSong[nextNote], DEC);        
        Serial.print(" @ "); // Current Time
        Serial.print(curTime, DEC);
        Serial.print(delta >= 0 ? " +" : " ");
        Serial.print(delta, DEC); // Time Difference
        
        nextNote++;
        if (deltaVal <= doubPoints) {
          Serial.print(" DPA "); // Double Points Awarded
          score += ppc * 2;
        } else if (deltaVal <= fullPoints) {
          Serial.print(" FPA "); // Full Points Awarded
          score += ppc;
        } else if (deltaVal <= halfPoints) {
          Serial.print(" HPA "); // Half Points Awarded
          score += ppc / 2;
        } else if (deltaVal <= noPoints) {
          Serial.print(" NPA "); // No Points (Too Early/Late) Awarded
        } else {
          Serial.print(" NAY "); // Not avaliable yet
          nextNote--;
        }
      
        Serial.println();
      }
    } else
      last[8] = false; 
  }
  
  delay(1000 / 250);
}
