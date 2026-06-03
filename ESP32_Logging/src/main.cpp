#define ENABLE_USER_AUTH
#define ENABLE_DATABASE

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>
#include <Wire.h>
#include <DHT.h>
#include "time.h"

// ! INSERISCI QUA LE TUE CRDENZIALI !

#define WIFI_SSID "la_tua_rete"
#define WIFI_PASSWORD "la_tua_password"

#define Web_API_KEY "..."
#define DATABASE_URL "..."

#define USER_EMAIL "..."
#define USER_PASS "..."

//

#define DHTPIN 21       
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void processData(AsyncResult &aResult);

UserAuth user_auth(Web_API_KEY, USER_EMAIL, USER_PASS);

FirebaseApp app;
WiFiClientSecure ssl_client;
using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client);
RealtimeDatabase Database;

// Timer
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 10000;

String uid;

String databasePath;
String tempPath = "/temperature";
String humPath = "/humidity";
String timePath = "/timestamp";

String parentPath;
unsigned long timestamp;

const char* ntpServer = "pool.ntp.org";

// Variabili sensore
float temperature;
float humidity;

// JSON
object_t jsonData, obj1, obj2, obj3;
JsonWriter writer;

// WiFi init
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
}

// Time function
unsigned long getTime() {
  time_t now;
  time(&now);
  return now;
}

void setup(){
  Serial.begin(115200);

  dht.begin();
  initWiFi();
  configTime(0, 0, ntpServer);

  ssl_client.setInsecure();
  ssl_client.setHandshakeTimeout(5);

  initializeApp(aClient, app, getAuth(user_auth), processData, "🔐 authTask");
  app.getApp<RealtimeDatabase>(Database);
  Database.url(DATABASE_URL);
}

void loop(){
  app.loop();

  if (app.ready()){

    unsigned long currentTime = millis();
    if (currentTime - lastSendTime >= sendInterval){

      lastSendTime = currentTime;

      uid = app.getUid().c_str();
      databasePath = "/UsersData/" + uid + "/readings";

      timestamp = getTime();
      Serial.print("time: ");
      Serial.println(timestamp);

      parentPath = databasePath + "/" + timestamp;

      temperature = dht.readTemperature();
      humidity = dht.readHumidity();

      if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Errore lettura DHT11!");
        return;
      }


      writer.create(obj1, tempPath, temperature);
      writer.create(obj2, humPath, humidity);
      writer.create(obj3, timePath, (int)timestamp);
      writer.join(jsonData, 3, obj1, obj2, obj3);

      Database.set<object_t>(aClient, parentPath, jsonData, processData, "RTDB_Send_Data");
    }
  }
}

void processData(AsyncResult &aResult){
  if (!aResult.isResult())
    return;

  if (aResult.isEvent())
    Firebase.printf("Event task: %s, msg: %s, code: %d\n",
      aResult.uid().c_str(),
      aResult.eventLog().message().c_str(),
      aResult.eventLog().code());

  if (aResult.isDebug())
    Firebase.printf("Debug task: %s, msg: %s\n",
      aResult.uid().c_str(),
      aResult.debug().c_str());

  if (aResult.isError())
    Firebase.printf("Error task: %s, msg: %s, code: %d\n",
      aResult.uid().c_str(),
      aResult.error().message().c_str(),
      aResult.error().code());

  if (aResult.available())
    Firebase.printf("task: %s, payload: %s\n",
      aResult.uid().c_str(),
      aResult.c_str());
}