int collarDiameter = 13.187; // collar diameter in mm, change to adjust the conversion from sterpper movement to real world measurements
float collarCircum = PI*collarDiameter;

float stepsPerMM = 800/collarCircum;
float mmPerStep = collarCircum/800;

int mmToSteps(float mm){
  int steps = (int) mm*stepsPerMM;
  return steps;
}

float stepsToMM(int steps){
  float mm = (float) steps*mmPerStep;
  return mm;
}

const int limitX = 2; // need to use pins 2 and 3 for limit switch pins since they are interrupt capable
const int limitY = 3;
const int limitZ = 11;
const int stepX = 6;
const int dirX = 7; // dirX = HIGH makes the carriage move towards the limit switch
const int stepY = 4; 
const int dirY = 5; // dirY = LOW makes the carriage move towards the limit switch
const int stepZ = 8;
const int dirZ = 9;
const int MS2Pin = 10;
const boolean MS2Val = HIGH;
const boolean xMinusDir = HIGH;
const boolean yMinusDir = LOW;
const boolean zUpDir = LOW;

int xPos = 0;
int yPos = 0;
int zPos = 0;
float mmX = 0;
float mmY = 0;
float mmXMax = 240;
float mmYMax = 130;//175
int maxXPos = mmToSteps(mmXMax);
int maxYPos = mmToSteps(mmYMax);

int xSpeedDelay = 500;
int ySpeedDelay = 500;
int zSpeedDelay = 500;

boolean continueX = true;
boolean continueY = true;

int brailleMatrix[3][2];

void limitXPressed(){
  Serial.println("Limit X Pressed");
  continueX = false;
}

void limitYPressed(){
  Serial.println("Limit Y Pressed");
  continueY = false;
}

void printPositions(){
  Serial.print("x: ");
  Serial.println(xPos);
  Serial.print("y: ");
  Serial.println(yPos);
}

void homeX(){
  Serial.println("X-Homing begin");

  digitalWrite(dirX,xMinusDir);
  while(continueX){
    digitalWrite(stepX,HIGH); 
    delayMicroseconds(xSpeedDelay);
    digitalWrite(stepX,LOW); 
    delayMicroseconds(xSpeedDelay);
  }
  digitalWrite(dirX,!xMinusDir);
  while(digitalRead(limitX) == LOW){
    digitalWrite(stepX,HIGH); 
    delayMicroseconds(xSpeedDelay);
    digitalWrite(stepX,LOW); 
    delayMicroseconds(xSpeedDelay);
  }
  continueX = true;
  xPos = 0;
}

void homeY(){
  Serial.println("Y-Homing begin");

  digitalWrite(dirY,yMinusDir);
  while(continueY){
    digitalWrite(stepY,HIGH); 
    delayMicroseconds(ySpeedDelay);
    digitalWrite(stepY,LOW); 
    delayMicroseconds(ySpeedDelay);
  }
  digitalWrite(dirY,!yMinusDir);
  while(digitalRead(limitY) == LOW){
    digitalWrite(stepY,HIGH); 
    delayMicroseconds(ySpeedDelay);
    digitalWrite(stepY,LOW); 
    delayMicroseconds(ySpeedDelay);
  }
  continueY = true;
  yPos = 0;
}

void homeZ(){
  Serial.println("Z-Homing begin");

  digitalWrite(dirZ,zUpDir);
  while(digitalRead(limitZ) == HIGH){
    digitalWrite(stepZ,HIGH); 
    delayMicroseconds(zSpeedDelay);
    digitalWrite(stepZ,LOW); 
    delayMicroseconds(zSpeedDelay);
  }
  digitalWrite(dirZ,!zUpDir);
  while(digitalRead(limitZ) == LOW){
    digitalWrite(stepZ,HIGH); 
    delayMicroseconds(zSpeedDelay);
    digitalWrite(stepZ,LOW); 
    delayMicroseconds(zSpeedDelay);
  }
  zPos = 0;
}

