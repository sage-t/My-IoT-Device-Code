#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 12
#define RELAY_PIN A1

Adafruit_NeoPixel strip = Adafruit_NeoPixel(143, PIN, NEO_RGB + NEO_KHZ800);

int currOption = 0;
int prevOption = 0;

int currInfo = 0;

int currScore1 = 0;
int currScore2 = 0;
int prevScore1 = -1;
int prevScore2 = -1;

void checkBoardStatus();
void showBall();
void setColoradoFlag();
void setAmericanFlag();

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

  pinMode(4, INPUT); // left switch
  pinMode(5, INPUT); // right switch

  float brightnessPot = analogRead(A0);
  int brightnessValue = (brightnessPot * 255) / 1023;
//  Serial.println(brightnessValue);

  strip.begin();
  strip.setBrightness(brightnessValue); // normally 60, turn down to ~10 for development
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  runOption();
//  Serial.println(getOption());
  
//  showUSA();


//  showBall(100);
//  setColoradoFlag();
//  strip.show();
//  
//  delay(1000);
//  
////  setAmericanFlag();
//  strip.show();
//
  delay(100);
}

/***********/
/* OPTIONS */
/***********/
int getOption() {
  bool left = digitalRead(4);
  bool right = digitalRead(5);

  float relay_value = 0.0;
  int optionLoops = 10;
  for (int i = 0; i < optionLoops; i++) {
    int pulseTest = pulseIn(RELAY_PIN, HIGH);
    if (pulseTest > relay_value) {
      relay_value = pulseTest;
    }
  }

  Serial.print("Relay Value: ");
  Serial.println(relay_value);

  if (relay_value > 109) {
    int info = (relay_value - 100) / 10;
    Serial.print("Info Value: ");
    Serial.println(info);
    if (info != currInfo) {
      currInfo = info;
      switch(currInfo) {
        case 1:
          currScore1 += 1;
          break;
        case 2:
          currScore1 -= 1;
          break;
        case 3:
          currScore2 += 1;
          break;
        case 4:
          currScore2 -= 1;
          break;
      }
    }

    return currOption;
  } else {
    currInfo = 0;
  }
  
  if (relay_value > 9) {
    Serial.println(relay_value);
    return relay_value / 10;
  } else if (relay_value < 7 && relay_value > 3) {
    return currOption;
  } else if (!left && !right) {
    return 1;
  } else if (left && !right) {
    return 2;
  } else if (!left && right) {
    return 3;
  } else {
    return 4;
  }
}

int runOption() {
  currOption = getOption();
  if (prevOption != currOption) {
    prevOption = currOption;
    prevScore1 = -1;
    prevScore2 = -1;
  }
  
  switch (currOption) {
    case 1:
//      showUSA();

//      showRainbowPortal();
      
//      setNaziFlag();
//      checkBoardStatus();

      paintBackground(0, 0, 0);
      strip.show();
      break;
    case 2:
      showBall();
      break;
    case 3:
      setColoradoFlag();
      strip.show();
      break;
    case 4:
      setAmericanFlag();
      strip.show();
      break;
    case 5:
      checkBoardStatus();
      break;
    case 6:
      paintBackground(200, 170, 255);
      strip.show();
      break;
    case 7:
      paintBackground(255, 255, 50);
      strip.show();
      break;
    case 8:
      showRainbowPortal();
      break;
  }
}

/*********/
/* SHOWS */
/*********/

//void showUSA() {
//  int repititions = 5;
//  int r1 = 255;
//  int g1 = 230;
//  int b1 = 0;
//  int r2 = 255;
//  int g2 = 130;
//  int b2 = 0;
//  
//  setAmericanFlag();
//  for (int i = 0; i < repititions; i++) {
//    paintU(g1, r1, b1);
//    strip.show();
//    delay(100);
//    paintU(g2, r2, b2);
//    strip.show();
//    delay(100);
//  }
//
//  setAmericanFlag();
//  for (int i = 0; i < repititions; i++) {
//    paintS(g1, r1, b1);
//    strip.show();
//    delay(100);
//    paintS(g2, r2, b2);
//    strip.show();
//    delay(100);
//  }
//
//  setAmericanFlag();
//  for (int i = 0; i < repititions; i++) {
//    paintA(g1, r1, b1);
//    strip.show();
//    delay(100);
//    paintA(g2, r2, b2);
//    strip.show();
//    delay(100);
//  }
//
//  setAmericanFlag();
//  strip.show();
//}


