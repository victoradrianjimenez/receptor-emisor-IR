#include <IRremote.hpp>
#define IR_RECEIVE_PIN 2

int inChar = 0;
uint8_t tRawlen = 0;
uint32_t tCurrentTicks = 0;
uint32_t i = 0;
bool debug = false;
String data;


void setup(){
  Serial.begin(115200);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
}

void loop() {
  if (Serial.available() > 0) {
    inChar = Serial.read();
    if (inChar == 'D'){
      debug = !debug;
      Serial.print("DEBUG=");
      Serial.println(debug);
    }
  }
  if (IrReceiver.decode()) {
      tRawlen = IrReceiver.decodedIRData.rawDataPtr->rawlen;
      tCurrentTicks = IrReceiver.decodedIRData.rawDataPtr->rawbuf[0] * MICROS_PER_TICK;
      data = String(tCurrentTicks);
      for (i = 1; i < tRawlen; i++) {
        tCurrentTicks = IrReceiver.decodedIRData.rawDataPtr->rawbuf[i] * MICROS_PER_TICK;
        data += "," + String(tCurrentTicks);
      }
      Serial.println(data);

      if (debug){
        // Print "old" raw data
        Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); 

        // Print complete received data in one line
        IrReceiver.printIRResultShort(&Serial);

        // Print the statement required to send this data
        IrReceiver.printIRSendUsage(&Serial);

        // Print raw data in a matrix
        IrReceiver.printIRResultRawFormatted(&Serial, true);
      }
      // Enable receiving of the next value
      IrReceiver.resume();
  }

}
