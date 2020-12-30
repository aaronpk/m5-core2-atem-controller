#include <M5Core2.h>
#include <Fonts/EVA_20px.h>
#include <stdio.h>
#include <WiFi.h>

// Download these from here
// https://github.com/kasperskaarhoj/SKAARHOJ-Open-Engineering/tree/master/ArduinoLibs
#include <SkaarhojPgmspace.h>
#include <ATEMmax.h>
#include <ATEMstd.h>


// Define the IP address of your ATEM switcher
IPAddress switcherIp(10, 11, 200, 9);

// Put your wifi SSID and password here
const char* ssid = "";
const char* password =  "";








#define CAMERA_OFF 0
#define CAMERA_PREVIEW 1
#define CAMERA_PROGRAM 2

#define CAMERA_BUTTON_Y 42
#define MACRO_BUTTON_Y 150

ATEMstd AtemSwitcher;


TouchButton c1 = TouchButton(2,   CAMERA_BUTTON_Y, 72, 72, "c1");
TouchButton c2 = TouchButton(2+72, CAMERA_BUTTON_Y, 72, 72, "c2");
TouchButton c3 = TouchButton(2+72+72, CAMERA_BUTTON_Y, 72, 72, "c3");
TouchButton c4 = TouchButton(2+72+72+72,   CAMERA_BUTTON_Y, 72, 72, "c4");


TouchButton b1 = TouchButton(2,   MACRO_BUTTON_Y, 72, 72, "b1");
TouchButton b2 = TouchButton(2+72, MACRO_BUTTON_Y, 72, 72, "b2");
TouchButton b3 = TouchButton(2+72+72, MACRO_BUTTON_Y, 72, 72, "b3");
TouchButton b4 = TouchButton(2+72+72+72,   MACRO_BUTTON_Y, 72, 72, "b4");


void buttonWasPressed(TouchEvent& e) {
  TouchButton& b = *e.button;

  char buttonType = b.name[0];
  int buttonNumber = b.name[1]-'0';

  if(buttonType == 'c') {
    AtemSwitcher.changeProgramInput(buttonNumber);
  } else {
    drawButton(buttonNumber, b.isPressed(), false);
    AtemSwitcher.setMacroAction(buttonNumber-1, 0); 
  }
}

void drawCameraButton(int buttonNumber, int state) {
  int font = 1;
  M5.Lcd.setTextSize(8);
  
  M5.Lcd.setTextColor(state == CAMERA_OFF ? WHITE : BLACK);

  int buttonColor;
  switch(state) {
    case CAMERA_OFF:
      buttonColor = TFT_DARKGREY; break;
    case CAMERA_PREVIEW:
      buttonColor = TFT_GREEN; break;
    case CAMERA_PROGRAM:
      buttonColor = TFT_RED; break;
  }

  //char* cameraName = AtemSwitcher.getInputShortName(buttonNumber-1);
  
  switch(buttonNumber) {
    case 1:
      M5.Lcd.fillRoundRect(2, CAMERA_BUTTON_Y, 72, 72, 6, buttonColor);
      M5.Lcd.drawCentreString("1", 2+2+(72/2), 42+10, font);
      break;
    case 2:
      M5.Lcd.fillRoundRect(2+72+2, CAMERA_BUTTON_Y, 72, 72, 6, buttonColor);
      M5.Lcd.drawCentreString("2", 2+2+72+2+(72/2), 42+10, font);
      break;
    case 3:
      M5.Lcd.fillRoundRect(2+72+2+72+2, CAMERA_BUTTON_Y, 72, 72, 6, buttonColor);
      M5.Lcd.drawCentreString("3", 2+2+72+2+72+2+(72/2), 42+10, font);
      break;
    case 4:
      M5.Lcd.fillRoundRect(2+72+2+72+2+72+2, CAMERA_BUTTON_Y, 72, 72, 6, buttonColor);
      M5.Lcd.drawCentreString("4", 2+2+72+2+72+2+72+2+(72/2), 42+10, font);
      break;
  }
}

