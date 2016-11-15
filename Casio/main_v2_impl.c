// Compile code: gcc -lncurses main.c

#include <stdio.h>
//#include <stdlib.h>
//#include <curses.h>
#include "asciisymbols_v2.h"
#include <stdio.h>
#include "xparameters.h"
#include "xgpio.h"
#include "xtmrctr.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_print.h"
#include "xtime_l.h"

// PROGRAM
const int programs = 5; // Amount of
int programMode = 1; // 1 - Time, 2 - Stopwatch, 3 - Set Alarm, 4 - Set Time, 5 - Alarming
int newProgramMode = 1;

// TIME
int timer = 0;
int seconds = 0;
int minutes = 0;
int hours = 0;
int setTimeMode = 0;

// STOPWATCH
int stopwatchTimer = 0;
int stopwatchSeconds = 0;
int stopwatchMinutes = 0;
int stopwatchHours = 0;
int stopwatchLapping = 0; // 0(Not lapping), 1(Lapping, counting in background).
int stopwatchCounting = 0; // 0(Not counting), 1(Counting).

// ALARM
int alarmTime = 0;
int alarmMinutes = 0;
int alarmHours = 0;
int alarmSet = 0; // Is the alarm set?
int setAlarm = 0; // If the alarm should be set?
int setAlarmMode = 0; // Set hours or minutes.
int alarming = 0; // 0 - Not Alarming, 1 - Alarming.

// DISPLAY
int displayHeight = 9;
int displayWidth = 48;
int displayInvert = 0;
char display[48][9];
int toggleColon = 1;

// DISPLAY FUNCTIONS
void initDisplay();
void updateDisplay();
void printDisplay();
void clearDisplay();
void setCharDisplay(int position, int digit);
void displayColon();

// PROGRAM Functions
void introduction();
void updateProgram(int input);

// INTERRUPTS
void timeInterrupt();
void buttonInterrupt();
int button;

// MAIN
int main() {

  print("Hello, World!\n\r");

  introduction();

  initDisplay();

  while(1) {

    updateDisplay();

  }

  return 0;

}

void buttonInterrupt(int button) {
  // 1 - bottom left
  // 2 - bottom right
  // 3 - top left
  // 4 - top right

  // read which button is pressed
  // button = ???

  if(button == 4) { // Invert display.
    print("Invert ASCII GUI.");
    displayInvert = !displayInvert;
    button = 0;
  }

  if(programMode == programs && button != 0) { // Press any key to stop alarm.
    print("Stop alarm.");
    alarming = 0;
    alarmSet = setAlarm;
    newProgramMode = 1;
    button = 0;
    return;
  }

  if(button == 1) { // Toggle program modes.
    newProgramMode++;
    if(newProgramMode == programs) newProgramMode = 1;
  }

  updateProgram(button);

  return;

}

void timeInterrupt() {

  timer++; // Update timer.

  // Alarm check
  if(timer == alarmTime && alarmSet == 1) {
    alarmSet = 0;
    alarming = 1;
    programMode = 5; // Alarming.
  }

  // Regular timing, reset time and daily alarm at midnight.
  if(timer > 86399) {
    alarmSet = setAlarm;
    timer = timer - 86400;
  }

  seconds = timer % 60;
  minutes = (timer / 60) % 60;
  hours = (timer / 3600) % 24;

  // Stopwatch timing
  if(stopwatchCounting) {

    stopwatchTimer++; // Update stopwatch timer.

    if(stopwatchTimer > 86399) { // Reset stopwatch when "full"
      stopwatchTimer = stopwatchTimer - 86400;
    }

    if(!stopwatchLapping) { // Check for lapping
      stopwatchSeconds = stopwatchTimer % 60;
      stopwatchMinutes = (stopwatchTimer / 60) % 60;
      stopwatchHours = (stopwatchTimer / 3600) % 60;
    }
  }

  updateProgram(0);

  return;

}

void updateProgram(int input) {

  if(newProgramMode == programMode && input == 0) return;

  programMode = newProgramMode;

  switch (programMode) {
    case 1: // Time.

      // Display time.

    break;

    case 2: // Stopwatch.

      // Display stopwatch time.

      if(input == 2) { // Start / Stop.
        stopwatchCounting = !stopwatchCounting;
        return;
      }

      if(input == 3 && stopwatchCounting == 1) { // Lap
        stopwatchLapping = !stopwatchLapping;
        return;

      }

      if(input == 3 && stopwatchCounting == 0) { // Reset
        stopwatchTimer = 0;
        stopwatchSeconds = 0;
        stopwatchMinutes = 0;
        stopwatchHours = 0;
        return;
      }

    break;

    case 3: // Set Alarm.

      // Display alarm time.

      if(setAlarmMode == 0 && input == 2) { // Toggle set alarm.
        setAlarm = !setAlarm;
        return;
      }

      // Toggle Start Adjust / Adjust Hours.
      if((setAlarmMode == 0 && input == 3) || (setAlarmMode == 2 && input == 2)) {
        setAlarmMode = 1;
        return;
      }

      // Adjust Minutes.
      if(setAlarmMode == 1 && input == 2) {
        setAlarmMode = 2;
        return;
      }

      // End Adjust.
      if(setAlarmMode > 0 && input == 3) {
        setAlarmMode = 0;
        return;
      }

      // Add hour
      if(setAlarmMode == 1 && input == 2) {
        alarmTime += 3600;
        if(alarmTime > 86399) alarmTime -= 86400;
      }

      if(setAlarmMode == 2 && input == 2) {
        alarmTime += 60;
        if((alarmMinutes / 60) > 59) {
          alarmTime -= 3660;
        }
      }

      alarmHours = (alarmTime / 3600) % 60;
      alarmMinutes = (alarmTime / 60) % 60;

      return;

    break;

    case 4: // Set Time.

      // Display time.

      // Same as set alarm.

    break;

    case programs: // Alarming

      //display blinking time

    break;

    default:

    break;
  }

  return;
}

