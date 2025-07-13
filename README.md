# Cardputer Components

Sistema de componentes para M5Cardputer con sistema de rutas tipo Vue Router.

## 📁 Estructura del Proyecto

```
cardputer-components/
├── 📄 cardputer-components.ino    # Archivo principal
├── 📄 theme.h                     # Configuración de temas
├── 📁 core/                       # Sistema de rutas y vistas base
│   ├── 📄 Router.h               # Sistema de rutas principal
│   ├── 📄 RoutedView.h           # Clase base para vistas con navegación
│   ├── 📄 View.h                 # Clase base para vistas
│   └── 📄 ViewManager.h          # Gestor de vistas (legacy)
├── 📁 components/                 # Componentes reutilizables
│   └── 📁 ui/                    # Componentes de interfaz
│       ├── 📄 button.h           # Componente botón
│       ├── 📄 menu.h             # Menú básico (legacy)
│       ├── 📄 ScrollableMenu.h   # Menú con scroll
│       └── 📄 title.h            # Componente título
└── 📁 views/                      # Vistas organizadas por categoría
    ├── 📁 main/                  # Vistas principales
    │   ├── 📄 MenuView.h         # Menú principal
    │   ├── 📄 HelloView.h        # Vista de saludo
    │   └── 📄 OtherView.h        # Otra vista
    ├── 📁 games/                 # Vistas de juegos
    │   └── 📄 GameView.h         # Vista de juegos
    ├── 📁 tools/                 # Vistas de herramientas
    │   └── 📄 ToolsView.h        # Vista de herramientas
    ├── 📁 system/                # Vistas del sistema
    │   ├── 📄 SettingsView.h     # Configuración
    │   └── 📄 SystemView.h       # Información del sistema
    └── 📁 about/                 # Vistas de información
        └── 📄 AboutView.h        # Acerca de
```

## 🚀 Características

### Sistema de Rutas
- **Router tipo Vue Router** - Navegación simple con strings
- **Rutas centralizadas** - Fácil configuración y mantenimiento
- **Navegación programática** - `navigate("/ruta")` y `goBack()`

### Menú con Scroll
- **Paginación automática** - Solo muestra 4 opciones por página
- **Indicador visual** - Muestra "1/2", "2/2", etc.
- **Navegación fluida** - Controles intuitivos

### Organización Modular
- **Vistas por categoría** - Fácil localización y mantenimiento
- **Componentes reutilizables** - UI components separados
- **Core system** - Lógica centralizada

## 🎮 Controles

### Menú Principal
- **`;`** - Subir en la lista
- **`.`** - Bajar en la lista
- **`Enter`** - Seleccionar opción

### Navegación
- **`Enter`** - Volver al menú principal (en cualquier vista)

## 📋 Rutas Disponibles

| Ruta | Vista | Descripción |
|------|-------|-------------|
| `/` | MenuView | Menú principal |
| `/hello` | HelloView | Vista de saludo |
| `/other` | OtherView | Otra opción |
| `/settings` | SettingsView | Configuración |
| `/games` | GameView | Juegos |
| `/tools` | ToolsView | Herramientas |
| `/system` | SystemView | Información del sistema |
| `/about` | AboutView | Acerca de |

## 🔧 Agregar Nuevas Vistas

1. **Crear la vista** en la carpeta correspondiente:
   ```cpp
   // views/games/SnakeView.h
   #pragma once
   #include "../../core/RoutedView.h"
   
   class SnakeView : public RoutedView {
     // Implementación...
   };
   ```

2. **Agregar la ruta** en el archivo principal:
   ```cpp
   router.addRoute("/snake", snakeView);
   snakeView->setRouter(&router);
   ```

3. **Agregar opción al menú** en `MenuView.h`:
   ```cpp
   _menu->addButton("Snake", snakeCallback);
   
   static void snakeCallback() {
     if (_instance) {
       _instance->navigate("/snake");
     }
   }
   ```

## 🎨 Temas

El archivo `theme.h` contiene todas las configuraciones de colores y estilos del sistema.

## 📝 Notas

- El sistema usa `RoutedView` como base para todas las vistas
- Los componentes UI están en `components/ui/`
- Las vistas están organizadas por funcionalidad
- El scroll del menú se maneja automáticamente 