void showBall() {
  randomSeed(122);
  int ballCount = 3;
  int ballData = 4;
  int balls[ballCount][ballData] = {
    // {x, y, up, right}
    {random(0, 11), random(0, 13), random(0, 2), random(0, 2)},
    {random(0, 11), random(0, 13), random(0, 2), random(0, 2)},
    {random(0, 11), random(0, 13), random(0, 2), random(0, 2)}
  };

//  long quitTime = seconds * 1000;
  long delayTime = 100;
//  long t = 0;

  /* set background */
  for(uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(255, 255, 255));
  }

  while (getOption() == 2) {
    /* init data for next frame */
    int newBalls[ballCount][ballData];
    for (int i = 0; i < ballCount; i++) {
      for (int j = 0; j < ballData; j++) {
        newBalls[i][j] = balls[i][j];
      }
    }

    for (int b = 0; b < ballCount; b++) {
      /* determine if ball is hit */
      bool isHit = false;
      for (int i = 0; i < ballCount; i++) {
        if (i != b && balls[b][0] == balls[i][0] && balls[b][1] == balls[i][1]) {
          isHit = true;
        }
      }

      /* randomly change direction cause it's too hard to do it right */
      if (isHit) {
        newBalls[b][2] = random(0, 2);
        newBalls[b][3] = random(0, 2);
      }
      
      /* progress ball */
      if (newBalls[b][2]) {
        newBalls[b][0] += 1;
      } else {
        newBalls[b][0] -= 1;
      }

      if (newBalls[b][3]) {
        newBalls[b][1] += 1;
      } else {
        newBalls[b][1] -= 1;
      }

      /* ensure balls don't go out of bounds */
      if (newBalls[b][0] > 10) {
        newBalls[b][0] -= 2;
        newBalls[b][2] = 0;
      } else if (newBalls[b][0] < 0) {
        newBalls[b][0] += 2;
        newBalls[b][2] = 1;
      }

      if (newBalls[b][1] > 12) {
        newBalls[b][1] -= 2;
        newBalls[b][3] = 0;
      } else if (newBalls[b][1] < 0) {
        newBalls[b][1] += 2;
        newBalls[b][3] = 1;
      }
    }

    /* output balls */
    for (int b = 0; b < ballCount; b++) {
      strip.setPixelColor(coord(balls[b][0], balls[b][1]), strip.Color(255, 255, 255));
      strip.setPixelColor(coord(newBalls[b][0], newBalls[b][1]), strip.Color(0, 255, 0));
    }
    strip.show();
    delay(delayTime);
//    t += delayTime;

    /* update frame */
    for (int i = 0; i < ballCount; i++) {
      for (int j = 0; j < ballData; j++) {
        balls[i][j] = newBalls[i][j];
      }
    }
  }
}

/* Rainbow Portal */
void showRainbowPortal() {
  Serial.println("Rainbow");
  int colors[6][3] = {
    // {g, r, b}
//    {0, 148, 211},  // violet
    {0, 255, 255},  // purple
    {0, 0, 255},    // blue
    {255, 0, 0},    // green
    {255, 255, 0},  // yellow
    {127, 255, 0},  // orange
    {0, 255, 0},    // red
  };

  int frameCount = 6;
  int colorCount = 6;
  while (getOption() == 8) {
    for (int c = 0; c < colorCount; c++) {
      for (int i = 0; i < frameCount; i++) {
        setFrame(0 + i, 0 + i, 12 - i, 10 - i, colors[(i + c) % colorCount][0], colors[(i + c) % colorCount][1], colors[(i + c) % colorCount][2]);
      }
      strip.show();
      delay(60);
    }
  }
}

/********/
/* SETS */
/********/

void setFrame(int x1, int y1, int x2, int y2, int g, int r, int b) {
  for(uint16_t i = 0; i < strip.numPixels(); i++) {
    if ((row(i) == x1 || row(i) == x2) && column(i) >= y1 && column(i) <= y2) {
      strip.setPixelColor(i, strip.Color(g, r, b));
    } else if ((column(i) == y1 || column(i) == y2) && row(i) >= x1 && row(i) <= x2) {
      strip.setPixelColor(i, strip.Color(g, r, b));
    }
  }
}

