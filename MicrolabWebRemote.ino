#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRtimer.h>
#include <IRutils.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Add this library: https://github.com/markszabo/IRremoteESP8266
#include <IRremoteESP8266.h>

#define IR_SEND_PIN D2

#define DELAY_BETWEEN_COMMANDS 500

IRsend irsend(IR_SEND_PIN);

const char* ssid = "ubnt";
const char* password = "59wfgkz59";

ESP8266WebServer server(80);

const int led = BUILTIN_LED;

String rowDiv = "    <div class=\"row\" style=\"padding-bottom:1em\">\n";
String endDiv = "    </div>\n";

String generateButton(String colSize, String id, String text, String url, String buttonType) {

  return  "<div class=\"" + colSize + "\" style=\"text-align: center\">\n" +
          "    <button id=\"" + id + "\" type=\"button\" class=\"btn " + buttonType + "\" style=\"width: 100%\" onclick='makeAjaxCall(\"" + url + "\")'>" + text + "</button>\n" +
          "</div>\n";
}

void handleRoot() {
  digitalWrite(led, 0);
  String website = "<!DOCTYPE html>\n";
  website = website + "<html>\n";
  website = website + "  <head>\n";
  website = website + "    <meta charset=\"utf-8\">\n";
  website = website + "    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
  website = website + "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
  website = website + "    <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">\n";
  website = website + "  </head>\n";
  website = website + "  <body>\n";
  website = website + "    <div class=\"container-fluid\">\n";
  website = website + rowDiv;
  website = website + endDiv;
  website = website + rowDiv;
  website = website + generateButton("col-xs-3", "power", "Power", "power", "btn-danger");
  website = website + generateButton("col-xs-3", "mute", "MUTE", "mute", "btn-success");
  website = website + endDiv;
  website = website + rowDiv;
  website = website + generateButton("col-xs-3", "inputsel", "INPUT SEL", "inputsel", "");
  website = website + generateButton("col-xs-3", "3d", "3D", "3d", "");
  website = website + endDiv;
  website = website + rowDiv;
  website = website + generateButton("col-xs-2", "balancer", "BALANCE-R", "balancer", "");
  website = website + generateButton("col-xs-2", "trebleup", "TREBLE+", "trebleup", "");
  website = website + generateButton("col-xs-2", "bassup", "BASS+", "bassup", "");
  website = website + endDiv;
  website = website + rowDiv;
  website = website + generateButton("col-xs-2", "balancel", "BALANCE-L", "balancel", "");
  website = website + generateButton("col-xs-2", "trebledown", "TREBLE-", "trebledown", "");
  website = website + generateButton("col-xs-2", "bassdown", "BASS-", "bassdown", "");
  website = website + endDiv;
  website = website + rowDiv;
  website = website + endDiv;
  website = website + rowDiv;
  website = website + generateButton("col-xs-3", "down","VOLUME-", "down", "");
  website = website + generateButton("col-xs-3", "up","VOLUME+", "up", "");
  website = website + endDiv;
  website = website + rowDiv;
  website = website + endDiv;
  website = website + rowDiv;
  website = website + endDiv;
  website = website + rowDiv;
  website = website + generateButton("col-xs-6", "myconfig","My config", "myconfig", "btn-info");
  website = website + "    <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js\"></script>\n";
  website = website + "    <script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\"></script>\n";
  website = website + "    <script> function makeAjaxCall(url){$.ajax({\"url\": url})}</script>\n";
  website = website + "  </body>\n";
  website = website + "</html>\n";

  server.send(200, "text/html", website);
  digitalWrite(led, 1);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 1);
}

void setup(void){
  irsend.begin();
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS Responder Started");
  }

  server.on("/", handleRoot);

  server.on("/power", [](){
    Serial.println("Power");
    irsend.sendNEC(0xFF00FF, 32);
    server.send(200, "text/plain", "Power on/off");
  });

  server.on("/mute", [](){
    Serial.println("MUTE");
    irsend.sendNEC(0xFF40BF, 32);
    server.send(200, "text/plain", "MUTE");
  });

  server.on("/inputsel", [](){
    Serial.println("INPUT SEL");
    irsend.sendNEC(0xFF20DF, 32);
    server.send(200, "text/plain", "INPUT SEL");
  });

  server.on("/3d", [](){
    Serial.println("3D");
    irsend.sendNEC(0xFF609F, 32);
    server.send(200, "text/plain", "3D");
  });

  server.on("/bassup", [](){
    Serial.println("BASS+");
    irsend.sendNEC(0xFF50AF, 32);
    server.send(200, "text/plain", "BASS+");
  });

  server.on("/bassdown", [](){
    Serial.println("BASS-");
    irsend.sendNEC(0xFF708F, 32);
    server.send(200, "text/plain", "BASS-");
  });

  server.on("/trebleup", [](){
    Serial.println("TREBLE+");
    irsend.sendNEC(0xFF906F, 32);
    server.send(200, "text/plain", "TREBLE+");
  });

  server.on("/trebledown", [](){
    Serial.println("TREBLE-");
    irsend.sendNEC(0xFFB04F, 32);
    server.send(200, "text/plain", "TREBLE-");
  });

  server.on("/balancer", [](){
    Serial.println("BALANCE-R");
    irsend.sendNEC(0xFF10EF, 32);
    server.send(200, "text/plain", "BALANCE-R");
  });

  server.on("/balancel", [](){
    Serial.println("BALANCE-L");
    irsend.sendNEC(0xFF30CF, 32);
    server.send(200, "text/plain", "BALANCE-L");
  });

  server.on("/up", [](){
    Serial.println("VOLUME+");
    irsend.sendNEC(0xFF8877, 32);
    server.send(200, "text/plain", "VOLUME+");
  });

  server.on("/down", [](){
    Serial.println("VOLUME-");
    irsend.sendNEC(0xFF08F7, 32);
    server.send(200, "text/plain", "VOLUME-");
  });

// My configuration is: 3D ON, TREBLE 03, BASS 04, 3D OFF
  server.on("/myconfig", [](){
    Serial.println("My config");
    irsend.sendNEC(0xFF609F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFFB04F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFFB04F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFFB04F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFFB04F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFFB04F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFFB04F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFFB04F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFFB04F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFFB04F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFFB04F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFFB04F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFFB04F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFFB04F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF906F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF906F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF906F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF906F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF906F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF906F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF906F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF906F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF906F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF906F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF708F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF708F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF708F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF708F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF708F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF708F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF708F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF708F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF708F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF708F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF708F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF708F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF708F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF708F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF50AF, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF50AF, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF50AF, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF50AF, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF50AF, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF50AF, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF50AF, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF50AF, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF50AF, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF50AF, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF50AF, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    irsend.sendNEC(0xFF609F, 32);
    delay(DELAY_BETWEEN_COMMANDS);
    server.send(200, "text/plain", "My config");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP Server Started");
}

void loop(void){
  server.handleClient();
}
