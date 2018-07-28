#include <SocketIoClient.h>
#include <ESP8266WiFi.h>


char ssid[] = "YOUR_SSID";
char pass[] = "YOUR_PASS";
SocketIoClient socket;
const int PIN_RELAY1 = 0;


void toggleEvent(const char* a , size_t b){
   int currentState = digitalRead(PIN_RELAY1);
   if(currentState == LOW) {
      digitalWrite(PIN_RELAY1 , HIGH);
      socket.emit("has_turn_off"); 
   }else{
      digitalWrite(PIN_RELAY1 , LOW);
      socket.emit("has_turn_on");   
   }
}


void connectEvent(const char* a , size_t b){
   Serial.println("connectEvent called");
   socket.emit("join_arduino");
}

void disconnectEvent(const char* a , size_t b){
   Serial.println("disconnectEvent called");
   socket.emit("leave_arduino");
}


void checkStateEvent(const char* a , size_t b){
   Serial.println("checkStateEvent called");
   int currentState = digitalRead(PIN_RELAY1);
   if(currentState == LOW){
      socket.emit("has_turn_on"); 
   }else{
      socket.emit("has_turn_off");   
   }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid,pass);
  while(WiFi.status() != WL_CONNECTED){
      Serial.println("Wifi not connected");
      delay(1000);
  }
  pinMode(PIN_RELAY1 , OUTPUT);
  digitalWrite(PIN_RELAY1 , HIGH);
  socket.on("connect",connectEvent);
  socket.on("toggle",toggleEvent);
  socket.on("disconnect",disconnectEvent);
  socket.on("check_state",checkStateEvent);
  socket.beginSSL("wemos-lamp-ioserver1.herokuapp.com",443);
}

void loop() {
  socket.loop();
}