void autohome(){
  Serial.println("Autohome start");
  
  digitalWrite(dirX,xMinusDir);
  while(continueX){
    digitalWrite(stepX,HIGH); 
    delayMicroseconds(xSpeedDelay);
    digitalWrite(stepX,LOW); 
    delayMicroseconds(xSpeedDelay);
  }
  digitalWrite(dirX,!xMinusDir);
  while(digitalRead(limitX) == LOW){
    digitalWrite(stepX,HIGH); 
    delayMicroseconds(xSpeedDelay);
    digitalWrite(stepX,LOW); 
    delayMicroseconds(xSpeedDelay);
  }
  continueX = true;
  xPos = 0;

  digitalWrite(dirY,yMinusDir);
  while(continueY){
    digitalWrite(stepY,HIGH); 
    delayMicroseconds(ySpeedDelay);
    digitalWrite(stepY,LOW); 
    delayMicroseconds(ySpeedDelay);
  }
  digitalWrite(dirY,!yMinusDir);
  while(digitalRead(limitY) == LOW){
    digitalWrite(stepY,HIGH); 
    delayMicroseconds(ySpeedDelay);
    digitalWrite(stepY,LOW); 
    delayMicroseconds(ySpeedDelay);
  }
  continueY = true;
  yPos = 0;

  Serial.println("Autohome end");
}

void xDown(float mm){
  int steps = mmToSteps(mm);
  digitalWrite(dirX,!xMinusDir);
  
  if ((xPos+steps) >= maxXPos)
    steps = maxXPos-xPos;
  xPos += steps;
  mmX = stepsToMM(xPos);
  
  for (int i = 0; i < steps; ++i){
    digitalWrite(stepX,HIGH);
    delayMicroseconds(xSpeedDelay);
    digitalWrite(stepX,LOW);
    delayMicroseconds(xSpeedDelay);
  }
}

void xUp(float mm){
  int steps = mmToSteps(mm);
  digitalWrite(dirX,xMinusDir);

  if ((xPos-steps) <= 0)
    steps = xPos;
  xPos -= steps;
  mmX = stepsToMM(xPos);
  
  for (int i = 0; i < steps; ++i){
    digitalWrite(stepX,HIGH);
    delayMicroseconds(xSpeedDelay);
    digitalWrite(stepX,LOW);
    delayMicroseconds(xSpeedDelay);
  }
}

void xTo(int stepPos){
  if (stepPos > xPos)
    digitalWrite(dirX,!xMinusDir);
  else
    digitalWrite(dirX,xMinusDir);
  int steps = abs(stepPos-xPos);
  for (int i = 0; i < steps; ++i){
    digitalWrite(stepX,HIGH);
    delayMicroseconds(xSpeedDelay);
    digitalWrite(stepX,LOW);
    delayMicroseconds(xSpeedDelay);
  }
  xPos = stepPos;
}

void yRight(float mm){
  int steps = mmToSteps(mm);
  digitalWrite(dirY,!yMinusDir);

  if ((yPos+steps) >= maxYPos)
    steps = maxYPos-yPos;
  yPos += steps;
  mmY = stepsToMM(yPos);
  
  for (int i = 0; i < steps; ++i){
    digitalWrite(stepY,HIGH);
    delayMicroseconds(ySpeedDelay);
    digitalWrite(stepY,LOW);
    delayMicroseconds(ySpeedDelay);
  }
}

void yLeft(float mm){
  int steps = mmToSteps(mm);
  digitalWrite(dirY,yMinusDir);

  if ((yPos-steps) <= 0)
    steps = yPos;
  yPos -= steps;
  mmY = stepsToMM(yPos);
  
  for (int i = 0; i < steps; ++i){
    digitalWrite(stepY,HIGH);
    delayMicroseconds(ySpeedDelay);
    digitalWrite(stepY,LOW);
    delayMicroseconds(ySpeedDelay);
  }
}

void penDown(){
  Serial.println("Pen down");
  digitalWrite(dirZ,!zUpDir);
  zPos = 180;// 200 
  for (int i = 0; i < zPos; ++i){
    digitalWrite(stepZ,HIGH);
    delayMicroseconds(zSpeedDelay);
    digitalWrite(stepZ,LOW);
    delayMicroseconds(zSpeedDelay);
  }
}

