import numpy as np
import os

def create_directory(directory):
    """Crea un directorio si no existe."""
    if not os.path.exists(directory):
        os.makedirs(directory)

def save_cost_table(matrix, filename):
    """Guarda una matriz de costos en un archivo."""
    if matrix.ndim == 1:  # Para vectores (como inserción y eliminación)
        np.savetxt(filename, matrix, fmt='%d', delimiter=' ', newline=' ')
    else:  # Para matrices 2D (como reemplazo y transposición)
        np.savetxt(filename, matrix, fmt='%d', delimiter=' ')

def generate_uniform_costs():
    """Genera tablas de costo con valores uniformes (como en el ejemplo del enunciado)."""
    # Costos uniformes: inserción=1, eliminación=1, sustitución=2, transposición=1
    insert_costs = np.ones(26, dtype=int)
    delete_costs = np.ones(26, dtype=int)
    
    replace_costs = np.full((26, 26), 2, dtype=int)
    np.fill_diagonal(replace_costs, 0)  # Costo 0 para reemplazar una letra por sí misma
    
    transpose_costs = np.ones((26, 26), dtype=int)
    
    return {
        'insert': insert_costs,
        'delete': delete_costs,
        'replace': replace_costs,
        'transpose': transpose_costs
    }

def generate_random_costs():
    """Genera tablas de costo con valores aleatorios."""
    insert_costs = np.random.randint(1, 9, 26)
    delete_costs = np.random.randint(1, 9, 26)
    
    replace_costs = np.random.randint(1, 9, (26, 26))
    np.fill_diagonal(replace_costs, 0)  # Costo 0 para reemplazar una letra por sí misma
    
    transpose_costs = np.random.randint(1, 9, (26, 26))
    
    return {
        'insert': insert_costs,
        'delete': delete_costs,
        'replace': replace_costs,
        'transpose': transpose_costs
    }




def main():
    # Definir los diferentes casos de prueba
    test_cases = {
        'uniform': generate_uniform_costs,
        'random': generate_random_costs
    }
    
    # Crear tablas de costo para cada caso
    for case_name, generator_func in test_cases.items():
        # Crear directorio para el caso
        directory = f'test_cases/{case_name}'
        create_directory(directory)
        
        # Generar y guardar las tablas de costo
        costs = generator_func()
        for cost_type, matrix in costs.items():
            filename = f'{directory}/cost_{cost_type}.txt'
            save_cost_table(matrix, filename)
        
        print(f'Generados archivos de costo para el caso: {case_name}')

if __name__ == '__main__':
    main()