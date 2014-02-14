#include <SoftwareSerial.h>

const int TX_BT = 10;
const int RX_BT = 11;

const int PIN_RED = 5;
const int PIN_GREEN = 6;

SoftwareSerial btSerial(TX_BT, RX_BT);

//Process the incoming command from Windows Phone.
void processCommand(char* command) {     //Takes in the command buffer. char* ch = new char; creates memory for ONE character, and assigns it to variable ch. 
//  Serial.print(command);
//  Serial.write(command);
//  sendCoOrdsToAPM(command);
  if(strcmp(command,"-7.222450000") == 0) {   //strcmp : string compare. If string1 = string2 the do the following. A 0 result means there equal
    //digitalWrite(PIN_RED, HIGH);
    //delay(5000);
    //digitalWrite(PIN_RED, LOW);
  } else if(strcmp(command,"TURN_ON_GREEN") == 0) {
    digitalWrite(PIN_GREEN, HIGH);
    delay(5000);
    digitalWrite(PIN_GREEN, LOW);
  }  
}

//Send a message back to the Windows Phone.
void sendMessage(char* message) {
  int messageLen = strlen(message); //strlen : gets the string length. "This sentance would be result in"   32
  if(messageLen < 256) {  
    btSerial.write(messageLen);
    btSerial.print(message);
  }
}

void sendCoOrdsToAPM(char* command){
//void sendMessage(char* coOrds) {
  int messageLen = strlen(command); //strlen : gets the string length. "This sentance would be result in"   32
  if(messageLen < 256) {
    Serial.write(messageLen);
    //Serial.println('command');
    } 
}

//Setup Arduino function
void setup() {
  Serial.begin(57600);
  Serial.println("USB Connected");
  btSerial.begin(9600);
  
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT); 
}

//Loop Arduino function
void loop() {
  if(btSerial.available()) {
      int commandSize = (int)btSerial.read(); //read first byte from serial. This should contain the size of the command. It's set on the phone.
      Serial.write(commandSize);
      char command[commandSize];
      int commandPos = 0;
      while(commandPos < commandSize) {
        if(btSerial.available()) {
          command[commandPos] = (char)btSerial.read();  //Keep reading bluetooth data while commandPos is 0
          Serial.write(command[commandPos]);
          commandPos++;
        }
      }
      command[commandPos] = 0;
      processCommand(command);
      
  }
}

