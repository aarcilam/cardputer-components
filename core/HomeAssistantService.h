#pragma once
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "NetworkService.h"

class HomeAssistantService {
public:
  static HomeAssistantService& getInstance() {
    static HomeAssistantService instance;
    return instance;
  }

  void setBaseUrl(const String& url) {
    _baseUrl = url;
  }

  void setToken(const String& token) {
    _token = token;
  }

  // Obtener el estado de una entidad
  String getState(const String& entityId) {
    String url = _baseUrl + "/api/states/" + entityId;
    return authenticatedGet(url);
  }

  // Llamar a un servicio (por ejemplo, encender luz)
  String callService(const String& domain, const String& service, const String& jsonData) {
    String url = _baseUrl + "/api/services/" + domain + "/" + service;
    return authenticatedPost(url, jsonData);
  }

  // Obtener todos los estados
  String getAllStates() {
    String url = _baseUrl + "/api/states";
    return authenticatedGet(url);
  }

  // Hacer una petición GET autenticada
  String authenticatedGet(const String& url) {
    if (!NetworkService::getInstance().isConnected()) {
      return "ERROR: No hay conexión WiFi";
    }
    HTTPClient http;
    http.begin(url);
    if (_token != "") {
      http.addHeader("Authorization", "Bearer " + _token);
    }
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

  // Hacer una petición POST autenticada
  String authenticatedPost(const String& url, const String& jsonData) {
    if (!NetworkService::getInstance().isConnected()) {
      return "ERROR: No hay conexión WiFi";
    }
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    if (_token != "") {
      http.addHeader("Authorization", "Bearer " + _token);
    }
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

private:
  String _baseUrl = "http://homeassistant.local:8123";
  String _token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJkMDM3MGE4YjA5MDI0YWQ3OThlZGRhOTZkMGJkYmNjZSIsImlhdCI6MTc1MjU5MDM5NCwiZXhwIjoyMDY3OTUwMzk0fQ.S8FEjTrlRr_w6Fw8xe5JKWv7TlIEB-BQ461aUbs5Lqk";
  HomeAssistantService() {}
  HomeAssistantService(const HomeAssistantService&) = delete;
  HomeAssistantService& operator=(const HomeAssistantService&) = delete;
}; 