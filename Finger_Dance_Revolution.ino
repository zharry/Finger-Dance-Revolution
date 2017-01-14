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
int testSong[][2] = { {}, {}, {}, {}, {}, {}, 
  {2000, 4000}, //6
  {6000, 8000}, //7
  {8000, 10000}, //8
  {4000, 12000} }; //9
int nextNote[10];
bool doneNote[10];
int score = 0;
int songOver = 0;

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

  for (int i = 6; i < 10; i++) {
    if (nextNote[i] >= (sizeof(testSong[i]) / sizeof(int))) {
      if (!doneNote[i]) {
        songOver++;
        doneNote[i] = true;
      }
      if (songOver >= 4) {
        Serial.println("SO"); //Song Over
        Serial.print("Final Score: ");
        Serial.println(score, DEC);
        while (true)
          delay(1000);
      }
    } else { 
      int curTime = T.ShowMilliSeconds();
  
      // If the player missed the current note
      if (testSong[i][nextNote[i]] + noPoints < curTime) {
        
        displayDebug(i, curTime);
        Serial.println(" MN"); // Missed Note
        nextNote[i]++;
      }
      
      if (!digitalRead(i) == HIGH) {
        if (!last[i]) {
          last[i] = true;
          int delta = -(testSong[i][nextNote[i]] - curTime);
          int deltaVal = abs(delta);
          
          displayDebug(i, curTime);
          Serial.print(delta >= 0 ? " +" : " ");
          Serial.print(delta, DEC); // Time Difference
          
          nextNote[i]++;
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
            nextNote[i]--;
          }
        
          Serial.println();
        }
      } else
        last[i] = false; 
    }
  }
  
  delay(1000 / 250);
}

void displayDebug(int i, int curTime) {
  Serial.print(" CB:"); // Current Button
  Serial.print(i, DEC);
  Serial.print(" CN:"); // Current Note
  Serial.print(nextNote[i], DEC);
  Serial.print(" T:"); // Expected Time
  Serial.print(testSong[i][nextNote[i]], DEC);        
  Serial.print(" @ "); // Current Time
  Serial.print(curTime, DEC);
}

