#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mbed.h"
#include "TextLCD.h"
#include "USBSerial.h"
#include "S_Effects.h"

#define WAIT_US 1000000
#define LCD_WIDTH 16

using namespace mbed;

USBSerial pc;


BusOut rows(p20, p19, p18);
BusIn cols(p11, p10, p9, p8);

TextLCD lcd(p0, p1, p12, p13, p14, p15);


char numberToRemember[100];
int roundNum = 1;
int score = 0;
char input[21];

char Keytable[] = { 'F', 'E', 'D', 'C',
                    '3', '6', '9', 'B',
                    '2', '5', '8', '0',
                    '1', '4', '7', 'A' };

void countdown() {
  lcd.cls();
  lcd.locate(0, 0);
  lcd.printf("Round %d..", roundNum);
  for (int i = 3; i >= 1; i--) {
    lcd.locate(0, 1);
    lcd.printf("Starting in.. ");  //This function is responsible for showing what round the player is in
    lcd.printf("%d", i);           //and also allows the to user to get ready to memorise a number by seeing the countdown
    Beep();                        // There is a beep effect added everytime the countdown number decreases
    wait_us(850000);
  }
}
char getKey() {
  int i, j;
  char ch;
  for (i = 0; i <= 3; i++) {
    rows = i;
    for (j = 0; j <= 3; j++) {
      if (((cols ^ 0x00FF) & (0x0001 << j)) != 0) {  //getKey function scans the pico keypad
        ch = Keytable[(i * 4) + j];                  // to check if the user has pressed a key
        wait_us(200000);                             // also includes 200000ms debounce
        return ch;
      }
    }
  }
  return ch;
}
void getInput(int length) {

  lcd.cls();
  lcd.printf("Enter Value:");
  lcd.locate(0, 1);
  pc.printf("\nEnter Value: ");

  int index = 0;

  char key = ' ';

  do {
    key = getKey();
    if ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'F')) {
      input[index] = key;
      lcd.putc(key);
      pc.printf("%c", key);
      index++;
      wait_us(200000);
    }

  } while (index < length);

  input[length] = '\0';
}
void Restart() {  //This function gives the user the option to restart the game after they have finished
  int score;
  char playagain = ' ';
  lcd.cls();
  lcd.locate(0, 0);
  lcd.printf("Playagain?");
  lcd.locate(0, 1);
  lcd.printf("Yes(1)  No(2)");
  while (1) {  //While loop is created to show what happens if either Yes Or No is chosen
    char key = ' ';
    key = getKey();
    if (key == '1') {
      lcd.cls();
      lcd.locate(0, 0);  //if 1 is chosen they can choose a different difficulty in singleplayer mode
      roundNum = 1;
      score = 0;
      singleplayer();
    } else if (key == '2') {
      lcd.cls();
      lcd.locate(0, 0);
      lcd.printf("Thanks For");
      lcd.locate(0, 1);  //if 2 is chosen then an appreciation, then it takes them to main menu
      lcd.printf("Playing!!");
      delay(5000);
      roundNum = 1;
      score = 0;
      main();
    }
  }
}
void Restart2() {  // This Restart function is for the Multiplayer mode.
  int score;       // and the same exact procedure applies just like the singleplayer mode
  char playagain = ' ';
  lcd.cls();
  lcd.locate(0, 0);
  lcd.printf("Playagain?");
  lcd.locate(0, 1);
  lcd.printf("Yes(1)  No(2)");
  while (1) {
    char key = ' ';
    key = getKey();
    if (key == '1') {
      lcd.cls();
      lcd.locate(0, 0);
      roundNum = 1;
      score = 0;
      multiplayer();
    } else if (key == '2') {
      lcd.cls();
      lcd.locate(0, 0);
      lcd.printf("Thanks For");
      lcd.locate(0, 1);
      lcd.printf("Playing!!");
      delay(5000);
      roundNum = 1;
      score = 0;
      main();
    }
  }
}

