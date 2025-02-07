  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <ArduinoOTA.h>
  #include <ESP8266WebServer.h>

const char* ssid     = "Prof.Rafael";
const char* password = "senhasenha";

ESP8266WebServer server(80);

const int output5 = LED_BUILTIN;//D1

//Você precisa colocar o ip da sua faixa
IPAddress local_IP(192,168,222,175); //Troque o IP
IPAddress gateway(192,168,222,90);
IPAddress subnet(255,255,255,0);

void handleRoot();
void handleOn();
void handleOff();

  void handleRoot() {
    String html = "<!DOCTYPE html><html>";
    html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    html += "<link rel=\"icon\" href=\"data:,\">";
    html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
    html += ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
    html += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
    html += ".button2 {background-color: #77878A;}</style></head>";
    html += "<body><h1>ESP8266 Web Server</h1>";
    html += "<h2>Prof. Rafael</h2>";
    html += "<button onclick=\"toggleLED('on')\" class=\"button\">ON</button>";
    html += "<button onclick=\"toggleLED('off')\" class=\"button button2\">OFF</button>";
    html += "<script>";
    html += "function toggleLED(state) {";
    html += "  var xhr = new XMLHttpRequest();";
    html += "  xhr.open('GET', '/5/' + state, true);";
    html += "  xhr.send();";
    html += "}";
    html += "</script>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  }

  void handleOff() {
    digitalWrite(output5, HIGH);
    server.send(200, "text/plain", "LED Ligado!");
  }

  void handleOn() {
    digitalWrite(output5, LOW);
    server.send(200, "text/plain", "LED Desligado!");
  }

  void setup(){
    Serial.begin(9600);
    pinMode(output5, OUTPUT);
    digitalWrite(output5, LOW);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    if (!WiFi.config(local_IP, gateway, subnet)) {
      Serial.println("STA Failed to configure");
    }
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  
    server.on("/", HTTP_GET, handleRoot);
    server.on("/5/off", HTTP_GET, handleOff);
    server.on("/5/on", HTTP_GET, handleOn);
    server.begin();  

    while (WiFi.waitForConnectResult() != WL_CONNECTED){
      Serial.println("Conexão falho! Rebooting..");
      WiFi.begin(ssid, password);;
      delay(5000);
    }  

    ArduinoOTA.onStart([] (){
      Serial.println("Start");
    });

    ArduinoOTA.onEnd([] (){
      Serial.println("\nEnd");
    });

    ArduinoOTA.onProgress([] (unsigned int progress, unsigned int total){
      Serial.printf("Progress: %u%% \r", (progress / (total / 100)));
    });

    ArduinoOTA.onError([] (ota_error_t error){
        Serial.printf("Error [%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        
      
    });
    ArduinoOTA.begin();
    Serial.println("Pronto");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

  }

  void loop(){
    ArduinoOTA.handle();
    server.handleClient();
  }