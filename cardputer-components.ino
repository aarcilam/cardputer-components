#include <M5Cardputer.h>
#include "theme.h"
#include "core/Router.h"
#include "core/SDCardService.h"
#include "core/KeyboardService.h"
#include "views/main/MenuView.h"
#include "views/main/SplashView.h"
#include "views/main/HelloView.h"
#include "views/main/OtherView.h"
#include "views/system/SettingsView.h"
#include "views/games/GameView.h"
#include "views/tools/ToolsView.h"
#include "views/tools/NetworkView.h"
#include "views/system/SystemView.h"
#include "views/system/WifiConfigView.h"
#include "views/system/SDCardView.h"
#include "views/about/AboutView.h"
#include "views/examples/FormView.h"
#include "views/tools/HomeAssistantView.h"

Router router;
SplashView* splashView;
MenuView* menuView;
HelloView* helloView;
OtherView* otherView;
SettingsView* settingsView;
GameView* gameView;
ToolsView* toolsView;
NetworkView* networkView;
SystemView* systemView;
WifiConfigView* wifiConfigView;
SDCardView* sdCardView;
AboutView* aboutView;
FormView* formView;
HomeAssistantView* homeAssistantView;

void setup() {
  M5Cardputer.begin();
  M5Cardputer.Display.setRotation(1);
  M5Cardputer.Display.setTextSize(Theme::FONT_NORMAL);
  M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);

  // Inicializar SD Card
  SDCardService& sd = SDCardService::getInstance();
  if (!sd.begin()) {
    // Si falla la SD, mostrar error pero continuar
    M5Cardputer.Display.setCursor(10, 60);
    M5Cardputer.Display.print("Error: SD no disponible");
    delay(2000);
  }
  
  // Inicializar servicio de teclado
  KeyboardService& keyboard = KeyboardService::getInstance();
  keyboard.begin();

  // Crear las vistas
  splashView = new SplashView();
  menuView = new MenuView();
  helloView = new HelloView();
  otherView = new OtherView();
  settingsView = new SettingsView();
  gameView = new GameView();
  toolsView = new ToolsView();
  networkView = new NetworkView();
  systemView = new SystemView();
  wifiConfigView = new WifiConfigView();
  sdCardView = new SDCardView();
  aboutView = new AboutView();
  formView = new FormView();
  homeAssistantView = new HomeAssistantView();
  
  // Configurar la instancia estática de MenuView
  MenuView::setInstance(menuView);
  
  // Configurar el router con las rutas
  router.addRoute("/splash", splashView);
  router.addRoute("/", menuView);
  router.addRoute("/hello", helloView);
  router.addRoute("/other", otherView);
  router.addRoute("/settings", settingsView);
  router.addRoute("/games", gameView);
  router.addRoute("/tools", toolsView);
  router.addRoute("/network", networkView);
  router.addRoute("/system", systemView);
  router.addRoute("/wifi", wifiConfigView);
  router.addRoute("/sd", sdCardView);
  router.addRoute("/about", aboutView);
  router.addRoute("/form", formView);
  router.addRoute("/homeassistant", homeAssistantView);
  
  // Configurar las referencias al router en las vistas
  splashView->setRouter(&router);
  menuView->setRouter(&router);
  helloView->setRouter(&router);
  otherView->setRouter(&router);
  settingsView->setRouter(&router);
  gameView->setRouter(&router);
  toolsView->setRouter(&router);
  networkView->setRouter(&router);
  systemView->setRouter(&router);
  wifiConfigView->setRouter(&router);
  sdCardView->setRouter(&router);
  aboutView->setRouter(&router);
  formView->setRouter(&router);
  homeAssistantView->setRouter(&router);
  
  // Iniciar con la vista de splash
  router.setInitialRoute("/splash");
}

void loop() {
  // Actualizar el servicio de teclado (esto también actualiza M5Cardputer)
  router.updateKeyboard();

  // Actualización periódica del header (cada 2 segundos)
  static unsigned long lastHeaderUpdate = 0;
  if (millis() - lastHeaderUpdate > 2000) {
    lastHeaderUpdate = millis();
    router.refreshHeader();
  }

  // Redibujar la vista actual solo cuando es necesario
  router.draw();
}

