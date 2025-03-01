
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "LittleFS.h"
#include "wifi_network_config.h"


#include <string>
#include <iostream>

#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) \
    do { \
        Serial.printf("DEBUG: %s:%d:%s(): " fmt, \
                __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
    } while (0)
#else
#define DEBUG_PRINT(fmt, ...) \
    do {} while (0)
#endif

// Create AsyncWebServer object on port 80#include "tft_printf.h"
AsyncWebServer wifi_config_server(80);

// Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";


//Variables to save values from HTML form
String ssid;
String pass;


// File paths to save input values permanently
const char* ssidPath = "/ssid.txt";
const char* passPath = "/pass.txt";


// Timer variables
unsigned long previousMillis = 0;
const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)


// Initialize LittleFS
void initLittleFS() {
  if (!LittleFS.begin(false)) {
    DEBUG_PRINT("An error has occurred while mounting LittleFS\n");
    while(true){};
  }
  DEBUG_PRINT("LittleFS mounted successfully\n");
}


// Read File from LittleFS
String readFile(fs::FS &fs, const char * path){
  DEBUG_PRINT("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if(!file || file.isDirectory()){
    DEBUG_PRINT("- failed to open file for reading\n");
    return String();
  }
  
  String fileContent;
  while(file.available()){
    fileContent = file.readStringUntil('\n');
    break;     
  }
  return fileContent;
}

// Write file to LittleFS
void writeFile(fs::FS &fs, const char * path, const char * message){
  DEBUG_PRINT("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    DEBUG_PRINT("- failed to open file for writing\n");
    return;
  }
  if(file.print(message)){
    DEBUG_PRINT("- file written\n");
  } else {
    DEBUG_PRINT("- write failed\n");
  }
}

// Initialize WiFi
bool testWifi() {
  if(ssid=="" || pass==""){
    DEBUG_PRINT("Undefined SSID, Password, microROS server IP address or Port\n");
    return false;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());
  DEBUG_PRINT("Connecting to WiFi...\n");


  unsigned long currentMillis = millis();
  previousMillis = currentMillis;

  while(WiFi.status() != WL_CONNECTED) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      DEBUG_PRINT("Failed to connect\n");
      return false;
    }
  }

  return true;
}


NETWORK_CONFIG network_config;

bool configureNetwork(bool forceConfigure, NETWORK_CONFIG *networkConfig) {


  initLittleFS();

  // detect webserver files

  File file = LittleFS.open("/wifimanager.html");
  if(!file){
    DEBUG_PRINT("No webpages file found\n");

    while(true){};
  }
 
  // Load values saved in LittleFS
  ssid = readFile(LittleFS, ssidPath);
  pass = readFile(LittleFS, passPath);

  
  DEBUG_PRINT("ssid : %s\n", ssid.c_str());
  DEBUG_PRINT("pass : %s\n", pass.c_str());

  if(testWifi() & !forceConfigure) {
    networkConfig->password = pass;
    networkConfig->ssid = ssid;
    return true;
  }
  else {
    // Connect to Wi-Fi network with SSID and password
    WiFi.mode(WIFI_AP);
    const char *ap_name = "wifi";
    DEBUG_PRINT("Setting AP (Access Point)\n");

    // NULL sets an open Access Point
    //WiFi.softAP(ap_name, NULL);
    WiFi.softAP("gerard", NULL);

    IPAddress IP = WiFi.softAPIP();

    DEBUG_PRINT("Connect to AP: %s, IP: %s\n", ap_name, IP.toString().c_str());
    
    // Web Server Root URL
    wifi_config_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(LittleFS, "/wifimanager.html", "text/html");
    });
    
    wifi_config_server.serveStatic("/", LittleFS, "/");
    
    wifi_config_server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
      int params = request->params();
      for(int i=0;i<params;i++){
        const AsyncWebParameter* p = request->getParam(i);
        if(p->isPost()){
          // HTTP POST ssid value
          if (p->name() == PARAM_INPUT_1) {
            ssid = p->value().c_str();
            DEBUG_PRINT("SSID set to: %s\n", ssid );
            // Write file to save value
            writeFile(LittleFS, ssidPath, ssid.c_str());
          }
          // HTTP POST pass value
          if (p->name() == PARAM_INPUT_2) {
            pass = p->value().c_str();
            DEBUG_PRINT("Password set to: %s\n", pass);
            // Write file to save value
            writeFile(LittleFS, passPath, pass.c_str());
          }
        }
      }
      request->send(200, "text/plain", "Done. Controller will restart");
      delay(3000);
      ESP.restart();
    });
    wifi_config_server.begin();
  }
  return false;
}