void penUp(){
  digitalWrite(dirZ,zUpDir);
  for (int i = zPos; i > 0; --i){
    digitalWrite(stepZ,HIGH);
    delayMicroseconds(zSpeedDelay);
    digitalWrite(stepZ,LOW);
    delayMicroseconds(zSpeedDelay);
  }
}

void setup() {
  // Sets the two pins as outputs
  delay(100);
  Serial.begin(115200);
  Serial.println("Setup start");
  pinMode(limitX,INPUT_PULLUP);
  pinMode(limitY,INPUT_PULLUP);
  pinMode(stepX,OUTPUT);
  pinMode(dirX,OUTPUT);
  pinMode(stepY,OUTPUT);
  pinMode(dirY,OUTPUT);
  pinMode(stepZ,OUTPUT);
  pinMode(dirZ,OUTPUT);
  pinMode(MS2Pin,OUTPUT);
  digitalWrite(MS2Pin,MS2Val);
  attachInterrupt(digitalPinToInterrupt(limitX), limitXPressed, FALLING);
  attachInterrupt(digitalPinToInterrupt(limitY), limitYPressed, FALLING);
  autohome();
  homeZ();
  delay(500);
  Serial.println("Setup end");
  Serial.print("Max x: ");
  Serial.println(maxXPos);
  Serial.print("Max y: ");
  Serial.println(maxYPos);
}

char charBuffer[1024];
int charPos = 0;

void loop() {
  delay(1000);
  Serial.println("Loop iteration");
//  printPositions();

  while(Serial.available() > 0){
    charBuffer[charPos++] = Serial.read();
  }

  Serial.print("Received: ");
  for (int i = 0; i < charPos; ++i)
    Serial.print(charBuffer[i]);
  Serial.println();

  processCharBuffer();

  for (int i = 0; i < 1024; ++i)
    charBuffer[i] = '\u0000';
  charPos = 0;

  


//  nextLine();
//  printH();
//  delay(100);
//  printP();
//  delay(100);
//  printFullCell();
//  delay(100);
//  printY();
//  delay(100);
//  printS();
//  delay(100);

}

int cellXPos = 0;
int cellYPos = 0;
const float inCellDotDist = 2.34; // is 2.34 according to Braille code
const float betweenCharDotDist = 6.2; // is either 6.2 or 3.1 according to Braille code
const float verticalLineSpacing = 10; // 10 according to Braille code, can make larger if need to

