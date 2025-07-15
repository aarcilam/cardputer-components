#pragma once
#include <map>
#include <functional>
#include "View.h"
#include "KeyboardService.h"

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
        setupKeyboardCallbacks(); // Configurar callbacks del teclado
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
  
  // Configurar callbacks del teclado para la vista actual
  void setupKeyboardCallbacks() {
    KeyboardService& keyboard = KeyboardService::getInstance();
    
    // Configurar callbacks basados en la vista actual
    if (_currentView) {
      keyboard.setOnSelect([this]() {
        if (_currentView) {
          _currentView->onSelect();
          _needsRedraw = true;
        }
      });
      
      keyboard.setOnGoBack([this]() {
        if (_currentView) {
          _currentView->onGoBack();
          _needsRedraw = true;
        }
      });
      
      keyboard.setOnNavigateNext([this]() {
        if (_currentView) {
          _currentView->onNavigateNext();
          _needsRedraw = true;
        }
      });
      
      keyboard.setOnNavigatePrev([this]() {
        if (_currentView) {
          _currentView->onNavigatePrev();
          _needsRedraw = true;
        }
      });
      
      keyboard.setOnKeyPressed([this](char key) {
        if (_currentView) {
          _currentView->handleInput(key);
          _needsRedraw = true;
        }
      });
    }
  }
  
  // Actualizar el servicio de teclado
  void updateKeyboard() {
    KeyboardService::getInstance().update();
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