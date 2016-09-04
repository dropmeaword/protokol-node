#include "config.h"
#include <EEPROM.h>
#include "guid.h"
#include <CmdMessenger.h> // serial messaging library
#include <InputDebounce.h> // debounce lib

enum
{
  kReset = 10,
  kRename = 20
};
char field_separator   = ' ';
char command_separator = ';';
// attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial, field_separator, command_separator);

// counter button configuration
static const int pinSwitch = 2;
static InputDebounce btn;

int guidChanged = 0;
int ackSent = 0;
unsigned long manualId = 0;
unsigned long nodeid = 0;

void on_reset()
{
  if(DEBUG) Serial.println("resetting");
  guid_clear();
  guid_init();
}

void on_unknown()
{
  Serial.println("unknown command");
  print_ack_guid();
}

void attachCommandCallbacks() {
  cmdMessenger.attach(kReset, on_reset);
  cmdMessenger.attach(on_unknown);
}

void send_ack() {
    ackSent = 0;
}

void btn_pressed()
{
    manualId += 1;
    guid_set( manualId );
}

void btn_released()
{
    print_ack_guid();
    guidChanged = 0;
}

void btn_hold(unsigned long duration)
{
  if( (duration > AUTOCONF_TIMEOUT) && (!guidChanged)) {
    guid_randomize();
    guidChanged = 1;
    manualId = 0; // reset manual ID counter
  }
}

void setup() {
  Serial.begin(SERIAL_DATARATE);
  guid_init();

  btn.registerCallbacks(btn_pressed, btn_released, btn_hold);
  btn.setup(pinSwitch, 20, InputDebounce::PIM_EXT_PULL_DOWN_RES); // 20ms

  cmdMessenger.printLfCr();
  attachCommandCallbacks();
}

void serialPump() {
  // Process incoming serial data, and perform callbacks
  cmdMessenger.feedinSerialData();
}

void print_ack_guid() {
  Serial.print("ACK ");
  for(int i = 4; i < GUIDLENGTH; i++) {
    //Serial.print( GUID[i], HEX );
    print_byte( GUID[i] );
    if(i < (GUIDLENGTH-1) ) { Serial.print( ':' ); }
  }
  Serial.println();
}

void loop()
{
  unsigned long now = millis();
  btn.process(now);

  serialPump();

  if(!ackSent || guidChanged) {
      print_ack_guid();
      ackSent = 1;
  }

  
  delay(20);
}