//this function allows the user to choose the difficulty settings for the singpleplayer mode
void singleplayer() {

  lcd.cls();
  lcd.locate(0, 0);
  lcd.printf("Difficulty");
  lcd.locate(0, 1);
  lcd.printf("1:E|, 2:M|, 3:H|");

  while (1) {
    char key = ' ';
    key = getKey();
    if (key == '1') {  //Depending on which mode is selected a difficulty function is called
      lcd.cls();
      lcd.locate(0, 0);  //options 1 is for the Easy mode, 2 medium and 3 hard mode
      SinglePlayerEasy();
      pc.printf("\n%c", key);


    } else if (key == '2') {
      pc.printf("\n%c", key);
      lcd.cls();
      lcd.locate(0, 0);
      SinglePlayerMed();

    } else if (key == '3') {
      pc.printf("\n%c", key);
      lcd.cls();
      lcd.locate(0, 0);
      SinglePlayerHard();
    }
  }
}
//SinglePlayerEasy only gives a range of random numbers between 1 - 3 as specified in the sEmax and sEmin variables
void SinglePlayerEasy() {
  srand(time(NULL));
  int sEmax = 3;
  int sEmin = 1;
  int score = 0;

  while (1) {
    countdown();

    char numberToRemember[100] = { 0 };


    for (int i = 0; i < roundNum; i++) {
      numberToRemember[i] = '0' + (rand() % (sEmax - sEmin + 1) + sEmin);
    }
    numberToRemember[roundNum] = '\0';


    lcd.cls();
    lcd.printf("Memorise:");
    lcd.locate(0, 1);
    lcd.printf("%s", numberToRemember);
    delay(3000);
    


    lcd.cls();
    lcd.printf("Now..");
    delay(500);


    getInput(roundNum);


    if (strcmp(input, numberToRemember) == 0) {
      lcd.cls();
      lcd.locate(0, 0);
      lcd.printf("Well Done:)");
      lcd.locate(0, 1);
      lcd.printf("That's Right:)");
      CorrectSound();
      flashGreen();
      delay(500);
      roundNum++;
      score += 1;

    } else {
      lcd.cls();
      LooseSound();
      flashRed();
      lcd.locate(0, 0);
      lcd.printf("Oops wrong:(!");
      pc.printf("Oops wrong:(!");
      lcd.locate(0, 1);
      lcd.printf("Ans: %s", numberToRemember);
      delay(5000);
      lcd.cls();
      lcd.locate(0, 0);
      lcd.printf("Your score : %d", score);
      lcd.locate(0, 1);
      lcd.printf("Round Reached %d", roundNum);
      delay(5000);
      Restart();
    }
  }
}
//SinglePlayerEasy only gives a range of random numbers between 1 - 5 as specified in the sMmax and sMmin variables
void SinglePlayerMed() {
  int score = 0;
  srand(time(NULL));
  int sMmax = 5;
  int sMmin = 1;

  while (1) {
    countdown();

    char numberToRemember[100] = { 0 };


    for (int i = 0; i < roundNum; i++) {
      numberToRemember[i] = '0' + (rand() % (sMmax - sMmin + 1) + sMmin);  //for loop to generate the random numbers from a specific range
    }
    numberToRemember[roundNum] = '\0';


    lcd.cls();
    lcd.printf("Memorise:");
    lcd.locate(0, 1);
    lcd.printf("%s", numberToRemember);  //User memorises number for ceratin amount of  time 
    delay(2000);                          //then the number dissappears

    lcd.cls();
    lcd.printf("Now..");
    delay(500);


    getInput(roundNum);


    if (strcmp(input, numberToRemember) == 0) {  //the if statement here compares
      lcd.cls();
      lcd.locate(0, 0);
      lcd.printf("Well Done:)!");
      lcd.locate(0, 1);
      lcd.printf("That's Right:)");
      CorrectSound();
      flashGreen();
      pc.printf("\nCorrect!");
      delay(3000);
      roundNum++;
      score += 1;
    } else {
      lcd.cls();
      LooseSound();
      flashRed();
      lcd.printf("Oops Wrong :(");
      pc.printf("\nOops Wrong :(");
      lcd.locate(0, 1);
      lcd.printf("Ans: %s", numberToRemember);
      delay(5000);
      lcd.cls();
      lcd.locate(0, 0);
      lcd.printf("Your Score : %d", score);
      lcd.locate(0, 1);
      lcd.printf("Round Reached %d", roundNum);
      delay(5000);
      Restart();
    }
  }
}
//SinglePlayerEasy only gives a range of random numbers between 0 - 9 as specified in the sHmax and sHmin variables
//which also includes hex values
void SinglePlayerHard() {
  int score = 0;

  srand(time(NULL));

  int sHmax = 9;  // sHmax and sHmin are the variables
  int sHmin = 0;  // that store the range of the random values

  int hmax = 22;  // hmax and hmin stores the range the for
  int hmin = 17;  // the hex values

  while (1) {
    countdown();

    char numberToRemember[100] = { 0 };


    for (int i = 0; i < roundNum; i++) {
      numberToRemember[i] = '0' + (rand() % (sHmax - sHmin + 1) + sHmin);
      if (numberToRemember[i] < '5') {
        numberToRemember[i] = '0' + (rand() % (hmax - hmin + 1) + hmin);  //This for loop generates random values that is mixed with hex values
      } else if (numberToRemember[i] > '5') {
        numberToRemember[i] = '0' + (rand() % (sHmax - sHmin + 1) + sHmin);
      }
    }
    numberToRemember[roundNum] = '\0';


    lcd.cls();
    lcd.printf("Memorise:");
    lcd.locate(0, 1);
    lcd.printf("%s", numberToRemember); //User memorises number for a limited of time before it dissapears
    delay(1200);

    lcd.cls();
    lcd.printf("Now..");
    delay(500);


    getInput(roundNum);


    if (strcmp(input, numberToRemember) == 0) {
      lcd.cls();
      lcd.locate(0, 0);
      lcd.printf("Well Done:)!");
      lcd.locate(0, 1);
      lcd.printf("That's Right:)");
      CorrectSound();
      flashGreen();
      pc.printf("\nCorrect!");
      delay(3000);
      roundNum++;
      score += 1;
    } else {
      lcd.cls();
      LooseSound();
      flashRed();
      lcd.printf("Oops Wrong :(");
      pc.printf("\nOops Wrong :(");
      lcd.locate(0, 1);
      lcd.printf("Ans: %s", numberToRemember);
      delay(5000);
      lcd.cls();
      lcd.locate(0, 0);
      lcd.printf("Your Score : %d", score);
      lcd.locate(0, 1);
      lcd.printf("Round Reached %d", roundNum);
      delay(5000);
      Restart();
    }
  }
}

