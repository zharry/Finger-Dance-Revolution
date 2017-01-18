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
const int songSize = 9;
/**const uint32_t song[][songSize] PROGMEM = { 
  {11750, 16350, 20950, 25550, 26700, 31300, 35900, 40500, 45100},     // 2 & 6
                                        {9450, 14050, 18650, 23250, 27850, 32450, 37050, 41650, 46000},     // 3 & 7
                                        {10600, 15200, 19800, 24400, 29000, 33600, 38200, 42800, 46000},   // 4 & 8
                                                                                          {12900, 17500, 22100, 26700, 30150, 34750, 39350, 43950, 47400} }; // 5 & 9**/
const uint32_t song[][songSize] PROGMEM = { 
  {11720, 16260, 20800, 25340, 26475, 31015, 35560, 40130, 44710},     // 2 & 6
  {9450, 13990, 18530, 23070, 27610, 32150, 36700, 41300, 45850},     // 3 & 7
  {10585, 15125, 19665, 24205, 28745, 33285, 37840, 42330, 45850},   // 4 & 8
  {12855, 17395, 21935, 26475, 29880, 34420, 38990, 43570, 46990} }; // 5 & 9
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
    
  // Player 1
  for (int i = 2; i < 6; i++) {
    int a = i - 2;
    if (nextNote[i] >= songSize) {
      if (!doneNote[i]) {
        songOver++;
        doneNote[i] = true;
      }
    } else { 
      unsigned long curNote = pgm_read_dword(&song[a][nextNote[i]]);
  
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
            scoreP1 += ppc * 2;
          } else if (deltaVal <= fullPoints) {
            Serial.print("FPA:");
            Serial.print(i);
            Serial.print(",0+");
            scoreP1 += ppc;
          } else if (deltaVal <= halfPoints) {
            Serial.print("HPA:");
            Serial.print(i);
            Serial.print(",0+");
            scoreP1 += ppc / 2;
          } else if (deltaVal <= noPoints) {
            Serial.print("NPA:");
            Serial.print(i);
            Serial.print(",0+");
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
    if (nextNote[i] >= songSize) {
      if (!doneNote[i]) {
        songOver++;
        doneNote[i] = true;
      }
    } else { 
      unsigned long curNote = pgm_read_dword(&song[a][nextNote[i]]);
  
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
            scoreP2 += ppc * 2;
          } else if (deltaVal <= fullPoints) {
            Serial.print("FPA:");
            Serial.print(i);
            Serial.print(",0+");
            scoreP2 += ppc;
          } else if (deltaVal <= halfPoints) {
            Serial.print("HPA:");
            Serial.print(i);
            Serial.print(",0+");
            scoreP2 += ppc / 2;
          } else if (deltaVal <= noPoints) {
            Serial.print("NPA:");
            Serial.print(i);
            Serial.print(",0+");
          } else {
            // Not avaliable yet
            nextNote[i]--;
          }
        }
      } else
        last[i] = false; 
    }
  }
  
  for (int y = 0; y < 4; y++) {
    for (int z = 0; z < songSize; z++) {
      unsigned long curNote = pgm_read_dword(&song[y][z]);
      long delta = curTime - curNote;
      if (abs(delta) <= 500) {
        for (int i = 0; i <= 4; i += 4) {
          if (y == 0) {
            Serial.print("L:");
            Serial.print(2 + i);
          } else if (y == 1) {
            Serial.print("T:");
            Serial.print(3 + i);
          } else if (y == 2) {
            Serial.print("B:");
            Serial.print(4 + i);
          } else if (y == 3) {
            Serial.print("R:");
            Serial.print(5 + i);
          }
          Serial.print(",");
          Serial.print(map(delta, -500, 500, 480, -31));
          Serial.print("+");
        }
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

  if (songOver >= 8) {
    Serial.print("SO:");
    Serial.print(scoreP1);
    Serial.print(",");
    Serial.println(scoreP2);
    while (true)
      delay(1000);
  }
  Serial.println();

  delay(1000 / 250);
}