void processCharBuffer(){
  Serial.println("Processing buffer");
  for (int i = 0; i < charPos; ++i){
    char valToPrint = (char)charBuffer[i];
    Serial.println(valToPrint);
    switch (valToPrint) {
      case 'a': printA();
                Serial.print("Printing from input: a");
                break;
      case 'b': printB();
                Serial.print("Printing from input: b");
                break;
      case 'c': printC();
                Serial.print("Printing from input: c");
                break;
      case 'd': printD();
                Serial.print("Printing from input: d");
                break;
      case 'e': printE();
                Serial.print("Printing from input: e");
                break;
      case 'f': printF();
                Serial.print("Printing from input: f");
                break;
      case 'g': printG();
                Serial.print("Printing from input: g");
                break;
      case 'h': printH();
                Serial.print("Printing from input: h");
                break;
      case 'i': printI();
                Serial.print("Printing from input: i");
                break;
      case 'j': printJ();
                Serial.print("Printing from input: j");
                break;
      case 'k': printK();
                Serial.print("Printing from input: k");
                break;
      case 'l': printL();
                Serial.print("Printing from input: l");
                break;
      case 'm': printM();
                Serial.print("Printing from input: m");
                break;
      case 'n': printN();
                Serial.print("Printing from input: n");
                break;
      case 'o': printO();
                Serial.print("Printing from input: o");
                break;
      case 'p': printP();
                Serial.print("Printing from input: p");
                break;
      case 'q': printQ();
                Serial.print("Printing from input: q");
                break;
      case 'r': printR();
                Serial.print("Printing from input: r");
                break;
      case 's': printS();
                Serial.print("Printing from input: s");
                break;
      case 't': printT();
                Serial.print("Printing from input: t");
                break;
      case 'u': printU();
                Serial.print("Printing from input: u");
                break;
      case 'v': printV();
                Serial.print("Printing from input: v");
                break;
      case 'w': printW();
                Serial.print("Printing from input: w");
                break;
      case 'x': printX();
                Serial.print("Printing from input: x");
                break;
      case 'y': printY();
                Serial.print("Printing from input: y");
                break;
      case 'z': printZ();
                Serial.print("Printing from input: z");
                break;
      case '#': printPound();
                Serial.print("Printing from input: #");
                break;
      case '1': print1();
                Serial.print("Printing from input: 1");
                break;
      case '2': print2();
                Serial.print("Printing from input: 2");
                break;
      case '3': print3();
                Serial.print("Printing from input: 3");
                break;
      case '4': print4();
                Serial.print("Printing from input: 4");
                break;
      case '5': print5();
                Serial.print("Printing from input: 5");
                break;
      case '6': print6();
                Serial.print("Printing from input: 6");
                break;
      case '7': print7();
                Serial.print("Printing from input: 7");
                break;
      case '8': print8();
                Serial.print("Printing from input: 8");
                break;
      case '9': print9();
                Serial.print("Printing from input: 9");
                break;
      case '0': print0();
                Serial.print("Printing from input: 0");
                break;
      case ',': printComma();
                Serial.print("Printing from input: ,");
                break;
      case ';': printSemicolon();
                Serial.print("Printing from input: ;");
                break;
      case ':': printColon();
                Serial.print("Printing from input: :");
                break;
      case '.': printPeriod();
                Serial.print("Printing from input: .");
                break;
      case '?': printQuestion();
                Serial.print("Printing from input: ?");
                break;
      case '!': printExclamation();
                Serial.print("Printing from input: !");
                break;
      case '\'': printApostrophe();
                Serial.print("Printing from input: '");
                break;
      case '"': printQuotes();
                Serial.print("Printing from input: \"");
                break;
      case '-': printHyphen();
                Serial.print("Printing from input: -");
                break;
      case ' ': printSpace();
                Serial.print("Printing from input:  ");
                break;
      case '\n': nextLine();
                Serial.print("Printing from input: nextline");
                break;
      default:  printF();
                Serial.print("Default");
                break;
    }
  }
}

void nextLine(){
  Serial.println("Moving to next line");
  xDown(verticalLineSpacing);
  homeY();
  yRight(1000);
}

void printBrailleMat(){ // takes the state of the Braille matrix and prints it to the paper
  Serial.println("Start print character: ");
  if (yPos-inCellDotDist-betweenCharDotDist <= 0)
    nextLine();
  int startXPos = xPos;
  printPositions();
  for (int r = 0; r < 3; ++r){
    if (brailleMatrix[r][1] == 1){
      penDown();
      delay(250);
      homeZ();
    }
    xDown(inCellDotDist);
  }
  xTo(startXPos);
  yLeft(inCellDotDist);
  for (int r = 0; r < 3; ++r){
    if (brailleMatrix[r][0] == 1){
      penDown();
      delay(250);
      homeZ();
    }
    xDown(inCellDotDist);
  }
  xTo(startXPos);
  yLeft(betweenCharDotDist);
  Serial.println("End print character: ");
  printPositions();
}

