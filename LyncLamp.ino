#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

//User States
#define STATE_GREEN 1
#define STATE_ORANGE 2
#define STATE_RED 3
#define STATE_NONE 0

//State Colors
#define COLOR_GREEN 1
#define COLOR_ORANGE 2
#define COLOR_RED 3

//Button Pins
#define GREEN_BTN_PIN 2
#define ORANGE_BTN_PIN 3
#define RED_BTN_PIN 4

//Time over LED pin
#define CONCENTRATION_TIME_OVER_LED_PIN 8

//Status LED ring 
#define STATUS_LED_PIN 6
#define NUM_LEDS 12
#define BRIGHTNESS 20

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, STATUS_LED_PIN, NEO_GRBW + NEO_KHZ800);

bool greenBtnPressed = 0;
bool orangeBtnPressed = 0;
bool redBtnPressed = 0;

bool lastGreenBtnPressed = 0;
bool lastOrangeBtnPressed = 0;
bool lastRedBtnPressed = 0;

long concentrationTime = 0;
long lastConcentrationTime = 0;
const long MAX_CONCENTRATION_TIME = 10000;
bool isOverConcentrationTime = false;

int requestedState = 0;

int userState = STATE_GREEN;
int lastUserState = STATE_GREEN;
bool userHasPermissions = true;


void setup() {
  pinMode(GREEN_BTN_PIN, INPUT_PULLUP);
  pinMode(ORANGE_BTN_PIN, INPUT_PULLUP);
  pinMode(RED_BTN_PIN, INPUT_PULLUP);
  pinMode(CONCENTRATION_TIME_OVER_LED_PIN, OUTPUT);

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  setStatusLEDs(COLOR_GREEN);

  Serial.begin(9600);
}

void loop() {
  //Update time
  concentrationTime = updateConcentrationTime();
  isOverConcentrationTime = isOver(concentrationTime, MAX_CONCENTRATION_TIME);

  //Indicate concentration time with red LED
  showConcentrationTimeOver(isOverConcentrationTime);

  userHasPermissions = (!isOverConcentrationTime);

  requestedState = checkUserInput();
  userState = changeState(requestedState, userHasPermissions);

  //Show state on LED ring
  showState(userState);

  lastUserState = userState;
  delay(5);
}


///Functions///

long updateConcentrationTime() {
  long returnTime = concentrationTime;
  int now = millis();

  if (lastUserState == STATE_RED) {
    returnTime += (now - lastConcentrationTime);
    lastConcentrationTime = now;
  }

  else {
    lastConcentrationTime = now;
  }
  
  return returnTime;
}


bool isOver(long timeToTest, long maxTime) {
  if (timeToTest >= maxTime)
    return true;
  else
    return false;
}


bool isPressed(int btnPin, bool *lastPressed) {
  bool currentlyPressed = !(digitalRead(btnPin));
  if (currentlyPressed && !(*lastPressed)) {
    //button pressed
    *lastPressed = currentlyPressed;
    return true;
  }
  else {
    *lastPressed = currentlyPressed;
    return false;
  }
}


int checkUserInput() {
  bool greenBtnPressed = isPressed(GREEN_BTN_PIN, &lastGreenBtnPressed);
  bool orangeBtnPressed = isPressed(ORANGE_BTN_PIN, &lastOrangeBtnPressed);
  bool redBtnPressed = isPressed(RED_BTN_PIN, &lastRedBtnPressed);

  if (greenBtnPressed) {
    Serial.println("Green pressed");
    return STATE_GREEN;
  }

  else if (orangeBtnPressed) {
    Serial.println("Orange pressed");
    return STATE_ORANGE;
  }

  else if (redBtnPressed) {
    Serial.println("Red pressed");
    return STATE_RED;
  }

  //no button pressed
  else {
    return STATE_NONE;
  }
}


int changeState(int reqState,  bool hasPermissions){
  if(hasPermissions){
    if(reqState == STATE_NONE)
      return lastUserState;
    else
      return reqState;
  }

  else{ //no permissions
    if(reqState == STATE_GREEN){
      return STATE_GREEN;
    }

    else if(reqState == STATE_ORANGE){
      return STATE_ORANGE;
    }

    else if(reqState == STATE_RED){
      return lastUserState;
    }

    else{ //STATE_NONE
      if(userState == STATE_RED)
        return STATE_GREEN;
      else
        return userState;
      
    }
  }
}


void setStatusLEDs(int colorSelector){
  for(int i = 0; i<strip.numPixels(); i++){
    if(colorSelector == COLOR_GREEN){
      strip.setPixelColor(i, 0,255,0,0);
    }
    else if(colorSelector == COLOR_ORANGE){
      strip.setPixelColor(i, 255,255,0,0);
    }
    else{ //COLOR_RED
      strip.setPixelColor(i, 255,0,0,0);
    }
  }
  strip.show();
}


void showState(int state){
  if(state == STATE_GREEN){
    setStatusLEDs(COLOR_GREEN);
  }
  else if (state == STATE_ORANGE){
    setStatusLEDs(COLOR_ORANGE);
  }
  else{ //STATE_RED
    setStatusLEDs(COLOR_RED);
  }
}

void showConcentrationTimeOver(bool isOverTime){
  if(isOverTime)
    digitalWrite(CONCENTRATION_TIME_OVER_LED_PIN, HIGH);
  else
    digitalWrite(CONCENTRATION_TIME_OVER_LED_PIN, LOW);
}
  
