#include <CountUpDownTimer.h>
#include <avr/pgmspace.h>

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
const int testSongSize = 3;
const uint32_t testSong[][testSongSize] PROGMEM = { 
  {2000, 4000, 18000},     // 2 & 6
  {6000, 8000, 20000},     // 3 & 7
  {10000, 12000, 18000},   // 4 & 8
  {14000, 16000, 34000} }; // 5 & 9
unsigned int nextNote[10];
bool doneNote[10];
unsigned int scoreP1 = 0;
unsigned int scoreP2 = 0;
unsigned int songOver = 0;

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  Serial.println("SS:0,0+");
    
  for (int i = 2; i <= 9; i++)
    pinMode(i, INPUT_PULLUP);
    
  T.SetStopTime(7200);
  T.StartTimer();
}

void loop() {
  digitalWrite(13, HIGH);
  T.Timer(); // run the timer
  unsigned long curTime = T.ShowMilliSeconds();

  if (songOver >= 8) {
    Serial.print("SO:");
    Serial.print(scoreP1);
    Serial.print(",");
    Serial.print(scoreP2);
    while (true)
      delay(1000);
  }
  
  for (int y = 0; y < 4; y++) {
    for (int z = 0; z < testSongSize; z++) {
      unsigned long curNote = pgm_read_dword(&testSong[y][z]);
      long delta = curTime - curNote;
      if (abs(delta) <= 500) {
        if (y == 0) {
          Serial.print("L:");
          Serial.print(2);
        } else if (y == 1) {
          Serial.print("T:");
          Serial.print(3);
        } else if (y == 2) {
          Serial.print("B:");
          Serial.print(4);
        } else if (y == 3) {
          Serial.print("R:");
          Serial.print(5);
        }
        Serial.print(",");
        Serial.print(map(delta, -500, 500, 451, -31));
        Serial.print("+");
      }
    }
  }
  Serial.print("CT:");
  Serial.print(curTime);
  Serial.print(",0+SC:");
  Serial.print(scoreP1);
  Serial.print(",");
  Serial.print(scoreP2);
  Serial.print("+");
  Serial.println();

    
  // Player 1
  for (int i = 2; i < 6; i++) {
    int a = i - 2;
    if (nextNote[i] >= testSongSize) {
      if (!doneNote[i]) {
        songOver++;
        doneNote[i] = true;
      }
    } else { 
      unsigned long curNote = pgm_read_dword(&testSong[a][nextNote[i]]);
  
      // If the player missed the current note
      if (curNote + noPoints < curTime) {
        
        //displayDebug(i, a, curNote, curTime);
        Serial.print("MN:");
        Serial.print(i);
        Serial.print(",0+");
        nextNote[i]++;
      }
      
      if (!digitalRead(i) == HIGH) {
        if (!last[i]) {
          last[i] = true;
          long delta = -(curNote - curTime);
          long deltaVal = abs(delta);
          
          nextNote[i]++;
          if (deltaVal <= doubPoints) {
            Serial.print("DPA:");
            Serial.print(i);
            Serial.print(",0+");
            Serial.println();
            scoreP1 += ppc * 2;
          } else if (deltaVal <= fullPoints) {
            Serial.print("FPA:");
            Serial.print(i);
            Serial.print(",0+");
            Serial.println();
            scoreP1 += ppc;
          } else if (deltaVal <= halfPoints) {
            Serial.print("HPA:");
            Serial.print(i);
            Serial.print(",0+");
            Serial.println();
            scoreP1 += ppc / 2;
          } else if (deltaVal <= noPoints) {
            Serial.print("NPA:");
            Serial.print(i);
            Serial.print(",0+");
            Serial.println();
          } else {
            // Not avaliable yet
            nextNote[i]--;
          }
        }
      } else
        last[i] = false; 
    }
  }

  // Player 2
  for (int i = 6; i < 10; i++) {
    int a = i - 6;
    if (nextNote[i] >= testSongSize) {
      if (!doneNote[i]) {
        songOver++;
        doneNote[i] = true;
      }
    } else { 
      unsigned long curNote = pgm_read_dword(&testSong[a][nextNote[i]]);
  
      // If the player missed the current note
      if (curNote + noPoints < curTime) {
        
        //displayDebug(i, a, curNote, curTime);
        Serial.print("MN:");
        Serial.print(i);
        Serial.print(",0+");
        nextNote[i]++;
      }
      
      if (!digitalRead(i) == HIGH) {
        if (!last[i]) {
          last[i] = true;
          long delta = -(curNote - curTime);
          long deltaVal = abs(delta);
          
          nextNote[i]++;
          if (deltaVal <= doubPoints) {
            Serial.print("DPA:");
            Serial.print(i);
            Serial.print(",0+");
            Serial.println();
            scoreP2 += ppc * 2;
          } else if (deltaVal <= fullPoints) {
            Serial.print("FPA:");
            Serial.print(i);
            Serial.print(",0+");
            Serial.println();
            scoreP2 += ppc;
          } else if (deltaVal <= halfPoints) {
            Serial.print("HPA:");
            Serial.print(i);
            Serial.print(",0+");
            Serial.println();
            scoreP2 += ppc / 2;
          } else if (deltaVal <= noPoints) {
            Serial.print("NPA:");
            Serial.print(i);
            Serial.print(",0+");
            Serial.println();
          } else {
            // Not avaliable yet
            nextNote[i]--;
          }
        }
      } else
        last[i] = false; 
    }
  }
  
  delay(1000 / 250);
}

void displayDebug(int i, int a, unsigned long curNote, unsigned long curTime) {
  Serial.print(" CB:"); // Current Button
  Serial.print(i);
  Serial.print(" CN:"); // Current Note
  Serial.print(nextNote[i]);
  Serial.print(" T:"); // Expected Time
  Serial.print(curNote);        
  Serial.print(" @ "); // Current Time
  Serial.print(curTime);
}

	
