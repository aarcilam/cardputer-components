#pragma once
#include "../../core/RoutedView.h"
#include "../../components/ui/header.h"
#include "../../core/NetworkService.h"

class NetworkView : public RoutedView {
private:
  String _lastResponse = "";
  String _currentTime = "";
  String _currentDate = "";
  unsigned long _lastUpdate = 0;
  bool _showResponse = false;

public:
  void draw() override {
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
    
    Header header("Red");
    header.draw();
    
    // Obtener instancia del servicio de red
    NetworkService& network = NetworkService::getInstance();
    
    // Actualizar información constantemente
    _currentTime = network.getCurrentTime();
    _currentDate = network.getCurrentDate();
    
    int yPos = 35;
    
    // Estado de la conexión
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.setCursor(10, yPos);
    M5Cardputer.Display.print("Estado: ");
    M5Cardputer.Display.print(network.isConnected() ? "Conectado" : "Desconectado");
    
    yPos += 15;
    M5Cardputer.Display.setCursor(10, yPos);
    M5Cardputer.Display.print("IP: ");
    M5Cardputer.Display.print(network.getIPAddress());
    
    // Mostrar respuesta si existe
    if (_showResponse && _lastResponse != "") {
      yPos += 25;
      M5Cardputer.Display.setCursor(10, yPos);
      M5Cardputer.Display.print("Respuesta:");
      
      yPos += 15;
      M5Cardputer.Display.setCursor(10, yPos);
      // Mostrar solo los primeros 100 caracteres
      String shortResponse = _lastResponse.substring(0, 100);
      if (_lastResponse.length() > 100) {
        shortResponse += "...";
      }
      M5Cardputer.Display.print(shortResponse);
    }
    
    yPos += 25;
    
    // Botones de ejemplo
    M5Cardputer.Display.setCursor(10, yPos);
    M5Cardputer.Display.print("1. Test HTTP GET");
    
    yPos += 15;
    M5Cardputer.Display.setCursor(10, yPos);
    M5Cardputer.Display.print("2. Obtener Clima");
    
    yPos += 15;
    M5Cardputer.Display.setCursor(10, yPos);
    M5Cardputer.Display.print("3. Enviar Webhook");
    
    
    // Instrucciones
    M5Cardputer.Display.setCursor(10, 140);
    M5Cardputer.Display.print("1-3: Acciones | Enter: Volver");
  }
  
  void handleInput(char key) override {
    NetworkService& network = NetworkService::getInstance();
    
    switch (key) {
      case '1':
        // Test HTTP GET
        _lastResponse = network.get("http://httpbin.org/get");
        _showResponse = true;
        markForRedraw();
        break;
        
      case '2':
        // Obtener clima (necesitas una API key)
        _lastResponse = network.getWeather("Madrid", "TU_API_KEY_AQUI");
        _showResponse = true;
        markForRedraw();
        break;
        
      case '3':
        // Enviar webhook (ejemplo para IFTTT)
        if (network.sendWebhook("TU_WEBHOOK_URL_AQUI", "Mensaje desde Cardputer", "Test", "OK")) {
          _lastResponse = "Webhook enviado exitosamente";
        } else {
          _lastResponse = "Error al enviar webhook";
        }
        _showResponse = true;
        markForRedraw();
        break;
        
      case '\n':
      case '\r':
      case 'Enter':
      case 'OK':
      case '/':
        goBack();
        break;
    }
  }
}; 