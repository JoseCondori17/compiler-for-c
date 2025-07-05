import streamlit as st
import subprocess
import tempfile
import os
import json
from pathlib import Path

# Configuración de la página
st.set_page_config(
    page_title="Compilador de C - Visualizador",
    page_icon="🔧",
    layout="wide",
    initial_sidebar_state="expanded"
)

# Título principal
st.title("🔧 Compilador de C - Visualizador")
st.markdown("---")

# Sidebar con información
with st.sidebar:
    st.header("Información")
    st.markdown("""
    Este compilador soporta:
    
    **Tipos de datos:**
    - `int`, `float`, `double`, `char`, `bool`, `void`
    
    **Estructuras de control:**
    - `if`, `else`, `for`, `while`, `do`
    - `break`, `continue`, `return`
    
    **Operadores:**
    - Aritméticos: `+`, `-`, `*`, `/`, `%`, `++`, `--`
    - Relacionales: `==`, `!=`, `<`, `<=`, `>`, `>=`
    - Lógicos: `&&`, `||`, `!`
    
    **Estructuras:**
    - `struct`
    - Punteros con `*` y `&`
    - Arrays con `[]`
    
    **Funciones:**
    - `printf()` para salida
    """)

# Función para ejecutar el compilador
def ejecutar_compilador(codigo_c):
    """Ejecuta el compilador con el código C proporcionado"""
    try:
        # Crear archivo temporal con el código C
        with tempfile.NamedTemporaryFile(mode='w', suffix='.txt', delete=False, encoding='utf-8') as temp_file:
            temp_file.write(codigo_c)
            temp_file_path = temp_file.name
        
        # Ejecutar el compilador
        resultado = subprocess.run(
            ["./program.exe", temp_file_path],
            capture_output=True,
            text=True,
            timeout=30
        )
        
        # Leer el archivo de salida assembly si existe
        assembly_output = ""
        output_path = Path("output.s")
        if output_path.exists():
            with open(output_path, 'r', encoding='utf-8') as f:
                assembly_output = f.read()
        
        # Limpiar archivo temporal
        os.unlink(temp_file_path)
        
        return {
            'stdout': resultado.stdout,
            'stderr': resultado.stderr,
            'returncode': resultado.returncode,
            'assembly': assembly_output
        }
    except subprocess.TimeoutExpired:
        return {
            'stdout': '',
            'stderr': 'Error: El compilador tardó demasiado en ejecutarse',
            'returncode': -1,
            'assembly': ''
        }
    except Exception as e:
        return {
            'stdout': '',
            'stderr': f'Error al ejecutar el compilador: {str(e)}',
            'returncode': -1,
            'assembly': ''
        }

# Función para extraer tokens del output
def extraer_tokens(output):
    """Extrae la información de tokens del output del compilador"""
    tokens = []
    lines = output.split('\n')
    in_scanner_section = False
    
    for line in lines:
        if "SCANNER RUN:" in line:
            in_scanner_section = True
            continue
        elif "SCANNER CORRECT" in line:
            in_scanner_section = False
            continue
        elif in_scanner_section and line.strip():
            # Parsear línea de token (formato: TIPO(texto))
            if '(' in line and ')' in line:
                try:
                    tipo_end = line.find('(')
                    tipo = line[:tipo_end].strip()
                    texto_start = tipo_end + 1
                    texto_end = line.rfind(')')
                    texto = line[texto_start:texto_end]
                    tokens.append({'tipo': tipo, 'texto': texto})
                except:
                    continue
            # Parsear línea de token simple (solo tipo)
            elif line.strip() and not line.startswith('Error:'):
                tipo = line.strip()
                tokens.append({'tipo': tipo, 'texto': ''})
    
    return tokens

# Función para extraer estructura del AST
def extraer_estructura(output):
    """Extrae la estructura del AST del output del compilador"""
    estructura = []
    lines = output.split('\n')
    in_printer_section = False
    
    for line in lines:
        if "PRINTER RUN:" in line:
            in_printer_section = True
            continue
        elif "PRINT VISITOR CORRECT" in line:
            in_printer_section = False
            break
        elif in_printer_section and line.strip():
            estructura.append(line.strip())
    
    return estructura

# Ejemplos de código C
ejemplos = {
    "Declaración simple": """int main() {
    int x = 10;
    printf(x);
    return 0;
}""",
    
    "Operaciones aritméticas": """int main() {
    int a = 5;
    int b = 3;
    int resultado = a + b * 2;
    printf(resultado);
    return 0;
}""",
    
    "Estructura de control": """int main() {
    int x = 15;
    if (x > 10) {
        printf(1);
    } else {
        printf(0);
    }
    return 0;
}""",
    
    "Bucle for": """int main() {
    int i;
    for (i = 0; i < 5; i = i + 1) {
        printf(i);
    }
    return 0;
}""",
    
    "Estructura con punteros": """int main() {
    int x = 42;
    int *ptr = &x;
    printf(*ptr);
    return 0;
}"""
}