int Round = 1;
char P1num[17],
P2num[17];


void multiplayer() {

  while (true) {
    lcd.cls();
    lcd.printf("P1: %d digits", Round);
    lcd.locate(0, 1);                           //P1 enters a number using the pc keypad which is sent to pico
    pc.printf("\nP1 enter %d digits: ", Round);

    for (int i = 0; i < Round;) {
      if (pc.readable()) {
        char c = pc.getc();
        if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')) {   //This if statement gives on the permitted characters that p1 can input
          P1num[i] = c;                                          //The characters allowed have to be the same as the characters on the pico
          lcd.locate(i, 1);
          lcd.putc(c);                                      //Player1 starts off with 1 digit in the 1st round then it increments thereon after
          pc.printf("%c", c);
          i++;
        }
      }
    }
    P1num[Round] = '\0';

    delay(1000);
    lcd.cls();
    lcd.printf("P2 Memorise:");
    lcd.locate(0, 1);           //Player2 memorises this number for a short period of time before inputting the value
    lcd.printf("%s", P1num);
    delay(1500);

    lcd.cls();
    lcd.printf("P2 Enter:");
    for (int i = 0; i < Round;) {
      char k = getKey();
      if ((k >= '0' && k <= '9') || (k >= 'A' && k <= 'F')) {   // Player2 enters the memorised number on the keyboard which is then compared    
        P2num[i] = k;
        lcd.locate(i, 1);
        lcd.putc(k);
        i++;
        wait_us(200000);
      }
    }
    P2num[Round] = '\0';


    lcd.cls();
    if (!strcmp(P1num, P2num)) {
      lcd.cls();
      lcd.locate(0, 0);
      lcd.printf("Well Done:)!");
      lcd.locate(0, 1);
      lcd.printf("That's Right:)");
      CorrectSound();
      flashGreen();
      delay(1500);
      if (++Round > 16) {
      Round = 16;
      score += 1;
    }
    } else {
      lcd.cls();
      lcd.locate(0, 0);
      lcd.printf("Oops Wrong :(");
      flashRed();
      LooseSound();
      lcd.locate(0, 1);
      lcd.printf("Ans: %s", P1num);
      delay(5000);
      lcd.cls();
      lcd.locate(0, 0);
      lcd.printf("Your Score : %d", score);
      lcd.locate(0, 1);
      lcd.printf("Round Reached %d", Round);
      delay(5000);
      Restart2();
      score += 0;
    }
  }
}

int main() {
  //setup();
  introSound();
  pc.printf("\nWelcome To The Number Memory Game\n");
  pc.printf("Select A Game Mode\n");
  pc.printf("1. SinglePlayer\n");       //Main menu system displayed on the serial monitor
  pc.printf("2. MultiPlayer");
  setup();                    //The setup function is called in the main function,                  
}                             //which contains the menu display on the LCD screen

void setup() {
  lcd.cls();
  lcd.locate(0, 0);
  lcd.printf("1. SinglePlayer");      

  lcd.locate(0, 1);
  lcd.printf("2. MultiPlayer");
  while (1) {
    char key = ' ';
    key = getKey();
    if (key == '1') {
      pc.printf("\n%c", key);
      lcd.cls();
      lcd.locate(0, 0);
      singleplayer();
    }

    else if (key == '2') {
      pc.printf("\n%c", key);
      lcd.cls();
      lcd.cls();
      lcd.locate(0, 0);
      multiplayer();
    }
  }
}
void loop() {
  char key = ' ';
  while (1) {
    key = getKey();
    if (key != ' ') {
      lcd.locate(0, 1);
      lcd.printf("%c", key);
      pc.printf("Key = %c\r\n", key);
      wait_us(0.3 * WAIT_US);
    }
  }
}