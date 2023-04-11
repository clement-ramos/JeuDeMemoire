#include <LiquidCrystal.h>

#define BUZZER 3
#define JOYSTICK_X A0
#define JOYSTICK_Y A1

LiquidCrystal LCD(13, 12, 4, 5, 6, 7);

const int LED[4] = { 11, 10, 9, 8 };
const int LCD_NB_ROWS = 2;
const int LCD_NB_COLUMNS = 16;
const int FREQUENCY = 255;

int levelCombination[50];
int combinationPlayed[50];
int xMap, yMap, xValue, yValue;
int step = 0;
int level = 0;
int inputNumber = 0;

int moveJoystick() {
  xValue = analogRead(JOYSTICK_X);
  yValue = analogRead(JOYSTICK_Y);
  xMap = map(xValue, 0, 1023, 0, 10);
  yMap = map(yValue, 0, 1023, 10, 0);

  if (xMap < 5) {
    // LED[0] GREEN 11
    return 11;
  } else if (5 < xMap) {
    // LED[3] YELLOW 8
    return 8;
  }

  if (yMap < 5) {
    // LED[2] RED 9
    return 9;
  } else if (5 < yMap) {
    // LED[1] BLUE 10
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
        step = 1;
      }
      break;

    case 1:
      LCD.setCursor(0, 0);
      LCD.print("Level: ");
      LCD.setCursor(7, 0);
      LCD.print(level);
      LCD.setCursor(0, 1);
      LCD.print("  * Memorise *  ");
      delay(500);

      levelCombination[level] = randomLed;
      for (int i = 0; i <= level; i++) {
        digitalWrite(levelCombination[i], HIGH);
        delay(400);
        digitalWrite(levelCombination[i], LOW);
        delay(400);
      }
      step = 2;
      break;

    case 2:
      LCD.setCursor(0, 1);
      LCD.print("    * Play *    ");

      for (int i = 0; i < 4; i++) {
        if (moveJoystick() != 0) {
          combinationPlayed[level] = moveJoystick();
          digitalWrite(combinationPlayed[level], HIGH);
          delay(400);
          digitalWrite(combinationPlayed[level], LOW);

          inputNumber++;

          if (inputNumber - 1 == level) {
            inputNumber = 0;
            step = 3;
            break;
          }
        }
      }
      break;

    case 3:
      LCD.setCursor(0, 1);
      LCD.print("* Verification *");
      delay(1000);

      for (int i = 0; i <= level; i++) {
        if (levelCombination[i] != combinationPlayed[i]) {
          step = 4;
        } else {
          step = 5;
        }
      }
      break;

    case 4:
      LCD.setCursor(0, 1);
      LCD.print("   *  LOST  *   ");

      tone(BUZZER, FREQUENCY);
      delay(500);
      noTone(BUZZER);

      level = 0;
      step = 0;
      delay(1000);
      break;

    case 5:
      LCD.setCursor(0, 1);
      LCD.print("    *  WIN  *    ");
      level++;
      step = 1;
      delay(1000);
      break;

    default:
      break;
  }
}
