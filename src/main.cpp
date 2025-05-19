/**
 * @file    main.c
 * @brief   Development project with ESP32C3.
 *
 * @author  Oqueber Navarro
 * @date    15-11/2024
 *
 * @details This project involves the development of an application for the ESP32C3 microcontroller 
 * using the Arduino programming language. It is compiled and designed to be executed 
 * in PlatformIO with Visual Studio Code.
 * 
 * For more information, visit: 
 * @see     https://oqueber.es/bmo
 */

// Standart lib
#include "Arduino.h"
#include <ESPAsyncWebServer.h>
#include "FS.h"
#include <SD.h>
#include <SPI.h>
#include <ArduinoJson.h>
//DNS
#include <ESPmDNS.h>
// OTA
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "main.h"

const char* ssid = "coshitos_house";
const char* password = "familiaF3liz.";

AsyncWebServer server(80);
String path = "/mensajes.json";

//
// WEB SERVER
//

void guardarMensajes(JsonArray array) {
  File file = SD.open(path, "w");
  if (file) {
    serializeJson(array, file);
    file.close();
  }
}

void crearArchivoInicial() {
  if (!SD.exists(path)) {
    File file = SD.open(path, "w");
    if (file) {
      file.println("[]"); // Crea el archivo con un arreglo vacío
      file.close();
    } else {
      Serial.println("Error al crear el archivo mensajes.json");
    }
  }
}

DynamicJsonDocument cargarMensajes() {
  DynamicJsonDocument doc(2048);
  if (SD.exists(path)) {
    File file = SD.open(path, "r");
    DeserializationError err = deserializeJson(doc, file);
    file.close();
    if (err) {
      Serial.println("Error de JSON, usando array vacío");
      doc.to<JsonArray>();
    }
  } else {
    doc.to<JsonArray>();
  }
  return doc;
}


//
// SETUP
//

void setup()
{
  Serial.begin(115200);

  uint8_t num_wifi_errors = 0;

  WiFi.begin(ssid, password);
  while ((WiFi.status() != WL_CONNECTED) && (num_wifi_errors < 5))
  {
    delay(500);
    num_wifi_errors++;
  }

  Serial.println("Conectado: " + WiFi.localIP().toString());

  // Inicializar SPI con pines personalizados
  if (!SD.begin()) {
    Serial.println("Fallo al iniciar SD con pines personalizados");
  }

  crearArchivoInicial();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SD, "/index.html", "text/html");
  });

  server.on("/mensajes", HTTP_GET, [](AsyncWebServerRequest* request) {
    DynamicJsonDocument doc = cargarMensajes();
    String json;
    serializeJson(doc, json);
    request->send(200, "application/json", json);
  });

  server.on("/mensajes", HTTP_POST, [](AsyncWebServerRequest* request) {}, NULL,
    [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
    DynamicJsonDocument nuevo(512);
    DeserializationError error = deserializeJson(nuevo, data);
    
    DynamicJsonDocument doc = cargarMensajes();
    JsonArray array = doc.as<JsonArray>();
    
    // Añadir nuevo mensaje
    array.add(nuevo);

    guardarMensajes(array);
    request->send(200, "application/json", "{\"resultado\":\"ok\"}");
  });

  server.on("/mensajes", HTTP_DELETE, [](AsyncWebServerRequest* request) {
    if (request->hasParam("id")) {
      int id = request->getParam("id")->value().toInt();
      DynamicJsonDocument doc = cargarMensajes();
      JsonArray array = doc.as<JsonArray>();
      if (id >= 0 && id < array.size()) {
        array.remove(id);
        guardarMensajes(array);
      }
      request->send(200, "application/json", "OK");
    } else {
      request->send(400, "application/json", "{\"error\": \"Falta el parámetro id\"}");
    }
  });

  // Servir archivos estáticos (si tienes archivos como HTML, JS, CSS)
  server.serveStatic("/", SD, "/");

  // Iniciar el servidor
  server.begin();
  
  if (!MDNS.begin("asistente")) {  // reemplaza "asistente" por el nombre que quieras
    Serial.println("Error iniciando mDNS");
  }

  Serial.println("start freertos");

  xTaskCreate(&init_inputs_task,"inputs",(1024*3), NULL,1,NULL );
  xTaskCreate(&init_display_task,"display",(1024*3), NULL,1,NULL );
  xTaskCreate(&init_tone_task,"tone",(1024*3), NULL,1,NULL );
  xTaskCreate(&init_light_task,"light",(1024*2),NULL,1,NULL);
  xTaskCreate(&init_servo_task,"light",(1024*2),NULL,1,NULL);
  xTaskCreate(&init_central_task,"centrl",(1024*2),NULL,1,NULL);

  
}


void loop(){taskYIELD ();}