void drawButton(int buttonNumber, bool isPressed, bool isRunning) {
  int font = 1;
  M5.Lcd.setTextSize(8);
  
  M5.Lcd.setTextColor(isPressed || isRunning ? BLACK : WHITE);

  int buttonColor = isPressed ? WHITE : (isRunning ? TFT_RED : TFT_DARKGREY);
  
  switch(buttonNumber) {
    case 1:
      M5.Lcd.fillRoundRect(2, MACRO_BUTTON_Y, 72, 72, 6, buttonColor);
      M5.Lcd.drawCentreString("1", 2+2+(72/2), MACRO_BUTTON_Y+10, font);
      break;
    case 2:
      M5.Lcd.fillRoundRect(2+72+2, MACRO_BUTTON_Y, 72, 72, 6, buttonColor);
      M5.Lcd.drawCentreString("2", 2+2+72+2+(72/2), MACRO_BUTTON_Y+10, font);
      break;
    case 3:
      M5.Lcd.fillRoundRect(2+72+2+72+2, MACRO_BUTTON_Y, 72, 72, 6, buttonColor);
      M5.Lcd.drawCentreString("3", 2+2+72+2+72+2+(72/2), MACRO_BUTTON_Y+10, font);
      break;
    case 4:
      M5.Lcd.fillRoundRect(2+72+2+72+2+72+2, MACRO_BUTTON_Y, 72, 72, 6, buttonColor);
      M5.Lcd.drawCentreString("4", 2+2+72+2+72+2+72+2+(72/2), MACRO_BUTTON_Y+10, font);
      break;
  }
}

void setupButtons() {
  M5.Touch.addHandler(buttonWasPressed, TE_BTNONLY + TE_TOUCH + TE_RELEASE);

  M5.Lcd.fillScreen(BLACK);

  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.drawString("CAMERAS", 6, CAMERA_BUTTON_Y-20, 1);
  M5.Lcd.drawString("MACROS", 6, MACRO_BUTTON_Y-20, 1);

  drawButton(1, false, false);
  drawButton(2, false, false);
  drawButton(3, false, false);
  drawButton(4, false, false);
  
  drawCameraButton(1, CAMERA_OFF);
  drawCameraButton(2, CAMERA_OFF);
  drawCameraButton(3, CAMERA_OFF);
  drawCameraButton(4, CAMERA_OFF);
}

void setup() {
  M5.begin(true, true, false, true);
  
  Serial.begin(115200);

  Serial.println("About to connect to wifi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  AtemSwitcher.begin(switcherIp);
  AtemSwitcher.serialOutput(0x80);
  AtemSwitcher.connect();

  setupButtons();
}

int currentRunningMacro = -1;
int tallyStates[4];

void loop() {
  // TouchPoint_t pos= M5.Touch.getPressPoint();

  /*
  // Bottom buttons
  if(pos.y > 240) {
    if(pos.x < 109) // Left
      M5.Lcd.setTextColor(RED);
    else if(pos.x > 218) // Middle
      M5.Lcd.setTextColor(BLUE);
    else if(pos.x >= 109 && pos.x <= 218) // Right
      M5.Lcd.setTextColor(GREEN);
  }
  */

  M5.update();
  AtemSwitcher.runLoop();

  if(AtemSwitcher.getMacroRunStatusState() && 0x01 == 1) {
    int index = AtemSwitcher.getMacroRunStatusIndex();
    if(index != currentRunningMacro) {
      drawButton(index + 1, false, true);
    }
    currentRunningMacro = index;
  } else {
    if(currentRunningMacro > -1) {
      drawButton(currentRunningMacro + 1, false, false);
      currentRunningMacro = -1;
    }
  }

  for(int i=1; i<=4; i++) {
    int currentTallyState = AtemSwitcher.getProgramTally(i) ? CAMERA_PROGRAM : (AtemSwitcher.getPreviewTally(i) ? CAMERA_PREVIEW : CAMERA_OFF);

    if(currentTallyState != tallyStates[i-1]) {
      drawCameraButton(i, currentTallyState);
    }
    
    tallyStates[i-1] = currentTallyState;
  }
  
}
