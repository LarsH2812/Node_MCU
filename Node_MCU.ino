#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include "Page.h"

const char* ap_ssid     = "vliegbeestAP";
const char* ap_password = "aykeislief";

uint8_t max_connections = 4;
int current_connections = 0, new_connections = 0;



uint64_t blink_t, blink_t_oud = 0, blink_dt;
uint64_t tijd_t, tijd_t_oud = 0, tijd_dt;

bool blink_out = false;

void wifi_disconnect_check(){
  new_connections = WiFi.softAPgetStationNum();
  if(new_connections > current_connections){
    current_connections = new_connections;
    Serial.print("a devices has connected, #connections: ");
    Serial.println(new_connections);
    
  }
  if(new_connections < current_connections){
    current_connections = new_connections;
    Serial.print("a devices has disconnected, #connections: ");
    Serial.println(new_connections);
     
  }
  // if(current_connections >0){
  //   blink(0);
  // } else { blink(500);}
}

AsyncWebServer server(80);



void setup()
{
  Serial.begin(115200);
  Serial.println();

  if(WiFi.softAP(ap_ssid, ap_password,1,false,max_connections) == true){
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP created with SSID");
    Serial.println(ap_ssid);
    Serial.print("AP IP address: ");
    Serial.println(IP);
  } else {
    Serial.println("Unable to create AP");
  }
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html",index_html, processor);
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(LED_BUILTIN, LOW);
    state |= (1<<8);
    request->send_P(200, "text/html",index_html, processor);
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(LED_BUILTIN, HIGH);
    state &= ~(1<<8);
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
    Serial.print(inputMessage1+" | "+inputMessage2+" | ");Serial.println(state,BIN);
    request->send_P(200,"text/html",index_html, processor);
  });

  server.on("/refresh", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200,"text/html",index_html,processor);
  });

  delay(2000); 
  server.begin();
  
}

void loop() {
   wifi_disconnect_check();

  // tijd_t = millis();
  // tijd_dt = tijd_t - tijd_t_oud;
  // if(tijd_dt >= 200){
  //   Serial.println(state,BIN);
  //   tijd_t_oud = tijd_t;
  // }
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