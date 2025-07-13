#pragma once
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class NetworkService {
public:
  static NetworkService& getInstance() {
    static NetworkService instance;
    return instance;
  }

  // Verificar si WiFi está conectado
  bool isConnected() {
    return WiFi.status() == WL_CONNECTED;
  }

  // Obtener la dirección IP
  String getIPAddress() {
    if (isConnected()) {
      return WiFi.localIP().toString();
    }
    return "No conectado";
  }

  // Obtener la intensidad de la señal WiFi
  int getSignalStrength() {
    if (isConnected()) {
      return WiFi.RSSI();
    }
    return -100;
  }

  // Hacer una petición GET
  String get(const String& url) {
    if (!isConnected()) {
      return "ERROR: No hay conexión WiFi";
    }

    HTTPClient http;
    http.begin(url);
    
    int httpCode = http.GET();
    String response = "";
    
    if (httpCode > 0) {
      response = http.getString();
    } else {
      response = "ERROR: " + String(httpCode);
    }
    
    http.end();
    return response;
  }

  // Hacer una petición POST con JSON
  String post(const String& url, const String& jsonData) {
    if (!isConnected()) {
      return "ERROR: No hay conexión WiFi";
    }

    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    int httpCode = http.POST(jsonData);
    String response = "";
    
    if (httpCode > 0) {
      response = http.getString();
    } else {
      response = "ERROR: " + String(httpCode);
    }
    
    http.end();
    return response;
  }

  // Obtener datos del clima (ejemplo)
  String getWeather(const String& city, const String& apiKey) {
    String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey + "&units=metric&lang=es";
    return get(url);
  }

  // Obtener la hora actual desde un servidor NTP
  String getCurrentTime() {
    if (!isConnected()) {
      return "No conectado";
    }

    // Configurar NTP
    configTime(0, 0, "pool.ntp.org");
    
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      return "Error al obtener hora";
    }
    
    char timeString[50];
    strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);
    return String(timeString);
  }

  // Obtener la fecha actual
  String getCurrentDate() {
    if (!isConnected()) {
      return "No conectado";
    }

    configTime(0, 0, "pool.ntp.org");
    
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      return "Error al obtener fecha";
    }
    
    char dateString[50];
    strftime(dateString, sizeof(dateString), "%d/%m/%Y", &timeinfo);
    return String(dateString);
  }

  // Enviar datos a un webhook (ejemplo para IFTTT)
  bool sendWebhook(const String& webhookUrl, const String& value1, const String& value2 = "", const String& value3 = "") {
    if (!isConnected()) {
      return false;
    }

    HTTPClient http;
    http.begin(webhookUrl);
    http.addHeader("Content-Type", "application/json");
    
    String jsonData = "{\"value1\":\"" + value1 + "\"";
    if (value2 != "") jsonData += ",\"value2\":\"" + value2 + "\"";
    if (value3 != "") jsonData += ",\"value3\":\"" + value3 + "\"";
    jsonData += "}";
    
    int httpCode = http.POST(jsonData);
    http.end();
    
    return httpCode == 200;
  }

private:
  NetworkService() {} // Constructor privado para singleton
  NetworkService(const NetworkService&) = delete;
  NetworkService& operator=(const NetworkService&) = delete;
}; 