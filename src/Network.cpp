#include "Network.h"
#include <WiFi.h>
#define WIFI_SSID "NETGEAR-Helton"
#define WIFI_PASSWORD "Uzf.Rmr2FKjyc"

#define API_KEY "AIzaSyD2Tsr9z9q43LsRi6SE2ZGg_zNjBjw6hFw"
#define FIREBASE_PROJECT_ID "first-project-b6aea"
#define USER_EMAIL "g@g.com"
#define USER_PASSWORD "test123"

static Network *instance = NULL;

Network::Network(){
  instance = this;
}

void WiFiEventConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WIFI CONNECTED! BUT WAIT FOR THE LOCAL IP ADDR");
}

void WiFiEventGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("LOCAL IP ADDRESS:");
  Serial.println(WiFi.localIP());
  instance->firebaseInit();
}

void WiFiEventDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WIFI DISCONNECTED!");
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
}

void Network::initWiFi(){
    WiFi.disconnect();
    WiFi.onEvent(WiFiEventConnected, SYSTEM_EVENT_STA_CONNECTED);
    WiFi.onEvent(WiFiEventGotIP, SYSTEM_EVENT_STA_GOT_IP);
    WiFi.onEvent(WiFiEventDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);

    WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
 }

 void Network::firebaseInit(){
  config.api_key = API_KEY;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&config, &auth);
 }

 void Network::firestoreDataUpdate(double temp, double humi){
  if(WiFi.status() == WL_CONNECTED && Firebase.ready()){
    String documentPath = "House/Room_1";

    FirebaseJson content;

    content.set("fields/temperature/doubleValue", String(temp).c_str());
    content.set("fields/humidity/doubleValue", String(humi).c_str());

    if(Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(),content.raw())){
      Serial.printf("ok\n%s\n\n",fbdo.payload().c_str());
      return;
    }else{
      Serial.println(fbdo.errorReason());
    }
  }
 }