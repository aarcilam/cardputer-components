#pragma once
#include "../../core/RoutedView.h"
#include "../../components/ui/ScrollableMenu.h"
#include "../../components/ui/header.h"
#include "../../core/KeyboardService.h"

class MenuView : public RoutedView {
private:
  bool _needsFullRedraw = true;

public:
  MenuView() {
    _menu = new ScrollableMenu(10, 35, M5Cardputer.Display.width() - 20, 3); // 3 opciones visibles a la vez, posicionado debajo del header
    _menu->addButton("Saludar", sayHelloCallback);
    _menu->addButton("Otra opcion", otherOptionCallback);
    _menu->addButton("Configuracion", settingsCallback);
    _menu->addButton("Juegos", gamesCallback);
    _menu->addButton("Herramientas", toolsCallback);
    _menu->addButton("Red", networkCallback);
    _menu->addButton("Home Assistant", homeAssistantCallback);
    _menu->addButton("Config WiFi", wifiConfigCallback);
    _menu->addButton("SD Card", sdCardCallback);
    _menu->addButton("Sistema", systemCallback);
    _menu->addButton("Acerca de", aboutCallback);
  }
  
  ~MenuView() {
    delete _menu;
  }
  
  void draw() override {
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
    Header header("Menu Principal");
    header.draw();
    _menu->draw();
  }
  
  bool needsFullRedraw() override {
    return _needsFullRedraw;
  }
  
  void drawPartial() override {
    // Solo redibujar la selección del menú, no limpiar toda la pantalla
    _menu->drawSelectionOnly();
    _needsFullRedraw = false;
  }
  
  void handleInput(char key) override {
    // El KeyboardService maneja la mayoría de las teclas
    // Este método se mantiene para compatibilidad con teclas específicas
  }
  
  void onSelect() override {
    _menu->activateSelected();
  }
  
  void onNavigateNext() override {
    int previousPage = _menu->getCurrentPage();
    _menu->selectDown();
    int currentPage = _menu->getCurrentPage();
    _needsFullRedraw = (currentPage != previousPage);
    markForRedraw();
  }
  
  void onNavigatePrev() override {
    int previousPage = _menu->getCurrentPage();
    _menu->selectUp();
    int currentPage = _menu->getCurrentPage();
    _needsFullRedraw = (currentPage != previousPage);
    markForRedraw();
  }
  
  void onEnter() override {
    _needsFullRedraw = true; // Redibujar completo al entrar
  }
  
  void refreshHeader() override {
    // Refrescar solo el header sin afectar el menú
    Header header("Menu Principal");
    header.refreshHeader();
  }
  
  // Callbacks estáticos
  static void sayHelloCallback() {
    if (_instance) {
      _instance->navigate("/hello");
    }
  }
  
  static void otherOptionCallback() {
    if (_instance) {
      _instance->navigate("/other");
    }
  }
  
  static void settingsCallback() {
    if (_instance) {
      _instance->navigate("/settings");
    }
  }
  
  static void gamesCallback() {
    if (_instance) {
      _instance->navigate("/games");
    }
  }
  
  static void toolsCallback() {
    if (_instance) {
      _instance->navigate("/tools");
    }
  }
  
  static void networkCallback() {
    if (_instance) {
      _instance->navigate("/network");
    }
  }
  
  static void wifiConfigCallback() {
    if (_instance) {
      _instance->navigate("/wifi");
    }
  }
  
  static void sdCardCallback() {
    if (_instance) {
      _instance->navigate("/sd");
    }
  }
  
  static void systemCallback() {
    if (_instance) {
      _instance->navigate("/system");
    }
  }
  
  static void aboutCallback() {
    if (_instance) {
      _instance->navigate("/about");
    }
  }
  
  static void homeAssistantCallback() {
    if (_instance) {
      _instance->navigate("/homeassistant");
    }
  }
  
  // Método para establecer la instancia
  static void setInstance(MenuView* instance) {
    _instance = instance;
  }
  
private:
  ScrollableMenu* _menu;
  static MenuView* _instance;
};

// Definición de la variable estática
MenuView* MenuView::_instance = nullptr; 