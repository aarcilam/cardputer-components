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
    
    // Resultados
    String _formData[4]; // name, email, age, phone

public:
    FormView() : _currentField(0), _totalFields(4), _showResults(false) {
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
        
        // Activar el primer campo
        activateCurrentField();
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
        activateCurrentField();
    }
    
    void onExit() override {
        // Desactivar todos los campos
        _nameField->deactivate();
        _emailField->deactivate();
        _ageField->deactivate();
        _phoneField->deactivate();
    }
    
    void onNavigateNext() override {
        if (!_showResults) {
            _currentField = (_currentField + 1) % _totalFields;
            activateCurrentField();
        }
    }
    
    void onNavigatePrev() override {
        if (!_showResults) {
            _currentField = (_currentField - 1 + _totalFields) % _totalFields;
            activateCurrentField();
        }
    }
    
    void onSelect() override {
        if (_showResults) {
            // Volver al formulario
            _showResults = false;
            activateCurrentField();
        } else {
            // Activar el campo actual
            activateCurrentField();
        }
    }
    
    void onGoBack() override {
        if (_showResults) {
            _showResults = false;
            activateCurrentField();
        } else {
            goBack();
        }
    }
    
    void handleInput(char key) override {
        // Manejar teclas específicas si es necesario
    }

private:
    void setupFieldCallbacks() {
        // Callback para el campo nombre
        _nameField->setOnConfirm([this](String text) {
            _formData[0] = text;
            nextField();
        });
        
        _nameField->setOnCancel([this]() {
            cancelField();
        });
        
        // Callback para el campo email
        _emailField->setOnConfirm([this](String text) {
            _formData[1] = text;
            nextField();
        });
        
        _emailField->setOnCancel([this]() {
            cancelField();
        });
        
        // Callback para el campo edad
        _ageField->setOnConfirm([this](String text) {
            _formData[2] = text;
            nextField();
        });
        
        _ageField->setOnCancel([this]() {
            cancelField();
        });
        
        // Callback para el campo teléfono
        _phoneField->setOnConfirm([this](String text) {
            _formData[3] = text;
            submitForm();
        });
        
        _phoneField->setOnCancel([this]() {
            cancelField();
        });
    }
    
    void activateCurrentField() {
        // Desactivar todos los campos
        _nameField->deactivate();
        _emailField->deactivate();
        _ageField->deactivate();
        _phoneField->deactivate();
        
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
    
    void nextField() {
        _currentField = (_currentField + 1) % _totalFields;
        activateCurrentField();
    }
    
    void cancelField() {
        // Mantener el campo actual activo
        activateCurrentField();
    }
    
    void submitForm() {
        _showResults = true;
        
        // Configurar callbacks para la vista de resultados
        KeyboardService& keyboard = KeyboardService::getInstance();
        keyboard.clearCallbacks();
        
        keyboard.setOnSelect([this]() {
            _showResults = false;
            activateCurrentField();
        });
        
        keyboard.setOnGoBack([this]() {
            _showResults = false;
            activateCurrentField();
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
        
        // Indicador de campo actual
        int indicatorY = 40 + (_currentField * 30);
        M5Cardputer.Display.setTextColor(Theme::ACCENT_COLOR);
        M5Cardputer.Display.setCursor(5, indicatorY);
        M5Cardputer.Display.print(">");
        
        // Instrucciones
        M5Cardputer.Display.setTextColor(Theme::SECONDARY_TEXT_COLOR);
        M5Cardputer.Display.setCursor(10, 160);
        M5Cardputer.Display.print(";/. Navegar | Enter: Editar | Del: Volver");
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