/* FLAG SETS */

void setAmericanFlag() {
  for(uint16_t i = 0; i < strip.numPixels(); i++) {
    if (row(i) > 5 && column(i) < 5) {
      if (i % 2) {
        strip.setPixelColor(i, strip.Color(0, 0, 255)); // blue
      } else {
        strip.setPixelColor(i, strip.Color(60, 60, 255)); // white
      }
    } else if (row(i) % 2 == 0) {
      strip.setPixelColor(i, strip.Color(0, 255, 0)); // red
    } else {
      strip.setPixelColor(i, strip.Color(255, 255, 255)); // white
    }
  }
}

void setColoradoFlag() {
  /* stripes */
  for(uint16_t i = 0; i < strip.numPixels(); i++) {
    if (row(i) < 4 || row(i) > 8) {
      strip.setPixelColor(i, strip.Color(0, 0, 255)); // blue
    } else {
      strip.setPixelColor(i, strip.Color(255, 255, 255)); // white
    }
  }

  /* sun */
  for (int x = 3; x < 6; x++) {
    for (int y = 4; y < 9; y++) {
      strip.setPixelColor(coord(x, y), strip.Color(255, 255, 0));
    }
  }

  /* C */
  for (int y = 3; y < 10; y++) {
    strip.setPixelColor(coord(2, y), strip.Color(0, 255, 0));
  }
  for (int x = 3; x < 6; x++) {
    for (int y = 2; y < 4; y++) {
      strip.setPixelColor(coord(x, y), strip.Color(0, 255, 0));
    }
  }
  for (int x = 3; x < 6; x++) {
    for (int y = 9; y < 11; y++) {
      strip.setPixelColor(coord(x, y), strip.Color(0, 255, 0));
    }
  }
  strip.setPixelColor(coord(6, 9), strip.Color(0, 255, 0));
  strip.setPixelColor(coord(6, 3), strip.Color(0, 255, 0));
  strip.setPixelColor(coord(6, 8), strip.Color(0, 255, 0));
  strip.setPixelColor(coord(6, 4), strip.Color(0, 255, 0));
}

//void setAmericanFlag() {
//  for(uint16_t i = 0; i < strip.numPixels(); i++) {
//    if (row(i) > 5 && column(i) < 5) {
//      if (i % 2) {
//        strip.setPixelColor(i, strip.Color(0, 0, 255)); // blue
//      } else {
//        strip.setPixelColor(i, strip.Color(60, 60, 255)); // white
//      }
//    } else if (row(i) % 2 == 0) {
//      strip.setPixelColor(i, strip.Color(0, 255, 0)); // red
//    } else {
//      strip.setPixelColor(i, strip.Color(255, 255, 255)); // white
//    }
//  }
//}

long randBlue;
long flickerCount = 0;
long flickerTick = 30;
//int naziWhite = 70;

//void setNaziFlag() {
//  for(uint16_t i = 0; i < strip.numPixels(); i++) {
//    if (row(i) > 1 && row(i) < 11 && column(i) < 8 && column(i) > 2) {
//      strip.setPixelColor(i, strip.Color(naziWhite, naziWhite, naziWhite)); // white
//    } else if ((row(i) == 1 || row(i) == 11) && column(i) < 7 && column(i) > 3) {
//      strip.setPixelColor(i, strip.Color(naziWhite, naziWhite, naziWhite)); // white
//    } 
////    else if ((column(i) == 8 || column(i) == 2) && row(i) < 9 && row(i) > 3) {
////      strip.setPixelColor(i, strip.Color(255, 255, 255)); // white
////    } 
//    else {
//      strip.setPixelColor(i, strip.Color(0, 130, 0)); // red
//    }
//  }
//
//  flickerCount += 1;
//
//  if (flickerCount == flickerTick) {
//    randBlue = random(30, 80);

//
//    flickerCount = 0;
//    flickerTick = random(15, 150);
//    if (flickerTick < 75) {
//      flickerTick = random(1, 6);
//    }
//  } else {
    
//  }
//
//  
//}

/**********/
/* PAINTS */
/**********/

//int paintNone[13][11] = {
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//};

