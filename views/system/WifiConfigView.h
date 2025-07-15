#pragma once
#include "../../core/RoutedView.h"
#include "../../components/ui/header.h"
#include "../../core/SDCardService.h"
#include "../../core/NetworkService.h"

class WifiConfigView : public RoutedView {
private:
  bool _showConfig = false;
  String _ssid = "";
  String _password = "";
  int _selectedOption = 0;
  const int _maxOptions = 4;

public:
  void draw() override {
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
    
    Header header("Config WiFi");
    header.draw();
    
    int yPos = 35;      
    
    // Estado actual
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.setCursor(10, yPos);
    M5Cardputer.Display.print("Estado WiFi: ");
    
    if (WiFi.status() == WL_CONNECTED) {
      M5Cardputer.Display.setTextColor(0x00FF00);
      M5Cardputer.Display.print("Conectado");
      M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
      
      yPos += 15;
      M5Cardputer.Display.setCursor(10, yPos);
      M5Cardputer.Display.print("SSID: ");
      M5Cardputer.Display.print(WiFi.SSID());
      
      yPos += 15;
      M5Cardputer.Display.setCursor(10, yPos);
      M5Cardputer.Display.print("IP: ");
      M5Cardputer.Display.print(WiFi.localIP().toString());
      
      yPos += 15;
      M5Cardputer.Display.setCursor(10, yPos);
      M5Cardputer.Display.print("MAC: ");
      M5Cardputer.Display.print(WiFi.macAddress());
      
      yPos += 15;
      M5Cardputer.Display.setCursor(10, yPos);
      M5Cardputer.Display.print("Señal: ");
      M5Cardputer.Display.print(WiFi.RSSI());
      M5Cardputer.Display.print(" dBm");
      
      yPos += 15;
      M5Cardputer.Display.setCursor(10, yPos);
      M5Cardputer.Display.print("Canal: ");
      M5Cardputer.Display.print(WiFi.channel());
    } else {
      M5Cardputer.Display.setTextColor(0xFF0000);
      M5Cardputer.Display.print("Desconectado");
      M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
      
      yPos += 15;
      M5Cardputer.Display.setCursor(10, yPos);
      M5Cardputer.Display.print("Ultima config usada:");
      
      yPos += 15;
      M5Cardputer.Display.setCursor(10, yPos);
      M5Cardputer.Display.print("SSID: ");
      M5Cardputer.Display.print(_ssid);
      
      yPos += 15;
      M5Cardputer.Display.setCursor(10, yPos);
      M5Cardputer.Display.print("Password: ");
      for (int i = 0; i < _password.length(); i++) {
        M5Cardputer.Display.print("*");
      }
    }
    
    yPos += 25;
    
    // Opciones del menú
    drawOption("1. Reintentar conexion", 0, yPos);
    yPos += 15;
    drawOption("2. Ver configuracion", 1, yPos);
    yPos += 15;
    drawOption("3. Crear configuracion", 2, yPos);
    yPos += 15;
    drawOption("4. Test conexion", 3, yPos);
    
    // Mostrar configuración si está seleccionada
    if (_showConfig) {
      yPos += 25;
      M5Cardputer.Display.setCursor(10, yPos);
      M5Cardputer.Display.print("SSID: ");
      M5Cardputer.Display.print(_ssid);
      
      yPos += 15;
      M5Cardputer.Display.setCursor(10, yPos);
      M5Cardputer.Display.print("Password: ");
      for (int i = 0; i < _password.length(); i++) {
        M5Cardputer.Display.print("*");
      }
    }
    
    // Instrucciones
    M5Cardputer.Display.setCursor(10, 140);
    M5Cardputer.Display.print(";/. Navegar | Enter: Seleccionar | Del: Volver");
  }
  
  void handleInput(char key) override {
    // Manejar teclas específicas si es necesario
  }
  
  void onSelect() override {
    executeSelectedOption();
  }
  
  void onNavigateNext() override {
    _selectedOption = (_selectedOption + 1) % _maxOptions;
    markForRedraw();
  }
  
  void onNavigatePrev() override {
    _selectedOption = (_selectedOption - 1 + _maxOptions) % _maxOptions;
    markForRedraw();
  }
  
  void onGoBack() override {
    goBack();
  }
  
  void refreshHeader() override {
    // Refrescar solo el header sin afectar el contenido
    Header header("Config WiFi");
    header.refreshHeader();
  }

private:
  void drawOption(const String& text, int option, int y) {
    if (option == _selectedOption) {
      M5Cardputer.Display.setTextColor(Theme::ACCENT_COLOR);
      M5Cardputer.Display.setCursor(8, y);
      M5Cardputer.Display.print("> ");
    } else {
      M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
      M5Cardputer.Display.setCursor(10, y);
    }
    M5Cardputer.Display.print(text);
  }
  
