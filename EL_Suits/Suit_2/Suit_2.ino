#include <Conceptinetics.h>
#include <Rdm_Defines.h>
#include <Rdm_Uid.h>

/* DMX_SLAVE_CHANNELS - the total number of channels */
#define DMX_SLAVE_CHANNELS   192
#define SUIT_NO             2

#define MASKS_GLVS        1
#define T_RED             2
#define T_L_BLUE          3
#define T_R_BLUE          4
#define B_L_RED           5
#define B_R_RED           6
#define B_L_BLUE          7
#define B_R_BLUE          8

DMX_Slave dmx_slave ( DMX_SLAVE_CHANNELS );

const int offSet = 4;   // Channel one goes to 5
const int numChannels = 8; // There are 8 channels on the EL shield
const int startPin = 5;

int fcnChannels[] = {1, 17, 33, 49, 65, 81};
int currentFcn = -1;

/* Functions:
    0: turnOnAll
    1: strobe
    2: turnOnRed
    3: turnOnBlue
    4: turnOnTop
    5: turnOnBottom
    6:
    7:
    else: turnOnAll
    -1: all off
*/

/*****************************************************************/
/************************** SETUP ********************************/
/*****************************************************************/

void setup() {

  dmx_slave.enable ();

  dmx_slave.setStartAddress (SUIT_NO);

  pinMode(5, OUTPUT);   // 2(A)
  pinMode(6, OUTPUT);   // 3(B)
  pinMode(7, OUTPUT);   // 4(C)
  pinMode(8, OUTPUT);   // 5(D)
  pinMode(9, OUTPUT);   // 6(E)
  pinMode(10, OUTPUT);  // 7(F)
  pinMode(11, OUTPUT);  // 8(G)
  pinMode(12, OUTPUT);  // 9(H)
}

/*****************************************************************/
/************************** MAIN *********************************/
/*****************************************************************/

void loop()
{

  for (int i = 0; i < (sizeof(fcnChannels)/sizeof(int)); i++) {
    if (dmx_slave.getChannelValue(fcnChannels[i]) > 127 && dmx_slave.getChannelValue(fcnChannels[i]) < 255) {
      switchOn(i);
    }
    else if ( dmx_slave.getChannelValue(fcnChannels[i]) <= 127 && dmx_slave.getChannelValue(fcnChannels[i]) > 0) {
      switchOff(i);
    }
    else {
      remainOn();
    }
  }

}

/*****************************************************************/
/************************ FUNCTIONS ******************************/
/*****************************************************************/

void switchOn(int fcnIndex) {
  mapToOff(currentFcn);
  currentFcn = fcnIndex;
  mapToOn(currentFcn);
}

void switchOff(int fcnIndex) {
  currentFcn = -1;
  mapToOff(fcnIndex);
}

// Essentially allows the strobe to continue when the DMX channel is at max
void remainOn() {
  if (currentFcn == 1) strobe();
}

void mapToOn(int fcnIndex)
{
  switch (fcnIndex) {
    case 0:
      turnOnAll();
      break;
    case 1:
      strobe();
      break;
    case 2:
      turnOnRed();
      break;
    case 3:
      turnOnBlue();
      break;
    case 4:
      turnOnTop();
      break;
    case 5:
      turnOnBottom();
      break;
    // Add more for specific channels
    default:
      turnOnAll();
      break;
  }
}

void mapToOff(int fcnIndex)
{
  switch (fcnIndex) {
    case 0:
      turnOffAll();
      break;
    case 1:
      turnOffAll();
      break;
    case 2:
      turnOffRed();
      break;
    case 3:
      turnOffBlue();
      break;
    case 4:
      turnOffTop();
      break;
    case 5:
      turnOffBottom();
      break;
    // Add more for specific channels
    default:
      turnOffAll();
      break;
  }
}

void turnOn(int ledPinOn)
{
  digitalWrite(ledPinOn + offSet, HIGH);
}


void turnOff(int ledPinOff)
{
  digitalWrite(ledPinOff + offSet, LOW);
}

void turnOnAll()
{
  for (int i = 1; i <= numChannels; i++) {
    turnOn(i);
  }
}

void turnOffAll()
{
  for (int i = 1; i <= numChannels; i++) {
    turnOff(i);
  }
}

void strobe()
{
  for (int i = 1; i <= numChannels; i++) {
    if (dmx_slave.getChannelValue(fcnChannels[1]) <= 127 && dmx_slave.getChannelValue(fcnChannels[1]) > 0) {
      switchOff(1);
    }

    turnOn(i);
    delay(50);
    turnOff(i);
  }
}

void turnOnRed() {
  turnOn(MASKS_GLVS);
  turnOn(T_RED);
  turnOn(B_L_RED);
  turnOn(B_R_RED);
}

void turnOffRed() {
  turnOff(MASKS_GLVS);
  turnOff(T_RED);
  turnOff(B_L_RED);
  turnOff(B_R_RED);
}

void turnOnBlue() {
  turnOn(T_L_BLUE);
  turnOn(T_R_BLUE);
  turnOn(B_L_BLUE);
  turnOn(B_R_BLUE);
}

void turnOffBlue() {
  turnOff(T_L_BLUE);
  turnOff(T_R_BLUE);
  turnOff(B_L_BLUE);
  turnOff(B_R_BLUE);
}

void turnOnTop() {
  turnOn(T_RED);
  turnOn(T_L_BLUE);
  turnOn(T_R_BLUE);
}

void turnOffTop() {
  turnOff(T_RED);
  turnOff(T_L_BLUE);
  turnOff(T_R_BLUE);
}

void turnOnBottom() {
  turnOn(B_L_RED);
  turnOn(B_R_RED);
  turnOn(B_L_BLUE);
  turnOn(B_R_BLUE);
}

void turnOffBottom() {
  turnOff(B_L_RED);
  turnOff(B_R_RED);
  turnOff(B_L_BLUE);
  turnOff(B_R_BLUE);
}

