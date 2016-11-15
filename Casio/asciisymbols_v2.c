#include <stdio.h>
#include <stdlib.h>
#include "asciisymbols_v2.h"

void initDisplay();
void clearDisplay();
void updateDisplay();
void setCharDisplay(int position, int digit);
void toggleColon();

int displayHeight = 9;
int displayWidth = 48;

char display[48][9];

int displayColon = 0;
int displayInvert = 0;

int main() {

  initDisplay();

  setCharDisplay(1, 1);
  setCharDisplay(2, 2);
  setCharDisplay(3, 3);
  setCharDisplay(4, 4);
  setCharDisplay(5, 5);
  setCharDisplay(6, 6);

  toggleColon();

  clearDisplay();

  displayInvert = 1;

  updateDisplay();

  //while(1);

  return 0;
}

void initDisplay() {
  for(int i = 0; i < displayWidth; i++) {
    for(int j = 0; j < displayHeight; j++) {
      display[i][j] = ' ';
    }
  }
}

void clearDisplay() {

  system("clear");

}

void toggleColon() {

  int colon1stPos = 15;
  int colon2ndPos = 32;

  if(displayColon) {
    for(int i = 0; i < 7; i++) {
      display[colon1stPos][i+1] = ' ';
      display[colon2ndPos][i+1] = ' ';
    }
  }
  else {
    for(int i = 0; i < 7; i++) {
      display[colon1stPos][i+1] = ascii_colon[i][0];
      display[colon2ndPos][i+1] = ascii_colon[i][0];
    }
  }
}

void setCharDisplay(int position, int digit) {

  int offset = 0;
  char outputDigit[7][5];

  switch (position) {
    case 1:
    offset = 1;
    break;
    case 2:
    offset = 8;
    break;
    case 3:
    offset = 18;
    break;
    case 4:
    offset = 25;
    break;
    case 5:
    offset = 35;
    break;
    case 6:
    offset = 42;
    break;
    default:
    return;
  }

  for(int i = 0; i < 5; i++) {
    for(int j = 0; j < 7; j++) {
      display[i+offset][j+1] = asciiDigits[digit][j][i];
    }
  }

}

void updateDisplay() {

  if(!displayInvert) {
    for(int i = 0; i < displayHeight; i++) {
      for(int j = 0; j < displayWidth; j++) {
        printf("%c",display[j][i]);
      }
      printf("\n");
    }
  } else {
    for(int i = 0; i < displayHeight; i++) {
      for(int j = 0; j < displayWidth; j++) {
        if(display[j][i] == '&') printf(" ");
        else printf("X");
      }
      printf("\n");
    }
  }



}
