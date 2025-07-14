#pragma once
#include "../../core/RoutedView.h"
#include "../../components/ui/header.h"
#include "../../core/SDCardService.h"

class SDCardView : public RoutedView {
private:
  int _selectedOption = 0;
  const int _maxOptions = 6;
  String _lastResult = "";
  bool _showResult = false;

public:
  void draw() override {
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
    
    Header header("SD Card");
    header.draw();
    
    SDCardService& sd = SDCardService::getInstance();
    
    int yPos = 35;
    
    // Estado de la SD
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.setCursor(10, yPos);
    M5Cardputer.Display.print("Estado: ");
    
    if (sd.isInitialized()) {
      M5Cardputer.Display.setTextColor(0x00FF00);
      M5Cardputer.Display.print("OK");
      M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
      
      yPos += 15;
      M5Cardputer.Display.setCursor(10, yPos);
      M5Cardputer.Display.print("Info: ");
      String info = sd.getCardInfo();
      if (info.length() > 30) {
        info = info.substring(0, 30) + "...";
      }
      M5Cardputer.Display.print(info);
    } else {
      M5Cardputer.Display.setTextColor(0xFF0000);
      M5Cardputer.Display.print("ERROR");
      M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    }
    
    yPos += 25;
    
    // Opciones del menú
    drawOption("1. Listar archivos", 0, yPos);
    yPos += 15;
    drawOption("2. Crear wificon.txt", 1, yPos);
    yPos += 15;
    drawOption("3. Ver wificon.txt", 2, yPos);
    yPos += 15;
    drawOption("4. Test velocidad", 3, yPos);
    yPos += 15;
    drawOption("5. Crear archivo test", 4, yPos);
    yPos += 15;
    drawOption("6. Info detallada", 5, yPos);
    
    // Mostrar resultado si existe
    if (_showResult && _lastResult != "") {
      yPos += 25;
      M5Cardputer.Display.setCursor(10, yPos);
      M5Cardputer.Display.print("Resultado:");
      
      yPos += 15;
      M5Cardputer.Display.setCursor(10, yPos);
      // Mostrar solo los primeros 100 caracteres
      String shortResult = _lastResult.substring(0, 100);
      if (_lastResult.length() > 100) {
        shortResult += "...";
      }
      M5Cardputer.Display.print(shortResult);
    }
    
    // Instrucciones
    M5Cardputer.Display.setCursor(10, 140);
    M5Cardputer.Display.print("1-6: Acciones | Enter: Seleccionar | /: Volver");
  }
  
  void handleInput(char key) override {
    switch (key) {
      case ';':
        _selectedOption = (_selectedOption - 1 + _maxOptions) % _maxOptions;
        markForRedraw();
        break;
        
      case '.':
        _selectedOption = (_selectedOption + 1) % _maxOptions;
        markForRedraw();
        break;
        
      case '\n':
      case '\r':
      case 'Enter':
      case '/':
        executeSelectedOption();
        break;
        
      case ',':
        goBack();
        break;
    }
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
    SDCardService& sd = SDCardService::getInstance();
    
    switch (_selectedOption) {
      case 0: // Listar archivos
        _lastResult = sd.listDir("/", 0);
        _showResult = true;
        break;
        
      case 1: // Crear wificon.txt
        createWifiConfig();
        break;
        
      case 2: // Ver wificon.txt
        if (sd.exists("/wificon.txt")) {
          _lastResult = "Contenido de wificon.txt:\n" + sd.readFile("/wificon.txt");
        } else {
          _lastResult = "Archivo wificon.txt no existe";
        }
        _showResult = true;
        break;
        
      case 3: // Test velocidad
        _lastResult = sd.testFileIO("/test_speed.txt");
        _showResult = true;
        break;
        
      case 4: // Crear archivo test
        if (sd.writeFile("/test.txt", "Archivo de prueba creado desde Cardputer\nFecha: " + String(millis()))) {
          _lastResult = "Archivo test.txt creado exitosamente";
        } else {
          _lastResult = "Error al crear archivo test.txt";
        }
        _showResult = true;
        break;
        
      case 5: // Info detallada
        _lastResult = sd.getCardInfo();
        _showResult = true;
        break;
    }
  }
  
  void createWifiConfig() {
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
    M5Cardputer.Display.setCursor(10, 40);
    M5Cardputer.Display.print("Creando wificon.txt...");
    
    SDCardService& sd = SDCardService::getInstance();
    
    // Crear archivo de ejemplo
    String ssid = "MiWiFi";
    String password = "MiPassword123";
    
    if (sd.writeConfigFile("/wificon.txt", ssid, password)) {
      M5Cardputer.Display.setCursor(10, 60);
      M5Cardputer.Display.print("Archivo creado exitosamente!");
      M5Cardputer.Display.setCursor(10, 80);
      M5Cardputer.Display.print("SSID: MiWiFi");
      M5Cardputer.Display.setCursor(10, 100);
      M5Cardputer.Display.print("Password: MiPassword123");
      M5Cardputer.Display.setCursor(10, 120);
      M5Cardputer.Display.print("Edita el archivo en tu PC");
    } else {
      M5Cardputer.Display.setCursor(10, 60);
      M5Cardputer.Display.print("Error al crear archivo");
    }
    
    M5Cardputer.Display.setCursor(10, 140);
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