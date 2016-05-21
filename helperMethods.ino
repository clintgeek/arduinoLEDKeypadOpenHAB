void debugPrinter(String title, int blankLines) {
  if (debug) {
    Serial.println(title);

    for (int i=0; i < blankLines; i++) {
      Serial.println();
    }
  }
}

void debugPrinter(String title, int value, int blankLines) {
  if (debug) {
    Serial.print(title);
    Serial.println(value);

    for (int i=0; i < blankLines; i++) {
      Serial.println();
    }
  }
}

void writeSingleColor(int colorIndex, int brightness) {
  int outPin;

  switch (colorIndex) {
    case 0:
      outPin = rOutPin;
      break;
    case 1:
      outPin = gOutPin;
      break;
    case 2:
      outPin = bOutPin;
      break;
  }

  analogWrite(outPin, brightness);
}

void rgb(int r, int g, int b) {
  writeSingleColor(0, r);
  writeSingleColor(1, g);
  writeSingleColor(2, b);
}

void setSingleColor(int colorIndex, int brightness) {
  switch (colorIndex) {
    case 0:
      rVal = brightness;
      break;
    case 1:
      gVal = brightness;
      break;
    case 2:
      bVal = brightness;
      break;
  }
}

void setRgb(int r, int g, int b) {
  setSingleColor(0, r);
  setSingleColor(1, g);
  setSingleColor(2, b);
}

void threadSafeDelay(int duration) {
  for (int delayCounter = 0; delayCounter < duration; delayCounter++) {
    inputWatcher();
    if (!abortNow) {
      delay(1);
    }
  }
}

void threadSafeDelay(int min, int max) {
  int totalDelay = random(min, max);

  for (int delayCounter = 0; delayCounter < totalDelay; delayCounter++) {
    inputWatcher();
    if (!abortNow) {
      delay(1);
    }
  }
}

void adjustColor(char color, char direction) {
  request = mode;
  switch (color) {
    case 'r':
      rVal = adjustBrightness(rVal, direction);
      break;
    case 'g':
      gVal = adjustBrightness(gVal, direction);
      break;
    case 'b':
      bVal = adjustBrightness(bVal, direction);
      break;
  }
}

int adjustBrightness(int brightness, char direction) {
  switch (direction) {
    case 'u':
      if (brightness <= 245) {
        brightness = brightness + 10;
      } else {
        brightness = 255;
      }
      break;
    case 'd':
      if (brightness >= 10) {
        brightness = brightness - 10;
      } else {
        brightness = 0;
      }
      break;
    case 'x':
      brightness = 255;
      break;
    case 'n':
      brightness = 0;
      break;
  }

  return brightness;
}

int primaryColor() {
  int primaryColor;
  if (rVal > gVal && rVal > bVal) {
    primaryColor = 0;
  }
  else if (gVal > rVal && gVal > bVal) {
    primaryColor = 1;
  }
  else primaryColor = 2;

  return primaryColor;
}

void breatheIn(int color) {
  int rgbColor[3];
  rgbColor[0] = 0;
  rgbColor[1] = 0;
  rgbColor[2] = 0;

  for (int brightness = 0; brightness <= 255; brightness++) {
    if (abortNow) { break; }
    rgbColor[color] = brightness;
    rgb(rgbColor[0], rgbColor[1], rgbColor[2]);
    threadSafeDelay(20);
  }
}

void breatheOut(int color) {
  int rgbColor[3];
  rgbColor[0] = 0;
  rgbColor[1] = 0;
  rgbColor[2] = 0;
  rgbColor[color] = 255;

  for (int brightness = 255; brightness >= 1; brightness--) {
    if (abortNow) { break; }
    rgbColor[color] = brightness;
    rgb(rgbColor[0], rgbColor[1], rgbColor[2]);
    threadSafeDelay(20);
  }
}

