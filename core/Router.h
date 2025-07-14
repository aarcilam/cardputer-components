#pragma once
#include <map>
#include <functional>
#include "View.h"

class Router {
public:
  Router() : _currentView(nullptr), _needsRedraw(true) {}
  
  // Registrar una ruta con su vista
  void addRoute(const String& path, View* view) {
    _routes[path] = view;
  }
  
  // Navegar a una ruta
  void navigate(const String& path) {
    auto it = _routes.find(path);
    if (it != _routes.end()) {
      View* newView = it->second;
      if (_currentView) {
        _currentView->onExit();
      }
      _currentView = newView;
      if (_currentView) {
        _currentView->setViewManager(nullptr); // El router maneja las vistas
        _currentView->onEnter();
        _needsRedraw = true; // Marcar para redibujar
      }
    }
  }
  
  // Obtener la vista actual
  View* getCurrentView() {
    return _currentView;
  }
  
  // Manejar input de la vista actual
  void handleInput(char key) {
    if (_currentView) {
      _currentView->handleInput(key);
      // Solo marcar para redibujar si la vista lo solicita explícitamente
      // Las vistas deben llamar markForRedraw() cuando necesiten actualizarse
    }
  }
  
  // Dibujar la vista actual
  void draw() {
    if (_currentView && _needsRedraw) {
      if (_currentView->needsFullRedraw()) {
        _currentView->clearScreen();
        _currentView->draw();
      } else {
        _currentView->drawPartial();
      }
      _needsRedraw = false; // Resetear flag después de dibujar
    }
  }
  
  // Forzar redibujo
  void forceRedraw() {
    _needsRedraw = true;
  }
  
  // Refrescar solo el header
  void refreshHeader() {
    if (_currentView) {
      _currentView->refreshHeader();
    }
  }
  
  // Establecer vista inicial
  void setInitialRoute(const String& path) {
    navigate(path);
  }
  
private:
  std::map<String, View*> _routes;
  View* _currentView;
  bool _needsRedraw;
}; 