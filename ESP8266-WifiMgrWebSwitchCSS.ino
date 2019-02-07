#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid="ESP8266";
const char *password = "1234567890";

const int mPin=D7;
boolean swStatus=false;
String strSwStatus;
String strButtonLabel;


ESP8266WebServer server(80);

String getHtmlPage(){
  String css;
  if (swStatus){
    Serial.println("Switch is On");
    strSwStatus="On";
    strButtonLabel="Turn Off";
    css="<style>.button {background-color:silver;border:none;border-radius:4px;color:red;padding:7px;text-align:center;text-decoration:none;text-decoration:none;}</style>";
  } else {
    Serial.println("Switch is Off");
    strSwStatus="Off";
    strButtonLabel="Turn On";
    css="<style>.button {background-color:silver;border:none;border-radius:4px;color:green;padding:7px;text-align:center;text-decoration:none;text-decoration:none;}</style>";
  }

  

  String strHtml="<!DOCTYPE html><html><head>";
  strHtml+=css;
  strHtml+="<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">";
  strHtml+="<meta name=\"viewport\" content=\"user-scalable=no, initial-scale=1, maximum-scale=1, ";
  strHtml+="minimum-scale=1, width=device-width, height=device-height\">";
  strHtml+="<title>Controller</title>";
  strHtml+="</head><body><h1>Controller/ควบคุม</h1>";
  strHtml+="<h3>Switch Status:";
  strHtml+=strSwStatus;
  strHtml+="</h3>";
  strHtml+="<br>";
  strHtml+="<a href='./toggle' class='button'>";
  strHtml+=strButtonLabel;
  strHtml+="</a>";
  strHtml+="</body></html>";
  return strHtml;
}

void handleRoot(){
  server.send(200, "text/html", getHtmlPage());
}

void toggleSW(){
  if (swStatus){
    swStatus=false;
    analogWrite(mPin, 0);
  } else {
    swStatus=true;
    analogWrite(mPin, 700);
  }
  server.send(200, "text/html", getHtmlPage());
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);
  IPAddress myIP=WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  pinMode(mPin, OUTPUT);
  analogWrite(mPin, 0);

  server.on("/", handleRoot);

  server.on("/toggle", toggleSW);


  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}
