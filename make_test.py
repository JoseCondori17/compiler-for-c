#!/usr/bin/env python3
# make_test.py - Genera outputs numerados en sus carpetas

import subprocess
import re
from pathlib import Path

# Configuración
COMPILER = "./program.exe"
TEST_ROOT = Path("./tests")
CATEGORIES = ["control", "declaracion", "expresion", "funcion","punteros", "struct"]

def get_test_number(test_file):
    """Extrae el número del test (ej: test3.txt -> 3)"""
    match = re.search(r'test(\d+)\.txt$', test_file.name)
    return int(match.group(1)) if match else 0

def run_test(input_path, category_dir):
    """Ejecuta un test y genera output numerado"""
    test_num = get_test_number(input_path)
    output_name = f"output{test_num}.s"
    output_path = category_dir / output_name
    
    # Ejecutar compilador (genera output.s temporal)
    cmd = [COMPILER, str(input_path)]
    result = subprocess.run(cmd, capture_output=True, text=True)
    
    if result.returncode != 0:
        print(f"❌ Error en {input_path.name}:")
        print(result.stderr)
        return False
    
    # Renombrar el output temporal
    temp_output = Path("output.s")
    if not temp_output.exists():
        print(f"⚠️ No se generó output para {input_path.name}")
        return False
    
    temp_output.rename(output_path)
    print(f"✅ {input_path.name} -> {output_name}")
    return True

def process_tests():
    """Procesa todos los tests organizados por categorías"""
    for category in CATEGORIES:
        category_dir = TEST_ROOT / category
        print(f"\n📂 Procesando categoría: {category.upper()}")
        
        # Ordenar tests numéricamente (test1.txt, test2.txt...)
        test_files = sorted(
            category_dir.glob("test*.txt"),
            key=get_test_number
        )
        
        for test_file in test_files:
            print(f"\n🔧 Ejecutando {test_file.name}...")
            if run_test(test_file, category_dir):
                # Mostrar primeras líneas del output
                output_num = get_test_number(test_file)
                output_file = category_dir / f"output{output_num}.s"
                with open(output_file) as f:
                    print(f"📄 Primeras líneas de output{output_num}.s:")
                    for _ in range(5):  # Muestra solo 5 líneas
                        line = f.readline()
                        if not line: break
                        print(line.rstrip())

if __name__ == "__main__":
    if not Path(COMPILER).exists():
        print(f"❌ Error: Compilador {COMPILER} no encontrado")
        exit(1)

    print("🛠️  Iniciando generación de outputs...")
    process_tests()
    print("\n🎉 Proceso completado")