  void executeSelectedOption() {
    switch (_selectedOption) {
      case 0: // Reintentar conexión
        retryWifiConnection();
        break;
        
      case 1: // Ver configuración
        loadWifiConfig();
        _showConfig = !_showConfig;
        break;
        
      case 2: // Crear configuración
        createWifiConfig();
        break;
        
      case 3: // Test conexión
        testWifiConnection();
        break;
    }
  }
  
  void retryWifiConnection() {
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
    M5Cardputer.Display.setCursor(10, 60);
    M5Cardputer.Display.print("Reintentando conexion...");
    
    NetworkService& network = NetworkService::getInstance();
    
    if (network.reconnectWiFi()) {
      M5Cardputer.Display.setCursor(10, 80);
      M5Cardputer.Display.print("¡Conectado exitosamente!");
      M5Cardputer.Display.setCursor(10, 100);
      M5Cardputer.Display.print("IP: ");
      M5Cardputer.Display.print(network.getIPAddress());
      
      // Refrescar el header para mostrar el nuevo estado de WiFi
      refreshHeader();
    } else {
      M5Cardputer.Display.setCursor(10, 80);
      M5Cardputer.Display.print("Error de conexion");
      M5Cardputer.Display.setCursor(10, 100);
      M5Cardputer.Display.print("Verifica configuracion");
    }
    
    delay(2000);
  }
  
  bool loadWifiConfig() {
    SDCardService& sd = SDCardService::getInstance();
    
    if (!sd.isInitialized()) {
      return false;
    }
    
    if (sd.exists("/wificon.txt")) {
      return sd.readFileLines("/wificon.txt", _ssid, _password);
    }
    return false;
  }
  
  void createWifiConfig() {
    // Por ahora solo muestra instrucciones
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
    M5Cardputer.Display.setCursor(10, 40);
    M5Cardputer.Display.print("Crear archivo wificon.txt");
    M5Cardputer.Display.setCursor(10, 60);
    M5Cardputer.Display.print("en la SD con formato:");
    M5Cardputer.Display.setCursor(10, 80);
    M5Cardputer.Display.print("TU_SSID");
    M5Cardputer.Display.setCursor(10, 100);
    M5Cardputer.Display.print("TU_PASSWORD");
    M5Cardputer.Display.setCursor(10, 120);
    M5Cardputer.Display.print("Presiona Enter para volver");
    
    // Esperar input
    while (true) {
      M5Cardputer.update();
      if (M5Cardputer.Keyboard.isChange()) {
        M5Cardputer.Keyboard.updateKeysState();
        auto& word = M5Cardputer.Keyboard.keysState().word;
        for (char key : word) {
          if (key == '\n' || key == '\r' || key == 'Enter' || key == 'OK') {
            return;
          }
        }
      }
      delay(100);
    }
  }
  
  void testWifiConnection() {
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
    M5Cardputer.Display.setCursor(10, 60);
    M5Cardputer.Display.print("Probando conexion...");
    
    if (WiFi.status() == WL_CONNECTED) {
      // Intentar hacer una petición HTTP simple
      HTTPClient http;
      http.begin("http://httpbin.org/get");
      
      int httpCode = http.GET();
      
      M5Cardputer.Display.setCursor(10, 80);
      if (httpCode > 0) {
        M5Cardputer.Display.print("Conexion OK - HTTP: ");
        M5Cardputer.Display.print(httpCode);
      } else {
        M5Cardputer.Display.print("Error HTTP: ");
        M5Cardputer.Display.print(httpCode);
      }
      
      http.end();
    } else {
      M5Cardputer.Display.setCursor(10, 80);
      M5Cardputer.Display.print("No hay conexion WiFi");
    }
    
    M5Cardputer.Display.setCursor(10, 120);
    M5Cardputer.Display.print("Presiona Enter para volver");
    
    // Esperar input
    while (true) {
      M5Cardputer.update();
      if (M5Cardputer.Keyboard.isChange()) {
        M5Cardputer.Keyboard.updateKeysState();
        auto& word = M5Cardputer.Keyboard.keysState().word;
        for (char key : word) {
          if (key == '\n' || key == '\r' || key == 'Enter' || key == 'OK') {
            return;
          }
        }
      }
      delay(100);
    }
  }
}; 