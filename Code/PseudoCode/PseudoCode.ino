void setup() {
  Setup
}

void loop() {
  [x, y] = readInput()
  driveCrane()

  readDrop()
  dropCrane()
}

int[] readInput() {
  return readJoystick()
}

void driveCrane(int[x, y]) {
  goInDirection()
}

void readDrop() {
  readPushbutton()
}

void dropCrane() {
  goDown()
  grab()
  goUp()

  goHome()
}

void goHome(){
  while(![0, 0]) {
    goDiagonallyNegative()
  }
}