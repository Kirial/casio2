// Keyboard Controls

char readKeyboard() {

  char inputC;

  initscr();
  noecho();
  cbreak();
  timeout(10);

  inputC = getch();

  switch (inputC) {

    case '1': // +1 seconds
    case '2': // +10 seconds
    case '3': // +1 minutes
    case '4': // +10 minutes
    case '5': // +1 hours
    case '6': // +10 hours
    case 'q': // Light
    case 'a': // Mode
    case 'e': // Adjust / Reset
    case 'd': // Start / Stop
    case 'h': // Help
    case 'x': // Exit Application
    break;

    default:
    inputC = '0';
    break;

  }

  nocbreak();
  endwin();

  return inputC;

}

void keyboardTiming(char inputC, int *time) {

  switch (inputC) {
    case '1':
    &time ++;
    stopwatchTimer += stopwatchCounting * 1;
    break;
    case '2':
    &time += 10;
    stopwatchTimer += stopwatchCounting * 10;
    break;
    case '3':
    &time += 60;
    stopwatchTimer += stopwatchCounting * 60;
    break;
    case '4':
    &time += 600;
    stopwatchTimer += stopwatchCounting * 600;
    break;
    case '5':
    &time += 3600;
    stopwatchTimer += stopwatchCounting * 3600;
    break;
    case '6':
    &time += 36000;
    stopwatchTimer += stopwatchCounting * 36000;
    break;
    default:
    break;
  }
}