void updateDisplay() {

  switch(programMode) {

    case 1: // Time

    setCharDisplay(1, hours/10);
    setCharDisplay(2, hours%10);
    setCharDisplay(3, minutes/10);
    setCharDisplay(4, minutes%10);
    setCharDisplay(5, seconds/10);
    setCharDisplay(6, seconds%10);
    toggleColon = timer%2;
    displayColon();

    clearDisplay();

    print("[Time Mode]\n");

    printDisplay();

    break;

    case 2: // Stopwatch

    setCharDisplay(1, stopwatchHours/10);
    setCharDisplay(2, stopwatchHours%10);
    setCharDisplay(3, stopwatchMinutes/10);
    setCharDisplay(4, stopwatchMinutes%10);
    setCharDisplay(5, stopwatchSeconds/10);
    setCharDisplay(6, stopwatchSeconds%10);
    toggleColon = stopwatchTimer%2;
    displayColon();

    clearDisplay();

    print("[Stopwatch Mode]");
    if(stopwatchCounting) print("[Counting]");
    if(stopwatchLapping) print("[Lapping]");
    print("\n");

    printDisplay();

    break;

    case 3:

    setCharDisplay(1, alarmHours/10);
    setCharDisplay(2, alarmHours%10);
    setCharDisplay(3, alarmMinutes/10);
    setCharDisplay(4, alarmMinutes%10);
    toggleColon = 1;
    displayColon();

    // Show on / off.

    clearDisplay();

    print("[Alarm Mode]");
    if(setAlarm) print("[Set]");
    if(setAlarmMode == 1) print("[Adjust Hours]");
    if(setAlarmMode == 2) print("[Adjust Minutes]");
    print("/n");

    printDisplay();

    break;

    case 4:

    clearDisplay();

    print("[Set Time Mode]");

    break;

    case 5: // Alarming

    setCharDisplay(1, hours/10);
    setCharDisplay(2, hours%10);
    setCharDisplay(3, minutes/10);
    setCharDisplay(4, minutes%10);
    setCharDisplay(5, seconds/10);
    setCharDisplay(6, seconds%10);

    for(int i = 0; i < 9; i++) {
        display[timer % displayWidth][i] = '&';
    }

    clearDisplay();

    print("[Alarming Mode]");

    printDisplay();

    break;

    default:

    programMode = 1;

    break;

  }

  return;

}

void printDisplay() { // The actual printing.

  if(!displayInvert) {
    for(int i = 0; i < displayHeight; i++) {
      for(int j = 0; j < displayWidth; j++) {
        print("%c",display[j][i]);
      }
      print("\n");
    }
  } else {
    for(int i = 0; i < displayHeight; i++) {
      for(int j = 0; j < displayWidth; j++) {
        if(display[j][i] == '&') print(" ");
        else print("X");
      }
      print("\n");
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

void clearDisplay() {
  for(int i = 0; i < displayWidth; i++) {
    for(int j = 0; j < displayHeight; j++) {
      display[i][j] = ' ';
    }
  }

  //system("clear");
}

void displayColon() {

  int colon1stPos = 15;
  int colon2ndPos = 32;

  if(!toggleColon) {
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

void initDisplay() {
  for(int i = 0; i < displayWidth; i++) {
    for(int j = 0; j < displayHeight; j++) {
      display[i][j] = ' ';
    }
  }
}

void introduction() {
  print("Welcome to the Casio Watch.\n");
  print("\n");
  print("Controls:\n");
  print("\n");
  print("(q) Reset/Lap/Adjust      (e) Light\n");
  print("(a) Mode                  (d) Start/Stop/Set\n");
  print("\n");
  print("Timing:\n");
  print("\n");
  print("(1) +1 second.\n");
  print("(2) +10 seconds.\n");
  print("(3) +1 minute.\n");
  print("(4) +10 minutes.\n");
  print("(5) +1 hour.\n");
  print("(6) +10 hours.\n");
  print("\n");
  print("Press (h) for help.\n");
  print("Press (x) to exit.\n");
  print("\n");
  print("Press any of the keys to continue...\n");
}