//int paintZero[11][3] = {
//  {1, 1, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 1, 1},
//};

//  1
// 2 3
//  4
// 5 6
//  7
int paintZero[7] = {1, 1, 1, 0, 1, 1, 1};
int paintOne[7] = {0, 0, 1, 0, 0, 1, 0};
int paintTwo[7] = {1, 0, 1, 1, 1, 0, 1};
int paintThree[7] = {1, 0, 1, 1, 0, 1, 1};
int paintFour[7] = {0, 1, 1, 1, 0, 1, 0};
int paintFive[7] = {1, 1, 0, 1, 0, 1, 1};
int paintSix[7] = {1, 1, 0, 1, 1, 1, 1};
int paintSeven[7] = {1, 0, 1, 0, 0, 1, 0};
int paintEight[7] = {1, 1, 1, 1, 1, 1, 1};
int paintNine[7] = {1, 1, 1, 1, 0, 1, 0};

void createNumberPaint(int numPaint[7], int paint[11][3]) {
  if (numPaint[0]) {
    paint[0][0] = 1;
    paint[0][1] = 1;
    paint[0][2] = 1;
  }
  if (numPaint[1]) {
    paint[0][0] = 1;
    paint[1][0] = 1;
    paint[2][0] = 1;
    paint[3][0] = 1;
    paint[4][0] = 1;
    paint[5][0] = 1;
  }
  if (numPaint[2]) {
    paint[0][2] = 1;
    paint[1][2] = 1;
    paint[2][2] = 1;
    paint[3][2] = 1;
    paint[4][2] = 1;
    paint[5][2] = 1;
  }
  if (numPaint[3]) {
    paint[5][0] = 1;
    paint[5][1] = 1;
    paint[5][2] = 1;
  }
  if (numPaint[4]) {
    paint[5][0] = 1;
    paint[6][0] = 1;
    paint[7][0] = 1;
    paint[8][0] = 1;
    paint[9][0] = 1;
    paint[10][0] = 1;
  }
  if (numPaint[5]) {
    paint[5][2] = 1;
    paint[6][2] = 1;
    paint[7][2] = 1;
    paint[8][2] = 1;
    paint[9][2] = 1;
    paint[10][2] = 1;
  }
  if (numPaint[6]) {
    paint[10][0] = 1;
    paint[10][1] = 1;
    paint[10][2] = 1;
  }
}

//int paintBlank[11][3] = {
//  {0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 0},
//};

//int paintOne[11][3] = {
//  {1, 1, 0},
//  {0, 1, 0},
//  {0, 1, 0},
//  {0, 1, 0},
//  {0, 1, 0},
//  {0, 1, 0},
//  {0, 1, 0},
//  {0, 1, 0},
//  {0, 1, 0},
//  {0, 1, 0},
//  {1, 1, 1},
//};
//
//int paintTwo[11][3] = {
//  {1, 1, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {1, 1, 1},
//  {1, 0, 0},
//  {1, 0, 0},
//  {1, 0, 0},
//  {1, 0, 0},
//  {1, 1, 1},
//};
//
//int paintThree[11][3] = {
//  {1, 1, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {1, 1, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {1, 1, 1},
//};
//
//int paintFour[11][3] = {
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 1, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//};
//
//int paintFive[11][3] = {
//  {1, 1, 1},
//  {1, 0, 0},
//  {1, 0, 0},
//  {1, 0, 0},
//  {1, 0, 0},
//  {1, 1, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {1, 1, 1},
//};
//
//int paintSix[11][3] = {
//  {1, 1, 1},
//  {1, 0, 0},
//  {1, 0, 0},
//  {1, 0, 0},
//  {1, 0, 0},
//  {1, 1, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 1, 1},
//};

//int paintSeven[11][3] = {
//  {1, 1, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//};
//
//int paintEight[11][3] = {
//  {1, 1, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 1, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 1, 1},
//};

//int paintNine[11][3] = {
//  {1, 1, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 0, 1},
//  {1, 1, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//  {0, 0, 1},
//};

void setPaint(int paint[13][11], int g, int r, int b) {
  for (int x = 0; x < 13; x++) {
    for (int y = 0; y < 11; y++) {
      if (paint[x][y]) {
        strip.setPixelColor(coord(y, 12 - x), strip.Color(g, r, b));
      }
    }
  }
}

