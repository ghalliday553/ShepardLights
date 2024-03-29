#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,2);
IPAddress subnet(255,255,255,0);


String html = "\n <!DOCTYPE html>\n <html>\n \n <head>\n   <title>Shepard Lights</title>\n   <style>\n      .button {\n         background-color: #4CAF50; /* Green */\n         border: none;\n         color: white;\n         padding: 40px 32px;\n         margin-top: 15px;\n         margin-bottom: 15px;\n         text-align: center;\n         text-decoration: none;\n         display: inline-block;\n         font-size: 30px;\n      }\n   </style>\n </head>\n \n <body>\n    <div style=\"display: flex; flex-direction: column; justify-content: space-between\">\n         <button class = \"button\" style = \"background-color: red\" type=\"button\" onclick=\"off()\">Off</button>\n         <button class = \"button\" type=\"button\" onclick=\"Passion()\">Passion</button>\n         <button class = \"button\" type=\"button\" onclick=\"Reapers()\">Reapers</button>\n         <button class = \"button\" type=\"button\" onclick=\"PickMeUp()\">Pick Me Up</button> \n         <button class = \"button\" type=\"button\" onclick=\"Introspective()\">Introspective</button>\n         <button class = \"button\" type=\"button\" onclick=\"Lake()\">The Lake</button>\n         <button class = \"button\" type=\"button\" onclick=\"Clouds()\">Head in the Clouds</button>\n         <button class = \"button\" type=\"button\" onclick=\"Uncanadian()\">Uncanadian</button>\n         <button class = \"button\" type=\"button\" onclick=\"Wildside()\">Wildside</button>    \n    </div>\n    <H1 id=\"currentSong\">\n      Current Song is: None\n    </H1>\n </body>\n <script>\n     function off() {\n        var offRequest = new XMLHttpRequest();\n        offRequest.open( \"GET\", \"/off\", false ); // false for synchronous request\n        offRequest.send( null );\n        document.getElementById(\"currentSong\").innerHTML = \"Current Song is: Off\";\n        return offRequest.responseText;\n     }\n     function Passion() {\n        var offRequest = new XMLHttpRequest();\n        offRequest.open( \"GET\", \"/passion\", false ); // false for synchronous request\n        offRequest.send( null );\n        document.getElementById(\"currentSong\").innerHTML = \"Current Song is: Passion\";\n        return offRequest.responseText;\n     }\n     function Reapers() {\n        var offRequest = new XMLHttpRequest();\n        offRequest.open( \"GET\", \"/reapers\", false ); // false for synchronous request\n        offRequest.send( null );\n        document.getElementById(\"currentSong\").innerHTML = \"Current Song is: Reapers\";\n        return offRequest.responseText;\n     }\n     function PickMeUp() {\n        var offRequest = new XMLHttpRequest();\n        offRequest.open( \"GET\", \"/pick\", false ); // false for synchronous request\n        offRequest.send( null );\n        document.getElementById(\"currentSong\").innerHTML = \"Current Song is: Pick Me Up\";\n        return offRequest.responseText;\n     }\n     function Introspective() {\n        var offRequest = new XMLHttpRequest();\n        offRequest.open( \"GET\", \"/intro\", false ); // false for synchronous request\n        offRequest.send( null );\n        document.getElementById(\"currentSong\").innerHTML = \"Current Song is: Introspective\";\n        return offRequest.responseText;\n     }\n     function Lake() {\n        var offRequest = new XMLHttpRequest();\n        offRequest.open( \"GET\", \"/lake\", false ); // false for synchronous request\n        offRequest.send( null );\n        document.getElementById(\"currentSong\").innerHTML = \"Current Song is: The Lake\";\n        return offRequest.responseText;\n     }\n     function Clouds() {\n        var offRequest = new XMLHttpRequest();\n        offRequest.open( \"GET\", \"/clouds\", false ); // false for synchronous request\n        offRequest.send( null );\n        document.getElementById(\"currentSong\").innerHTML = \"Current Song is: Head in my Dad\";\n        return offRequest.responseText;\n     }\n     function Uncanadian() {\n        var offRequest = new XMLHttpRequest();\n        offRequest.open( \"GET\", \"/uncanadian\", false ); // false for synchronous request\n        offRequest.send( null );\n        document.getElementById(\"currentSong\").innerHTML = \"Current Song is: Uncanadian\";\n        return offRequest.responseText;\n     }\n     function Wildside() {\n        var offRequest = new XMLHttpRequest();\n        offRequest.open( \"GET\", \"/wildside\", false ); // false for synchronous request\n        offRequest.send( null );\n        document.getElementById(\"currentSong\").innerHTML = \"Current Song is: Wildside\";\n        return offRequest.responseText;\n     }\n </script>\n </html>";

void handleRoot() {
  server.send(200, "text/html", html );
}

void handleOff() {
  Serial.write("off");
  server.send(200, "text/html", html );
}

void handlePassion() {
  Serial.write("passion");
  server.send(200, "text/html", html );
}

void handleReapers() {
  Serial.write("reapers");
  server.send(200, "text/html", html );
}

void handlePickMeUp() {
  Serial.write("pick");
  server.send(200, "text/html", html );
}

void handleIntrospective() {
  Serial.write("intro");
  server.send(200, "text/html", html );
}

void handleLake() {
  Serial.write("lake");
  server.send(200, "text/html", html );
}

void handleClouds() {
  Serial.write("clouds");
  server.send(200, "text/html", html );
}

void handleUncanadian() {
  Serial.write("uncanadian");
  server.send(200, "text/html", html );
}

void handleWildside() {
  Serial.write("wildside");
  server.send(200, "text/html", html );
}

void setup(void) {
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP("test");
  
  server.on("/", handleRoot);
  server.on("/off", handleOff);
  server.on("/passion", handlePassion);
  server.on("/reapers", handleReapers);
  server.on("/pick", handlePickMeUp);
  server.on("/intro", handleIntrospective);
  server.on("/lake", handleLake);
  server.on("/clouds", handleClouds);
  server.on("/uncanadian", handleUncanadian);
  server.on("/wildside", handleWildside);
  server.begin();

  Serial.begin(115200);
  Serial.print("this is a test");
}

void loop(void) {
  server.handleClient();
}
