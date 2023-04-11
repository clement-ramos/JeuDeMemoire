#include <LiquidCrystal.h>

#define joyX A0
#define joyY A1

LiquidCrystal LCD(13, 12, 4, 5, 6, 7);

const int LED[4] = { 11, 10, 9, 8 };
const int BUTTON[5] = { 6, 5, 4, 3, 2 };

const int LCD_NB_ROWS = 2;
const int LCD_NB_COLUMNS = 16;

int xMap, yMap, xValue, yValue;
int step, level;

boolean EtatBt[4] = { 0, 0, 0, 0 };
boolean winOrLost;

int levelCombination[50];
int combinationPlayed[50];

int moveJoystick() {
  xValue = analogRead(joyX);
  yValue = analogRead(joyY);
  xMap = map(xValue, 0, 1023, 0, 10);
  yMap = map(yValue, 0, 1023, 10, 0);

  if (xMap < 5) {
    return 11;
  } else if (5 < xMap) {
    return 8;
  }

  if (yMap < 5) {
    return 9;
  } else if (5 < yMap) {
    return 10;
  }

  return 0;
}

void setup() {
  LCD.begin(LCD_NB_COLUMNS, LCD_NB_ROWS);

  for (int i = 0; i < 4; i++) {
    // LED[0] GREEN 11
    // LED[1] BLUE 10
    // LED[2] RED 9
    // LED[3] YELLOW 8
    pinMode(LED[i], OUTPUT);
    digitalWrite(LED[i], LOW);
  }

  Serial.begin(9600);
}

void loop() {
  int randomLed = random(8, 12);
  switch (step) {
    case 0:
      LCD.setCursor(0, 0);
      LCD.print("* Memory  Game *");
      LCD.setCursor(1, 1);
      LCD.print("Move  Joystick");

      if (moveJoystick() != 0) {
        LCD.clear();
        step++;
      }
      break;
      
    case 1:
      LCD.setCursor(0, 0);
      LCD.print("Level: ");
      LCD.setCursor(7, 0);
      LCD.print(level);
      LCD.setCursor(0, 1);
      LCD.print("  * Memorise *  ");
      delay(2000);

      levelCombination[level] = randomLed;
      for (int i = 0; i <= level; i++) {
        digitalWrite(levelCombination[i], HIGH);
        delay(400);
        digitalWrite(levelCombination[i], LOW);
        delay(400);
      }
      step++;
      break;

    case 2:
      LCD.setCursor(0, 1);
      LCD.print("    * Play *    ");
      delay(500);

      for (int i = 0; i < 4; i++) {
        if (moveJoystick() != 0) {
          combinationPlayed[level] = moveJoystick();
          digitalWrite(combinationPlayed[level], HIGH);
          delay(500);
          digitalWrite(combinationPlayed[level], LOW);
          step++;
        }
      }
      break;

    case 3:
      LCD.setCursor(0, 1);
      LCD.print("* Verification *");
      delay(1000);

      for (int i = 0; i <= level; i++) {
        if (levelCombination[i] != combinationPlayed[i]) {
          Serial.print("levelCombination: ");
          Serial.println(levelCombination[i]);
          Serial.print("combinationPlayed: ");
          Serial.println(combinationPlayed[i]);
          winOrLost = 1;
          break;
        }
      }

      if (winOrLost == 1) {
        step = 4;
      } else {
        step = 5;
      }
      break;

    case 4:
      LCD.setCursor(0, 1);
      LCD.print("   *  LOST  *   ");
      delay(500);
      break;
      
    case 5:
      LCD.setCursor(0, 1);
      LCD.print("    *  WIN  *    ");
      delay(500);
      break;

    default:
      break;
  }
}
