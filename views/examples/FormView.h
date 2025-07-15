#ifndef FORM_VIEW_H
#define FORM_VIEW_H

#include "../../core/RoutedView.h"
#include "../../components/ui/header.h"
#include "../../components/ui/InputText.h"
#include "../../core/KeyboardService.h"

/**
 * @brief Vista de formulario de ejemplo que demuestra el uso del componente InputText
 * 
 * Esta vista muestra un formulario con múltiples campos de entrada que usan
 * el KeyboardService para manejar la navegación y entrada de texto.
 */
class FormView : public RoutedView {
private:
    // Campos de entrada
    InputText* _nameField;
    InputText* _emailField;
    InputText* _ageField;
    InputText* _phoneField;
    
    // Estado del formulario
    int _currentField;
    int _totalFields;
    bool _showResults;
    bool _editingField;
    
    // Resultados
    String _formData[4]; // name, email, age, phone

public:
    FormView() : _currentField(0), _totalFields(4), _showResults(false), _editingField(false) {
        // Crear campos de entrada
        _nameField = new InputText(10, 40, 140, 20, "Nombre", 30);
        _emailField = new InputText(10, 70, 140, 20, "Email", 50);
        _ageField = new InputText(10, 100, 140, 20, "Edad", 3);
        _phoneField = new InputText(10, 130, 140, 20, "Teléfono", 15);
        
        // Configurar validadores
        _nameField->setValidator([](char c) {
            return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ';
        });
        
        _emailField->setValidator([](char c) {
            return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || 
                   (c >= '0' && c <= '9') || c == '@' || c == '.' || c == '_' || c == '-';
        });
        
        _ageField->setValidator([](char c) {
            return c >= '0' && c <= '9';
        });
        
        _phoneField->setValidator([](char c) {
            return (c >= '0' && c <= '9') || c == '+' || c == '-' || c == '(' || c == ')';
        });
        
        // Configurar callbacks
        setupFieldCallbacks();
    }
    
    ~FormView() {
        delete _nameField;
        delete _emailField;
        delete _ageField;
        delete _phoneField;
    }
    
    void draw() override {
        M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
        
        Header header("Formulario de Ejemplo");
        header.draw();
        
        if (_showResults) {
            drawResults();
        } else {
            drawForm();
        }
    }
    
    void onEnter() override {
        _currentField = 0;
        _showResults = false;
        _editingField = false;
        
        // Configurar callbacks del KeyboardService para navegación
        KeyboardService& keyboard = KeyboardService::getInstance();
        keyboard.clearCallbacks();
        
        keyboard.setOnSelect([this]() {
            if (!_editingField) {
                startEditingCurrentField();
            }
        });
        
        keyboard.setOnGoBack([this]() {
            if (_editingField) {
                stopEditingCurrentField();
            } else {
                goBack();
            }
        });
        
        keyboard.setOnNavigateNext([this]() {
            if (!_editingField) {
                _currentField = (_currentField + 1) % _totalFields;
            }
        });
        
        keyboard.setOnNavigatePrev([this]() {
            if (!_editingField) {
                _currentField = (_currentField - 1 + _totalFields) % _totalFields;
            }
        });
    }
    
    void onExit() override {
        // Desactivar todos los campos
        _nameField->deactivate();
        _emailField->deactivate();
        _ageField->deactivate();
        _phoneField->deactivate();
        _editingField = false;
    }
    
    void handleInput(char key) override {
        // Si estamos editando un campo, el InputText manejará la entrada
        if (_editingField) {
            return;
        }
        
        // Si no estamos editando, manejar navegación con teclas directas
        switch (key) {
            case ';': // Subir
                _currentField = (_currentField - 1 + _totalFields) % _totalFields;
                break;
            case '.': // Bajar
                _currentField = (_currentField + 1) % _totalFields;
                break;
            case 13: // Enter
                startEditingCurrentField();
                break;
        }
    }

private:
    void setupFieldCallbacks() {
        // Callback para el campo nombre
        _nameField->setOnConfirm([this](String text) {
            _formData[0] = text;
            _currentField = 1; // Ir al siguiente campo
            stopEditingCurrentField();
            startEditingCurrentField();
        });
        
        _nameField->setOnCancel([this]() {
            stopEditingCurrentField();
        });
        
        // Callback para el campo email
        _emailField->setOnConfirm([this](String text) {
            _formData[1] = text;
            _currentField = 2; // Ir al siguiente campo
            stopEditingCurrentField();
            startEditingCurrentField();
        });
        
        _emailField->setOnCancel([this]() {
            stopEditingCurrentField();
        });
        
        // Callback para el campo edad
        _ageField->setOnConfirm([this](String text) {
            _formData[2] = text;
            _currentField = 3; // Ir al siguiente campo
            stopEditingCurrentField();
            startEditingCurrentField();
        });
        
        _ageField->setOnCancel([this]() {
            stopEditingCurrentField();
        });
        
        // Callback para el campo teléfono
        _phoneField->setOnConfirm([this](String text) {
            _formData[3] = text;
            submitForm();
        });
        
        _phoneField->setOnCancel([this]() {
            stopEditingCurrentField();
        });
    }
    