# Interfaz principal
col1, col2 = st.columns([1, 1])

with col1:
    st.subheader("📝 Código C de Entrada")
    
    # Selector de ejemplos
    ejemplo_seleccionado = st.selectbox(
        "Cargar ejemplo:",
        ["Ninguno"] + list(ejemplos.keys())
    )
    
    # Editor de código
    if ejemplo_seleccionado != "Ninguno":
        codigo_inicial = ejemplos[ejemplo_seleccionado]
    else:
        codigo_inicial = """#include <stdio.h>

int main() {
    int x = 10;
    printf(x);
    return 0;
}"""
    
    codigo_c = st.text_area(
        "Escribe tu código C aquí:",
        value=codigo_inicial,
        height=400,
        help="Escribe código C válido. El compilador generará tokens y código assembly."
    )
    
    # Botón para compilar
    if st.button("🔨 Compilar y Analizar", type="primary", use_container_width=True):
        if codigo_c.strip():
            with st.spinner("Compilando código..."):
                resultado = ejecutar_compilador(codigo_c)
            
            # Guardar resultado en session state
            st.session_state.resultado_compilacion = resultado
            st.session_state.codigo_analizado = codigo_c
            st.rerun()
        else:
            st.error("Por favor, ingresa código C válido.")

with col2:
    st.subheader("📊 Resultados del Análisis")
    
    if 'resultado_compilacion' in st.session_state:
        resultado = st.session_state.resultado_compilacion
        
        # Pestañas para diferentes vistas
        tab1, tab2, tab3, tab4 = st.tabs(["🎯 Tokens", "🌳 Estructura", "⚙️ Assembly", "📋 Logs"])
        
        with tab1:
            st.markdown("### Análisis Léxico - Tokens Generados")
            tokens = extraer_tokens(resultado['stdout'])
            
            if tokens:
                # Crear tabla de tokens
                token_data = []
                for i, token in enumerate(tokens):
                    token_data.append({
                        "Número": i + 1,
                        "Tipo": token['tipo'],
                        "Texto": token['texto']
                    })
                
                st.dataframe(
                    token_data,
                    use_container_width=True,
                    hide_index=True
                )
                
                # Estadísticas de tokens
                col_stats1, col_stats2, col_stats3 = st.columns(3)
                with col_stats1:
                    st.metric("Total Tokens", len(tokens))
                with col_stats2:
                    tipos_unicos = len(set(token['tipo'] for token in tokens))
                    st.metric("Tipos Únicos", tipos_unicos)
                with col_stats3:
                    identificadores = len([t for t in tokens if t['tipo'] == 'IDENTIFIER'])
                    st.metric("Identificadores", identificadores)
            else:
                st.warning("No se pudieron extraer tokens del output.")
        
        with tab2:
            st.markdown("### Análisis Sintáctico - Estructura del AST")
            estructura = extraer_estructura(resultado['stdout'])
            
            if estructura:
                st.markdown("**Árbol de Análisis Sintáctico:**")
                for linea in estructura:
                    st.text(linea)
            else:
                st.warning("No se pudo extraer la estructura del AST.")
        
        with tab3:
            st.markdown("### Código Assembly Generado")
            if resultado['assembly']:
                st.code(resultado['assembly'], language='assembly')
                
                # Información sobre el assembly
                st.markdown("**Información del código assembly:**")
                lineas_asm = len(resultado['assembly'].split('\n'))
                st.metric("Líneas de Assembly", lineas_asm)
            else:
                st.warning("No se generó código assembly.")
        
        with tab4:
            st.markdown("### Logs de Compilación")
            
            if resultado['stdout']:
                st.markdown("**Output estándar:**")
                st.code(resultado['stdout'], language='text')
            
            if resultado['stderr']:
                st.markdown("**Errores:**")
                st.error(resultado['stderr'])
            
            if resultado['returncode'] == 0:
                st.success("✅ Compilación exitosa")
            else:
                st.error(f"❌ Compilación falló (código: {resultado['returncode']})")
    else:
        st.info("👆 Escribe código C y presiona 'Compilar y Analizar' para ver los resultados.")

# Footer
st.markdown("---")
st.markdown(
    """
    <div style='text-align: center; color: #666;'>
        <p>Compilador de C - Visualizador Interactivo | Desarrollado con Streamlit</p>
    </div>
    """,
    unsafe_allow_html=True
)