void displayBrailleMat(){
  for (int r = 0; r < 3; ++r){
    for (int c = 0; c < 2; ++c){
      Serial.print(brailleMatrix[r][c]);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void mirrorBrailleMat(){
  for (int r = 0; r < 3; ++r){
    int temp = brailleMatrix[r][1];
    brailleMatrix[r][1] = brailleMatrix[r][0];
    brailleMatrix[r][0] = temp;
  }
}

void printFullCell(){
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = 1;
  printBrailleMat();
}

void printA(){
  int tempMat[3][2] = { {1,0},
                        {0,0},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printB(){
  int tempMat[3][2] = { {1,0},
                        {1,0},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printC(){
  int tempMat[3][2] = { {1,1},
                        {0,0},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printD(){
  int tempMat[3][2] = { {1,1},
                        {0,1},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printE(){
  int tempMat[3][2] = { {1,0},
                        {0,1},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printF(){
  int tempMat[3][2] = { {1,1},
                        {1,0},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printG(){
  int tempMat[3][2] = { {1,1},
                        {1,1},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printH(){
  int tempMat[3][2] = { {1,0},
                        {1,1},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printI(){
  int tempMat[3][2] = { {0,1},
                        {1,0},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printJ(){
  int tempMat[3][2] = { {0,1},
                        {1,1},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printK(){
  int tempMat[3][2] = { {1,0},
                        {0,0},
                        {1,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printL(){
  int tempMat[3][2] = { {1,0},
                        {1,0},
                        {1,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printM(){
  int tempMat[3][2] = { {1,1},
                        {0,0},
                        {1,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printN(){
  int tempMat[3][2] = { {1,1},
                        {0,1},
                        {1,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printO(){
  int tempMat[3][2] = { {1,0},
                        {0,1},
                        {1,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printP(){
  int tempMat[3][2] = { {1,1},
                        {1,0},
                        {1,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printQ(){
  int tempMat[3][2] = { {1,1},
                        {1,1},
                        {1,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printR(){
  int tempMat[3][2] = { {1,0},
                        {1,1},
                        {1,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printS(){
  int tempMat[3][2] = { {0,1},
                        {1,0},
                        {1,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printT(){
  int tempMat[3][2] = { {0,1},
                        {1,1},
                        {1,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printU(){
  int tempMat[3][2] = { {1,0},
                        {0,0},
                        {1,1}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printV(){
  int tempMat[3][2] = { {1,0},
                        {1,0},
                        {1,1}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printW(){
  int tempMat[3][2] = { {0,1},
                        {1,1},
                        {0,1}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printX(){
  int tempMat[3][2] = { {1,1},
                        {0,0},
                        {1,1}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printY(){
  int tempMat[3][2] = { {1,1},
                        {0,1},
                        {1,1}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printZ(){
  int tempMat[3][2] = { {1,0},
                        {0,1},
                        {1,1}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printPound(){
  int tempMat[3][2] = { {0,1},
                        {0,1},
                        {1,1}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void print1(){
  int tempMat[3][2] = { {1,0},
                        {0,0},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void print2(){
  int tempMat[3][2] = { {1,0},
                        {1,0},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void print3(){
  int tempMat[3][2] = { {1,1},
                        {0,0},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void print4(){
  int tempMat[3][2] = { {1,1},
                        {0,1},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void print5(){
  int tempMat[3][2] = { {1,0},
                        {0,1},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void print6(){
  int tempMat[3][2] = { {1,1},
                        {1,0},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void print7(){
  int tempMat[3][2] = { {1,1},
                        {1,1},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void print8(){
  int tempMat[3][2] = { {1,0},
                        {1,1},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void print9(){
  int tempMat[3][2] = { {0,1},
                        {1,0},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void print0(){
  int tempMat[3][2] = { {0,1},
                        {1,1},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printComma(){
  int tempMat[3][2] = { {0,0},
                        {1,0},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printSemicolon(){
  int tempMat[3][2] = { {0,0},
                        {1,0},
                        {1,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printColon(){
  int tempMat[3][2] = { {0,0},
                        {1,1},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printPeriod(){
  int tempMat[3][2] = { {0,0},
                        {1,1},
                        {0,1}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printQuestion(){
  int tempMat[3][2] = { {0,0},
                        {1,0},
                        {1,1}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printExclamation(){
  int tempMat[3][2] = { {0,0},
                        {1,1},
                        {1,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printApostrophe(){
  int tempMat[3][2] = { {0,0},
                        {0,0},
                        {1,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printQuotes(){
  int tempMat[3][2] = { {0,0},
                        {0,0},
                        {1,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
  printQuotes2();
}

void printQuotes2(){
  int tempMat[3][2] = { {0,0},
                        {1,1},
                        {1,1}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printHyphen(){
  int tempMat[3][2] = { {0,0},
                        {0,0},
                        {1,1}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}

void printSpace(){
  int tempMat[3][2] = { {0,0},
                        {0,0},
                        {0,0}};
  for (int r = 0; r < 3; ++r)
    for (int c = 0; c < 2; ++c)
      brailleMatrix[r][c] = tempMat[r][c];
  mirrorBrailleMat();
  printBrailleMat();
}
