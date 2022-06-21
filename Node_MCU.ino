#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <ESP8266mDNS.h>
#include "Page.h"

const char* ap_ssid     = "UAV | MeH2.A2";
const char* ap_password = "aykeislief";
const char* DomainName = "UAV";

#define serialDebug false

uint8_t max_connections = 1;
int current_connections = 0, new_connections = 0;

uint64_t blink_t, blink_t_oud = 0, blink_dt;
uint64_t tijd_t, tijd_t_oud = 0, tijd_dt;

bool blink_out = false;

void wifi_disconnect_check(){
  new_connections = WiFi.softAPgetStationNum();
  if(new_connections > current_connections){
    current_connections = new_connections;
    if(serialDebug){
      Serial.print("a devices has connected, #connections: ");
      Serial.println(new_connections);
    }
    
    
  }
  if(new_connections < current_connections){
    current_connections = new_connections;
    if(serialDebug){
      Serial.print("a devices has disconnected, #connections: ");
      Serial.println(new_connections);
    }
    
     
  }
  if(current_connections >0){
    blink(0);
  } else { blink(500);}
}

AsyncWebServer server(80);



void setup()
{
  Serial.begin(115200);
  Serial.println(" ");

  if(WiFi.softAP(ap_ssid, ap_password,1,false,max_connections) == true){
    IPAddress IP = WiFi.softAPIP();
    if(serialDebug){
      Serial.print("[INFO] AP created with SSID");
      Serial.println(ap_ssid);
      Serial.print("[INFO] AP IP address: ");
      Serial.println(IP);
    }
    
  } else {
    if(serialDebug){Serial.println("[ERROR] Unable to create AP");}
  }

  if(!MDNS.begin(DomainName, WiFi.softAPIP())){
    if(serialDebug){Serial.println("[ERROR] mDNS responder did not setup");}
  } else {
    if(serialDebug){Serial.println("[INFO] mDNS setup successful!");Serial.println("[INFO] Controler accecible via UAV.local");}
  }

  MDNS.addService("http", "tcp", 80);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html",index_html, processor);
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(LED_BUILTIN, LOW);
    state |= (1<<7);
    Serial.write(state);
    request->send_P(200, "text/html",index_html, processor);
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(LED_BUILTIN, HIGH);
    state &= ~(1<<7);
    Serial.write(state);
    request->send_P(200, "text/html",index_html, processor);
    
  });

  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
    String inputMessage1;
    String inputParam1;
    String inputMessage2;
    String inputParam2;

    if(request->hasParam(PARAM_INPUT_1)&request->hasParam(PARAM_INPUT_2)){
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputParam1 = PARAM_INPUT_1;
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      inputParam2 = PARAM_INPUT_2;

      if(inputMessage2.toInt() == 1){
        state &= ~(0b00111111);
        state |= (1 << inputMessage1.toInt());
        
      } else {
        state &= ~(1<<inputMessage1.toInt());
        
      }
    }
    //if(serialDebug){Serial.print(inputMessage1+" | "+inputMessage2+" | ");Serial.println(state,BIN);}

    Serial.write(state);
    request->send_P(200,"text/html",index_html, processor);
  });

  server.on("/refresh", HTTP_GET, [](AsyncWebServerRequest *request){
    if(serialDebug){Serial.println("refresh");}
    request->send_P(200,"text/html",index_html,processor);
  });

  delay(2000); 
  server.begin();
  if(serialDebug){Serial.println("[INFO] HTTP server started");}
  
}

void loop() {
  MDNS.update();
  wifi_disconnect_check();
}


void blink(int speed){
  blink_t = millis();
  blink_dt = blink_t - blink_t_oud;
  if(blink_dt >= speed){
    switch (blink_out){
        case false:
            digitalWrite(LED_BUILTIN, HIGH);
            blink_out = true;
            break;
        
        case true:
            digitalWrite(LED_BUILTIN, LOW);
            blink_out = false;
            break;
    }

    blink_t_oud = blink_t;  
    }
}