    void startEditingCurrentField() {
        if (_editingField) return;
        
        _editingField = true;
        
        // Limpiar callbacks de navegación para que el InputText los configure
        KeyboardService& keyboard = KeyboardService::getInstance();
        keyboard.clearCallbacks();
        
        // Activar el campo actual
        switch (_currentField) {
            case 0:
                _nameField->activate();
                break;
            case 1:
                _emailField->activate();
                break;
            case 2:
                _ageField->activate();
                break;
            case 3:
                _phoneField->activate();
                break;
        }
    }
    
    void stopEditingCurrentField() {
        if (!_editingField) return;
        
        _editingField = false;
        
        // Desactivar todos los campos (esto también limpia sus callbacks)
        _nameField->deactivate();
        _emailField->deactivate();
        _ageField->deactivate();
        _phoneField->deactivate();
        
        // Restaurar callbacks de navegación
        KeyboardService& keyboard = KeyboardService::getInstance();
        keyboard.clearCallbacks();
        
        keyboard.setOnSelect([this]() {
            if (!_editingField) {
                startEditingCurrentField();
            }
        });
        
        keyboard.setOnGoBack([this]() {
            if (_editingField) {
                stopEditingCurrentField();
            } else {
                goBack();
            }
        });
        
        keyboard.setOnNavigateNext([this]() {
            if (!_editingField) {
                _currentField = (_currentField + 1) % _totalFields;
            }
        });
        
        keyboard.setOnNavigatePrev([this]() {
            if (!_editingField) {
                _currentField = (_currentField - 1 + _totalFields) % _totalFields;
            }
        });
    }
    
    void submitForm() {
        _showResults = true;
        _editingField = false;
        
        // Desactivar todos los campos
        _nameField->deactivate();
        _emailField->deactivate();
        _ageField->deactivate();
        _phoneField->deactivate();
        
        // Configurar callbacks para la vista de resultados
        KeyboardService& keyboard = KeyboardService::getInstance();
        keyboard.clearCallbacks();
        
        keyboard.setOnSelect([this]() {
            _showResults = false;
            onEnter(); // Restaurar estado inicial
        });
        
        keyboard.setOnGoBack([this]() {
            _showResults = false;
            onEnter(); // Restaurar estado inicial
        });
    }
    
    void drawForm() {
        // Título del formulario
        M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
        M5Cardputer.Display.setTextSize(1);
        M5Cardputer.Display.setCursor(10, 30);
        M5Cardputer.Display.print("Completa el formulario:");
        
        // Etiquetas de los campos
        M5Cardputer.Display.setCursor(10, 35);
        M5Cardputer.Display.print("Nombre:");
        
        M5Cardputer.Display.setCursor(10, 65);
        M5Cardputer.Display.print("Email:");
        
        M5Cardputer.Display.setCursor(10, 95);
        M5Cardputer.Display.print("Edad:");
        
        M5Cardputer.Display.setCursor(10, 125);
        M5Cardputer.Display.print("Teléfono:");
        
        // Dibujar campos
        _nameField->draw();
        _emailField->draw();
        _ageField->draw();
        _phoneField->draw();
        
        // Indicador de campo actual (solo si no estamos editando)
        if (!_editingField) {
            int indicatorY = 40 + (_currentField * 30);
            M5Cardputer.Display.setTextColor(Theme::ACCENT_COLOR);
            M5Cardputer.Display.setCursor(5, indicatorY);
            M5Cardputer.Display.print(">");
        }
        
        // Instrucciones
        M5Cardputer.Display.setTextColor(Theme::SECONDARY_TEXT_COLOR);
        M5Cardputer.Display.setCursor(10, 160);
        if (_editingField) {
            M5Cardputer.Display.print("Escribiendo... | Enter: Confirmar | Del: Cancelar");
        } else {
            M5Cardputer.Display.print(";/. Navegar | Enter: Editar | Del: Volver");
        }
    }
    
    void drawResults() {
        // Título
        M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
        M5Cardputer.Display.setTextSize(1);
        M5Cardputer.Display.setCursor(10, 30);
        M5Cardputer.Display.print("Datos del Formulario:");
        
        // Mostrar resultados
        int yPos = 50;
        M5Cardputer.Display.setCursor(10, yPos);
        M5Cardputer.Display.print("Nombre: ");
        M5Cardputer.Display.print(_formData[0]);
        
        yPos += 20;
        M5Cardputer.Display.setCursor(10, yPos);
        M5Cardputer.Display.print("Email: ");
        M5Cardputer.Display.print(_formData[1]);
        
        yPos += 20;
        M5Cardputer.Display.setCursor(10, yPos);
        M5Cardputer.Display.print("Edad: ");
        M5Cardputer.Display.print(_formData[2]);
        
        yPos += 20;
        M5Cardputer.Display.setCursor(10, yPos);
        M5Cardputer.Display.print("Teléfono: ");
        M5Cardputer.Display.print(_formData[3]);
        
        // Mensaje de confirmación
        yPos += 30;
        M5Cardputer.Display.setTextColor(Theme::ACCENT_COLOR);
        M5Cardputer.Display.setCursor(10, yPos);
        M5Cardputer.Display.print("¡Formulario completado!");
        
        // Instrucciones
        M5Cardputer.Display.setTextColor(Theme::SECONDARY_TEXT_COLOR);
        M5Cardputer.Display.setCursor(10, 160);
        M5Cardputer.Display.print("Enter: Editar | Del: Volver");
    }
};

#endif // FORM_VIEW_H 