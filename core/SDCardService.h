#pragma once
#include <M5Cardputer.h>
#include <SPI.h>
#include <SD.h>

#define SD_SPI_SCK_PIN  40
#define SD_SPI_MISO_PIN 39
#define SD_SPI_MOSI_PIN 14
#define SD_SPI_CS_PIN   12

class SDCardService {
public:
  static SDCardService& getInstance() {
    static SDCardService instance;
    return instance;
  }

  // Inicializar SD
  bool begin() {
    SPI.begin(SD_SPI_SCK_PIN, SD_SPI_MISO_PIN, SD_SPI_MOSI_PIN, SD_SPI_CS_PIN);
    
    if (!SD.begin(SD_SPI_CS_PIN, SPI, 25000000)) {
      _isInitialized = false;
      return false;
    }

    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
      _isInitialized = false;
      return false;
    }

    _isInitialized = true;
    _cardType = cardType;
    _cardSize = SD.cardSize() / (1024 * 1024); // MB
    return true;
  }

  // Verificar si está inicializada
  bool isInitialized() const {
    return _isInitialized;
  }

  // Obtener información de la tarjeta
  String getCardInfo() {
    if (!_isInitialized) return "SD no inicializada";
    
    String info = "Tipo: ";
    switch (_cardType) {
      case CARD_MMC: info += "MMC"; break;
      case CARD_SD: info += "SDSC"; break;
      case CARD_SDHC: info += "SDHC"; break;
      default: info += "UNKNOWN"; break;
    }
    
    info += " | Tamaño: " + String(_cardSize) + "MB";
    info += " | Libre: " + String(SD.totalBytes() / (1024 * 1024)) + "MB";
    info += " | Usado: " + String(SD.usedBytes() / (1024 * 1024)) + "MB";
    
    return info;
  }

  // Leer archivo completo
  String readFile(const String& path) {
    if (!_isInitialized) return "";
    
    File file = SD.open(path);
    if (!file) {
      return "";
    }
    
    String content = "";
    while (file.available()) {
      content += (char)file.read();
    }
    file.close();
    
    return content;
  }

  // Escribir archivo
  bool writeFile(const String& path, const String& content) {
    if (!_isInitialized) return false;
    
    File file = SD.open(path, FILE_WRITE);
    if (!file) {
      return false;
    }
    
    bool success = file.print(content);
    file.close();
    return success;
  }

  // Agregar contenido a archivo
  bool appendFile(const String& path, const String& content) {
    if (!_isInitialized) return false;
    
    File file = SD.open(path, FILE_APPEND);
    if (!file) {
      return false;
    }
    
    bool success = file.print(content);
    file.close();
    return success;
  }

  // Verificar si archivo existe
  bool exists(const String& path) {
    if (!_isInitialized) return false;
    return SD.exists(path);
  }

  // Eliminar archivo
  bool deleteFile(const String& path) {
    if (!_isInitialized) return false;
    return SD.remove(path);
  }

  // Renombrar archivo
  bool renameFile(const String& path1, const String& path2) {
    if (!_isInitialized) return false;
    return SD.rename(path1, path2);
  }

  // Crear directorio
  bool createDir(const String& path) {
    if (!_isInitialized) return false;
    return SD.mkdir(path);
  }

  // Eliminar directorio
  bool removeDir(const String& path) {
    if (!_isInitialized) return false;
    return SD.rmdir(path);
  }

  // Listar archivos en directorio
  String listDir(const String& dirname, uint8_t levels = 0) {
    if (!_isInitialized) return "SD no inicializada";
    
    String result = "Directorio: " + dirname + "\n";
    
    File root = SD.open(dirname);
    if (!root) {
      return "Error al abrir directorio";
    }
    if (!root.isDirectory()) {
      return "No es un directorio";
    }

    File file = root.openNextFile();
    while (file) {
      if (file.isDirectory()) {
        result += "  DIR : " + String(file.name()) + "\n";
        if (levels) {
          result += listDir(file.path(), levels - 1);
        }
      } else {
        result += "  FILE: " + String(file.name()) + " (" + String(file.size()) + " bytes)\n";
      }
      file = root.openNextFile();
    }
    
    return result;
  }

  // Leer archivo línea por línea
  bool readFileLines(const String& path, String& line1, String& line2) {
    if (!_isInitialized) return false;
    
    File file = SD.open(path);
    if (!file) {
      return false;
    }
    
    line1 = file.readStringUntil('\n');
    line2 = file.readStringUntil('\n');
    
    line1.trim();
    line2.trim();
    
    file.close();
    return true;
  }

  // Escribir archivo con dos líneas (para configuración WiFi)
  bool writeConfigFile(const String& path, const String& line1, const String& line2) {
    if (!_isInitialized) return false;
    
    String content = line1 + "\n" + line2;
    return writeFile(path, content);
  }

  // Obtener tamaño del archivo
  size_t getFileSize(const String& path) {
    if (!_isInitialized) return 0;
    
    File file = SD.open(path);
    if (!file) {
      return 0;
    }
    
    size_t size = file.size();
    file.close();
    return size;
  }

  // Test de velocidad de lectura/escritura
  String testFileIO(const String& path) {
    if (!_isInitialized) return "SD no inicializada";
    
    String result = "";
    static uint8_t buf[512];
    
    // Test de escritura
    File file = SD.open(path, FILE_WRITE);
    if (!file) {
      return "Error al abrir archivo para escritura";
    }
    
    uint32_t start = millis();
    for (int i = 0; i < 100; i++) {
      file.write(buf, 512);
    }
    uint32_t writeTime = millis() - start;
    file.close();
    
    result += "Escritura: " + String(100 * 512) + " bytes en " + String(writeTime) + "ms\n";
    
    // Test de lectura
    file = SD.open(path);
    if (!file) {
      return "Error al abrir archivo para lectura";
    }
    
    start = millis();
    while (file.available()) {
      file.read(buf, 512);
    }
    uint32_t readTime = millis() - start;
    file.close();
    
    result += "Lectura: " + String(file.size()) + " bytes en " + String(readTime) + "ms";
    
    return result;
  }

private:
  SDCardService() : _isInitialized(false), _cardType(CARD_NONE), _cardSize(0) {}
  SDCardService(const SDCardService&) = delete;
  SDCardService& operator=(const SDCardService&) = delete;
  
  bool _isInitialized;
  uint8_t _cardType;
  uint64_t _cardSize;
}; 