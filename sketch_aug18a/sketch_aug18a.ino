#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
 
/* Put your SSID & Password */
const char* ssid = "NaeemGlasses";  // Enter SSID here
const char* password = "123456789";  //Enter Password here
 
/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);
ESP8266WebServer server(80);
 
/*-------------------------*/
uint8_t glassPin = 2;
uint8_t glassStatus = 0;
int ledState = LOW;
const long interval = 83;
int previousMillis=0;
 /*-------------------------*/
void setup() {
  Serial.begin(115200);
  pinMode(glassPin, OUTPUT);
  Serial.print("Setting soft-AP ... ");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ssid, password);
//  wifi_set_sleep_type(LIGHT_SLEEP_T);
  server.on("/", handle_OnConnect);
  server.on("/glassOn", handle_glassOn);
  server.on("/glassOff", handle_glassOff);
  server.on("/glassOnOffTimer", handle_glassOnOffTimer);
  server.onNotFound(handle_NotFound);
  server.begin();
}


void loop() {
 unsigned long currentMillis = millis();
  server.handleClient();
  if(glassStatus==0)
  {digitalWrite(glassPin, LOW);}
  else if(glassStatus==2)
  {
if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(glassPin, ledState);
  }
}
    else 
  {digitalWrite(glassPin, HIGH);}

}


 
void handle_OnConnect() {
 // LED1status = LOW;
  glassStatus = 0;
 // Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(glassStatus)); 
}

void handle_glassOn() {
  glassStatus = 1;
 // Serial.println("GPIO6 Status: ON");
  server.send(200, "text/html", SendHTML(glassStatus)); 
}
 
void handle_glassOnOffTimer() {
  glassStatus = 2;
 // Serial.println("GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(glassStatus)); 
}
void handle_glassOff() {
  glassStatus = 0;
 // Serial.println("GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(glassStatus)); 
}
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
 
String SendHTML(uint8_t glassStatus){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +=".button-timer {background-color: #BC8F8F;}\n";
  ptr +=".button-timer:active {background-color: #FFC0CB;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Smart Sports Glasses</h1>\n";
  ptr +="<h3>M.NaeemAbadi</h3>\n";
   
  if(glassStatus==1)
  {ptr +="<p>glassStatus: ON</p><a class=\"button button-timer\" href=\"/glassOnOffTimer\">TIMER</a>\n";}
  else if (glassStatus==2)
  {ptr +="<p>glassStatus: TIMER</p><a class=\"button button-off\" href=\"/glassOff\">OFF</a>\n";}
  else 
  {ptr += "<p>glassStatus: OFF</p><a class=\"button button-on\" href=\"/glassOn\">ON</a>\n";}
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