/* copy small paint1 to small paint2 */
void copySmallPaint(int paint1[11][3], int paint2[11][3]) {
  for (int x = 0; x < 11; x++) {
    for (int y = 0; y < 3; y++) {
      if (paint1[x][y]) {
        paint2[x][y] = paint1[x][y];
      }
    }
  }
}

/* copy paint1 (small) to paint2 (full) */
void copySmallToBigPaint(int paint1[11][3], int paint2[13][11], int x1, int y1) {
  for (int x = 0; x < 11; x++) {
    for (int y = 0; y < 3; y++) {
      if (paint1[x][y]) {
        paint2[x + x1][y + y1] = paint1[x][y];
      }
    }
  }
}

/* copy paint1 to paint2 */
void copyPaint(int paint1[13][11], int paint2[13][11]) {
  for (int x = 0; x < 13; x++) {
    for (int y = 0; y < 11; y++) {
      if (paint1[x][y]) {
        paint2[x][y] = paint1[x][y];
      }
    }
  }
}

void setPaintNumber(int n, int paint[11][3]) {
  switch (n % 10) {
    case 0:
      createNumberPaint(paintZero, paint);
      break;
    case 1:
      createNumberPaint(paintOne, paint);
      break;
    case 2:
      createNumberPaint(paintTwo, paint);
      break;
    case 3:
      createNumberPaint(paintThree, paint);
      break;
    case 4:
      createNumberPaint(paintFour, paint);
      break;
    case 5:
      createNumberPaint(paintFive, paint);
      break;
    case 6:
      createNumberPaint(paintSix, paint);
      break;
    case 7:
      createNumberPaint(paintSeven, paint);
      break;
    case 8:
      createNumberPaint(paintEight, paint);
      break;
    case 9:
      createNumberPaint(paintNine, paint);
      break;
  }
}

int paintDivide[13][11] = {
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
};

//void paintDivideideFun(int g, int r, int b) {
//  int paint[13][11] = {
//    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
//  };
//
//  for (int x = 0; x < 13; x++) {
//    for (int y = 0; y < 11; y++) {
//      if (paint[x][y]) {
//        strip.setPixelColor(coord(y, 12 - x), strip.Color(g, r, b));
//      }
//    }
//  }
//}

//void paintU(int g, int r, int b) {
//  int paint[13][11] = {
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
//    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
//    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  };
//
//  for (int x = 0; x < 13; x++) {
//    for (int y = 0; y < 11; y++) {
//      if (paint[x][y]) {
//        strip.setPixelColor(coord(y, 12 - x), strip.Color(g, r, b));
//      }
//    }
//  }
//}
//
//void paintS(int g, int r, int b) {
//  int paint[13][11] = {
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  };
//
//  for (int x = 0; x < 13; x++) {
//    for (int y = 0; y < 11; y++) {
//      if (paint[x][y]) {
//        strip.setPixelColor(coord(y, 12 - x), strip.Color(g, r, b));
//      }
//    }
//  }
//}
//
//void paintA(int g, int r, int b) {
//  int paint[13][11] = {
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
//    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
//    {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
//    {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  };
//
//  for (int x = 0; x < 13; x++) {
//    for (int y = 0; y < 11; y++) {
//      if (paint[x][y]) {
//        strip.setPixelColor(coord(y, 12 - x), strip.Color(g, r, b));
//      }
//    }
//  }
//}


void paintBackground(int g, int r, int b) {
  for (int x = 0; x < 13; x++) {
    for (int y = 0; y < 11; y++) {
      strip.setPixelColor(coord(y, 12 - x), strip.Color(g, r, b));
    }
  }
}

/***********/
/* HELPERS */
/***********/

/* return row of pixel index; bottom = 0 */
int row(int i) {
  return 13 * i / 143;
}

/* return column of pixel index; leftmost = 0 */
int column(int i) {
  int col = i % 11;
  if (row(i) % 2) {
    col = 10 - col;
  }
  return col;
}

/* return pixel index from coords; x = column; y = row */
int coord(int x, int y) {
  int row_i = 11 * y;
  if (y % 2) {
    return row_i + 10 - x; 
  } else {
    return row_i + x;
  }
}

