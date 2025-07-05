# 🔧 Compilador de C

Un compilador completo de C que soporta estructuras, punteros y genera código assembly x86-64.

## 🚀 Características

- **Análisis léxico**: Scanner que genera tokens
- **Análisis sintáctico**: Parser que construye AST
- **Verificación de tipos**: Type checker
- **Generación de código**: Código assembly x86-64
- **Soporte para**: estructuras, punteros, arrays, funciones

## 🎯 Aplicación Web Interactiva

¡Ahora puedes visualizar el proceso de compilación de manera interactiva!

### 🌐 Streamlit App

Ejecuta la aplicación web para una experiencia visual:

```bash
# Instalar dependencias
pip install -r requirements.txt

# Ejecutar la aplicación
streamlit run main.py
```

O usa los scripts de inicio:
- **Windows**: `start_app.bat`
- **Linux/Mac**: `./start_app.sh`

### ✨ Características de la App

- **Editor de código C** con ejemplos predefinidos
- **Visualización de tokens** en tabla organizada
- **Estructura del AST** jerárquica
- **Código assembly** generado con syntax highlighting
- **Logs detallados** del proceso de compilación

## 🛠️ Compilación

```bash
g++ *.cpp -o program
```

## 🧪 Pruebas

```bash
# Ejecutar todos los tests
python make_test.py

# Probar la aplicación Streamlit
python test_streamlit.py
```

## 📁 Estructura

```
compiler-for-c/
├── main.py              # Aplicación Streamlit
├── main.cpp             # Programa principal
├── scanner.cpp/h        # Análisis léxico
├── parser.cpp/h         # Análisis sintáctico
├── visitor.cpp/h        # Generación de código
├── tests/               # Casos de prueba
├── requirements.txt     # Dependencias Python
└── README_STREAMLIT.md  # Documentación de la app
```

## 📖 Documentación

- [Documentación de la App Streamlit](README_STREAMLIT.md)
- [Gramática del lenguaje](grammar.txt)

## 🤝 Contribuir

1. Fork el proyecto
2. Crea una rama para tu feature
3. Commit tus cambios
4. Push a la rama
5. Abre un Pull Request
