#!/usr/bin/env python3
# make_test.py - Versión corregida para Windows

import subprocess
import re
import os
from pathlib import Path

# Configuración
COMPILER = "./program.exe"
TEST_ROOT = Path("./tests")
CATEGORIES = ["control", "declaracion", "expresion", "funcion","punteros","struct"]

def get_test_number(test_file):
    """Extrae el número del test (ej: test3.txt -> 3)"""
    match = re.search(r'test(\d+)\.txt$', test_file.name, re.IGNORECASE)
    return int(match.group(1)) if match else 0

def run_test(input_path, category_dir):
    """Ejecuta un test y genera output numerado"""
    test_num = get_test_number(input_path)
    output_name = f"output{test_num}.s"
    output_path = category_dir / output_name
    
    # Eliminar output anterior si existe
    if output_path.exists():
        try:
            output_path.unlink()
        except PermissionError:
            print(f"⚠️ No se pudo eliminar {output_name} existente")
            return False
    
    # Ejecutar compilador
    cmd = [COMPILER, str(input_path)]
    result = subprocess.run(cmd, capture_output=True, text=True)
    
    if result.returncode != 0:
        print(f"❌ Error en {input_path.name}:")
        print(result.stderr)
        return False
    
    # Mover/renombrar el output
    temp_output = Path("output.s")
    if not temp_output.exists():
        print(f"⚠️ No se generó output para {input_path.name}")
        return False
    
    try:
        temp_output.rename(output_path)
    except FileExistsError:
        print(f"⚠️ Error: {output_name} ya existe y está bloqueado")
        return False
    
    print(f"✅ {input_path.name} -> {output_name}")
    return True

def process_tests():
    """Procesa todos los tests organizados por categorías"""
    for category in CATEGORIES:
        category_dir = TEST_ROOT / category
        if not category_dir.exists():
            print(f"⚠️ Carpeta {category} no encontrada")
            continue
            
        print(f"\n📂 Procesando categoría: {category.upper()}")
        
        # Ordenar tests numéricamente
        test_files = sorted(
            [f for f in category_dir.glob("test*.txt") if get_test_number(f) > 0],
            key=get_test_number
        )
        
        if not test_files:
            print("No se encontraron tests válidos (test1.txt, test2.txt, etc.)")
            continue
            
        for test_file in test_files:
            print(f"\n🔧 Ejecutando {test_file.name}...")
            if run_test(test_file, category_dir):
                output_num = get_test_number(test_file)
                output_file = category_dir / f"output{output_num}.s"
                try:
                    with open(output_file) as f:
                        print("📄 Contenido generado:")
                        print(f.read(200) + "...")  # Muestra solo los primeros 200 caracteres
                except Exception as e:
                    print(f"⚠️ Error leyendo output: {e}")

if __name__ == "__main__":
    if not Path(COMPILER).exists():
        print(f"❌ Error: Compilador {COMPILER} no encontrado")
        exit(1)

    print("🛠️  Iniciando generación de outputs...")
    process_tests()
    print("\n🎉 Proceso completado")