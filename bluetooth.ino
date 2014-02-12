#include <SoftwareSerial.h>

const int TX_BT = 10;
const int RX_BT = 11;

const int PIN_RED = 5;
const int PIN_GREEN = 6;
const int PIN_YELLOW = 7;

const int PIN_PROXIMITY_SENSOR = 3;

SoftwareSerial btSerial(TX_BT, RX_BT);

//Frequency to send periodic messages to Windows Phone, in milliseconds.
const unsigned long periodicMessageFrequency = 5000;
unsigned long time = 0;

//Process the incoming command from Windows Phone.
void processCommand(char* command) {
  if(strcmp(command,"TURN_ON_RED") == 0) {
    digitalWrite(PIN_RED, HIGH);
    sendMessage("LED_RED:ON");
  } else if(strcmp(command,"TURN_OFF_RED") == 0) {
    digitalWrite(PIN_RED, LOW);
    sendMessage("LED_RED:OFF");
  } else if(strcmp(command,"TURN_ON_GREEN") == 0) {
    digitalWrite(PIN_GREEN, HIGH);
    sendMessage("LED_GREEN:ON");
  } else if(strcmp(command,"TURN_OFF_GREEN") == 0) {
    digitalWrite(PIN_GREEN, LOW);
    sendMessage("LED_GREEN:OFF");
  } else if(strcmp(command,"TURN_ON_YELLOW") == 0) {
    digitalWrite(PIN_YELLOW, HIGH);
    sendMessage("LED_YELLOW:ON");
  } else if(strcmp(command,"TURN_OFF_YELLOW") == 0) {
    digitalWrite(PIN_YELLOW, LOW);
    sendMessage("LED_YELLOW:OFF");    
  }
}

//Send a message back to the Windows Phone.
void sendMessage(char* message) {
  int messageLen = strlen(message);
  if(messageLen < 256) {  
    btSerial.write(messageLen);
    btSerial.print(message);
  }
}

//Send a set of periodic messages to the Windows Phone.
//This message could be a sensor data, like a thermometer data.
void sendPeriodicMessages() {
  int valProximitySensor = digitalRead(PIN_PROXIMITY_SENSOR);
  if(valProximitySensor == HIGH) {
    sendMessage("PROXIMITY:NOT_DETECTED");
  } else {
    sendMessage("PROXIMITY:DETECTED");
  }
}

//Setup Arduino function
void setup() {
  Serial.begin(9600);
  Serial.println("USB Connected");
  btSerial.begin(9600);
  
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_YELLOW, OUTPUT);  
  
  pinMode(PIN_PROXIMITY_SENSOR, INPUT);
}

//Loop Arduino function
void loop() {
  if(btSerial.available()) {
      int commandSize = (int)btSerial.read();
      char command[commandSize];
      int commandPos = 0;
      while(commandPos < commandSize) {
        if(btSerial.available()) {
          command[commandPos] = (char)btSerial.read();
          commandPos++;
        }
      }
      command[commandPos] = 0;
      processCommand(command);
  }
  unsigned long currentTime = millis();
  if((currentTime - time) > periodicMessageFrequency) {
    sendPeriodicMessages();
    time = currentTime;
  }
}


