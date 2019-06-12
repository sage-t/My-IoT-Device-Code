/****************/
/* INTERACTIONS */
/****************/

int paintBoard[13][11] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

int paintScore[11][3] = {
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
};

void updateBoard(int score1, int score2) {
  paintBackground(0, 0, 0);
  /* reset paintBoard */
  for (int x = 0; x < 13; x++) {
    for (int y = 0; y < 11; y++) {
      paintBoard[x][y] = 0;
    }
  }
  /* reset paintScore */
  for (int x = 0; x < 11; x++) {
    for (int y = 0; y < 3; y++) {
      paintScore[x][y] = 0;
    }
  }
  
  copyPaint(paintDivide, paintBoard);

  setPaintNumber(score1, paintScore);
  copySmallToBigPaint(paintScore, paintBoard, 0, 1);

  /* reset paintScore */
  for (int x = 0; x < 11; x++) {
    for (int y = 0; y < 3; y++) {
      paintScore[x][y] = 0;
    }
  }
  
  setPaintNumber(score2, paintScore);
  copySmallToBigPaint(paintScore, paintBoard, 0, 7);

  /* handle values greater than 9 */
  for (int i = 0; i < (score1 / 10); i++) { paintBoard[12][i] = 1; }
  for (int i = 0; i < (score2 / 10); i++) { paintBoard[12][10 - i] = 1; }

  setPaint(paintBoard, 80, 255, 0);
  
  delay(10);
  strip.show();
}

long lastBoardUpdate = 0;
long now;

void checkBoardStatus() {
  Serial.println("checking scoreboard");

  now = millis();
  if ((currScore1 != prevScore1) || (currScore2 != prevScore2) || (now - lastBoardUpdate > 1000)) {
    lastBoardUpdate = now;
    Serial.println("updating scoreboard");
    updateBoard(currScore1, currScore2);
    prevScore1 = currScore1;
    prevScore2 = currScore2;
  }
}
