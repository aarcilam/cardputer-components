#pragma once
#include "View.h"

// Forward declaration
class Router;

class RoutedView : public View {
public:
  virtual ~RoutedView() = default;
  
  // Establecer referencia al router
  void setRouter(Router* router) {
    _router = router;
  }
  
  // Marcar para redibujar
  void markForRedraw() {
    if (_router) {
      _router->forceRedraw();
    }
  }
  
protected:
  // Métodos de navegación simplificados
  void navigate(const String& path) {
    if (_router) {
      _router->navigate(path);
    }
  }
  
  void goBack(const String& path = "/") {
    // Por ahora navega al menú principal
    navigate(path);
  }
  
private:
  Router* _router = nullptr;
}; 