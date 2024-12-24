#include <LiquidCrystal.h>

LiquidCrystal lcd(10, 6, 5, 4, 3, 2);

int ptsP1 = 0, ptsP2 = 0;

const int buttonAddPts = 7;
int lastButtonAddPtsState = LOW;
const int buttonSubPts = 11;
int lastButtonSubPtsState = LOW;

const int buttonPlayer = 8;
int lastButtonPlayerState = LOW;

const int buttonReset = 9;

int player = 0;

int speaker = 12;

#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_C6  1047
#define REST      0

int tempo = 120;

int melody[] = {
  NOTE_G5,22, NOTE_FS5,28, NOTE_DS5,28, NOTE_A4,22,
  NOTE_GS4,28, NOTE_E5,22, NOTE_GS5,28, NOTE_C6,18
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;

unsigned long lastMillis = 0;  // Variable for controlling the timing of the melody
bool melodyPlaying = false;    // Flag to track whether the melody is playing
int melodyNoteIndex = 0;       // Index for tracking the current note in the melody

void gameInit(void);

void setup() {
  lcd.begin(16, 2);
  delay(100);
  pinMode(speaker, OUTPUT);
  gameInit();  
}

void gameInit(void) {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("SCOREBOARD");

  unsigned long previousMillis = 0;
  unsigned long interval = 80;
  int dotPosition = 0;

  unsigned long currentMillis = millis();
  while (dotPosition <= 15 ) {
    currentMillis = millis();

    if (currentMillis - previousMillis >= interval && dotPosition <= 15) {
      previousMillis = currentMillis;
      lcd.setCursor(dotPosition, 1);
      lcd.print(".");
      delay(80);
      lcd.setCursor(dotPosition, 1);
      lcd.print(" ");
      dotPosition++;
    }

  }

  delay(200);
  lcd.clear();
}

bool buttonPressed(int buttonPin, int &lastState) {
    int currentState = digitalRead(buttonPin);
    if (currentState != lastState) {
        lastState = currentState; 
        return currentState == HIGH;
    }
    return false;
}

void playNewGameTune(int speaker) {
  unsigned long currentMillis = millis();

  if (melodyNoteIndex < notes) {
    if (currentMillis - lastMillis >= noteDuration) {
      tone(speaker, melody[melodyNoteIndex * 2], noteDuration * 0.9);
      lastMillis = currentMillis;
      melodyNoteIndex++;
    }
  } else {
    noTone(speaker);
  }
}

void loop() {

  if (buttonPressed(buttonPlayer, lastButtonPlayerState)) {
      player = (player == 0) ? 1 : 0;
        tone(speaker,1500,50);
        delay(100);
        tone(speaker,2000,50);
    }  

  if (buttonPressed(buttonAddPts, lastButtonAddPtsState)) {
      if (player == 0) {
        tone(speaker,1000,25);
        ptsP1 += 1;
      } else {
        tone(speaker,1000,25);
        ptsP2 += 1;
      }
    }
  
  if (buttonPressed(buttonSubPts, lastButtonSubPtsState)) {
      if (player == 0) {
        if (ptsP1 > 0) {
          tone(speaker,100,50);
          ptsP1 -= 1;
        }
      } else {
        if (ptsP2 > 0) {
          tone(speaker,100,50);
          ptsP2 -= 1;
        }
      }
    }

  if (digitalRead(buttonReset) == HIGH) {
    melodyNoteIndex = 0;
    playNewGameTune(speaker);

    int j = 0;
    for (int i = 0; i < 9; i++) {
      lcd.clear();
      if (i % 2 == 0) {
        j = 0;
      } else {
        j = 1;
      }
      lcd.setCursor(i, j);
      lcd.print("NEW GAME");
      delay(140);

      playNewGameTune(speaker);
    }

    player = 0;
    ptsP1 = 0;
    ptsP2 = 0;

    gameInit();

    while (digitalRead(buttonReset) == HIGH) {
      delay(10);
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PLAYER 1 - ");
  lcd.print(ptsP1);

  if (player == 0) {
    lcd.setCursor(15, 0);
    lcd.print("<");
    lcd.setCursor(15, 1);
    lcd.print(" ");
  } else {
    lcd.setCursor(15, 0);
    lcd.print(" ");
    lcd.setCursor(15, 1);
    lcd.print("<");
  }

  lcd.setCursor(0, 1);
  lcd.print("PLAYER 2 - ");
  lcd.print(ptsP2);

  delay(10);
}
