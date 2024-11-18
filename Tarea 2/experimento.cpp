#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <limits>
#include <algorithm>

using namespace std;

// Estructura para almacenar las tablas de costos
struct CostTables {
    vector<int> insertCosts;
    vector<int> deleteCosts;
    vector<vector<int>> replaceCosts;
    vector<vector<int>> transposeCosts;
};

// Funciones de costo
int costo_ins(char b, const CostTables& costs) {
    return costs.insertCosts[b - 'a'];
}

int costo_del(char a, const CostTables& costs) {
    return costs.deleteCosts[a - 'a'];
}

int costo_sub(char a, char b, const CostTables& costs) {
    return costs.replaceCosts[a - 'a'][b - 'a'];
}

int costo_trans(char a, char b, const CostTables& costs) {
    return costs.transposeCosts[a - 'a'][b - 'a'];
}

// Implementación por fuerza bruta
int editDistanceBruteForce(const string& s1, const string& s2, int i, int j, const CostTables& costs) {
    // Casos base
    if (i == 0) return j;
    if (j == 0) return i;
    
    int minCost = numeric_limits<int>::max();
    
    // Eliminar
    minCost = min(minCost, editDistanceBruteForce(s1, s2, i-1, j, costs) + 
                  costo_del(s1[i-1], costs));
    
    // Insertar
    minCost = min(minCost, editDistanceBruteForce(s1, s2, i, j-1, costs) + 
                  costo_ins(s2[j-1], costs));
    
    // Sustituir
    minCost = min(minCost, editDistanceBruteForce(s1, s2, i-1, j-1, costs) + 
                  costo_sub(s1[i-1], s2[j-1], costs));
    
    // Transposición (si hay suficientes caracteres)
    if (i > 1 && j > 1 && s1[i-1] == s2[j-2] && s1[i-2] == s2[j-1]) {
        minCost = min(minCost, editDistanceBruteForce(s1, s2, i-2, j-2, costs) + 
                      costo_trans(s1[i-2], s1[i-1], costs));
    }
    
    return minCost;
}

// Implementación con programación dinámica
int editDistanceDP(const string& s1, const string& s2, const CostTables& costs) {
    int m = s1.length();
    int n = s2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    // Inicialización
    for (int i = 0; i <= m; i++)
        dp[i][0] = i;
    for (int j = 0; j <= n; j++)
        dp[0][j] = j;
    
    // Llenar la tabla dp
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            // Calcular costos de operaciones básicas
            int delete_cost = dp[i-1][j] + costo_del(s1[i-1], costs);
            int insert_cost = dp[i][j-1] + costo_ins(s2[j-1], costs);
            int replace_cost = dp[i-1][j-1] + costo_sub(s1[i-1], s2[j-1], costs);
            
            dp[i][j] = min({delete_cost, insert_cost, replace_cost});
            
            // Considerar transposición
            if (i > 1 && j > 1 && s1[i-1] == s2[j-2] && s1[i-2] == s2[j-1]) {
                int transpose_cost = dp[i-2][j-2] + costo_trans(s1[i-2], s1[i-1], costs);
                dp[i][j] = min(dp[i][j], transpose_cost);
            }
        }
    }
    
    return dp[m][n];
}

// Función para cargar las tablas de costos
CostTables loadCostTables() {
    CostTables costs;
    ifstream file;
    
    // Cargar costos de inserción (26 valores)
    file.open("cost_insert.txt");
    costs.insertCosts.resize(26);
    for (int i = 0; i < 26; i++) {
        file >> costs.insertCosts[i];
    }
    file.close();
    
    // Cargar costos de eliminación (26 valores)
    file.open("cost_delete.txt");
    costs.deleteCosts.resize(26);
    for (int i = 0; i < 26; i++) {
        file >> costs.deleteCosts[i];
    }
    file.close();
    
    // Cargar matriz de costos de reemplazo (26x26)
    file.open("cost_replace.txt");
    costs.replaceCosts.resize(26, vector<int>(26));
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            file >> costs.replaceCosts[i][j];
        }
    }
    file.close();
    
    // Cargar matriz de costos de transposición (26x26)
    file.open("cost_transpose.txt");
    costs.transposeCosts.resize(26, vector<int>(26));
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            file >> costs.transposeCosts[i][j];
        }
    }
    file.close();
    
    return costs;
}

// Estructura para almacenar casos de prueba
struct TestCase {
    string s1;
    string s2;
    string description;
};

int main() {
    // Cargar tablas de costos
    CostTables costs = loadCostTables();
    
    // Definir casos de prueba
    vector<TestCase> testCases = {
        {"abrupto", "computo", "Caso estandar"},
        {"", "", "Cadenas vacías"},
        {"abc", "abc", "Cadenas iguales"},
        {"ab", "ba", "Transposición simple"},
        {"abcde", "abdce", "Transposición en medio"},
        {"programming", "algorithm", "Cadenas diferentes largos"},
        {"papas", "mamas", "Caracteres repetidos"},
        {"xxyz", "zyxx", "Inversión completa"}
    };

    vector<TestCase> testCases2 = {
        {"usm", "ic", "asimetrico 1"},
        {"fedes", "gwk", "asimetrico 2"},
        {"aulasde", "hoaa", "asimetrico 3"},
        {"clasesdea", "basha", "asimetrico 4"},
        {"ramovirtual", "siuuuu", "asimetrico 5"},
        {"losdefederico", "aetrasc", "asimetrico 6"},
        {"silascampanadas", "risasjaj", "asimetrico 7"},
        {"prestigiosauniver", "zkateconz", "asimetrico 8"}
    };

        vector<TestCase> testCases3 = {
        {"ebh", "fsa", "simetrico 1"},
        {"cbxv", "gwga", "simetrico 2"},
        {"amdlg", "houdr", "simetrico 3"},
        {"mvicjh", "traszj", "simetrico 4"},
        {"cankled", "oonvnf", "simetrico 5"},
        {"aetyubvz", "ifnxkfgu", "simetrico 6"},
        {"empanadas", "clonesdes", "simetrico 7"},
        {"santamaria", "narutouzum", "simetrico 8"}
    };
    
    // Ejecutar casos de prueba
    cout << "Ejecutando casos de prueba...\n\n";
    
    for (const auto& test : testCases) {
        cout << "Caso: " << test.description << endl;
        cout << "Cadena 1: \"" << test.s1 << "\"" << endl;
        cout << "Cadena 2: \"" << test.s2 << "\"" << endl;
        
        // Medir tiempo para fuerza bruta
        auto start = chrono::high_resolution_clock::now();
        int costBF = editDistanceBruteForce(test.s1, test.s2, test.s1.length(), test.s2.length(), costs);
        auto end = chrono::high_resolution_clock::now();
        auto durationBF = chrono::duration_cast<chrono::microseconds>(end - start);
        
        // Medir tiempo para programación dinámica
        start = chrono::high_resolution_clock::now();
        int costDP = editDistanceDP(test.s1, test.s2, costs);
        end = chrono::high_resolution_clock::now();
        auto durationDP = chrono::duration_cast<chrono::microseconds>(end - start);
        
        cout << "Costo (Fuerza Bruta): " << costBF << " - Tiempo: " << durationBF.count() << " microsegundos" << endl;
        cout << "Costo (Prog. Dinámica): " << costDP << " - Tiempo: " << durationDP.count() << " microsegundos" << endl;
        cout << "-------------------\n";
    }

    for (const auto& test : testCases2) {
        cout << "Caso: " << test.description << endl;
        cout << "Cadena 1: \"" << test.s1 << "\"" << endl;
        cout << "Cadena 2: \"" << test.s2 << "\"" << endl;
        
        // Medir tiempo para fuerza bruta
        auto start = chrono::high_resolution_clock::now();
        int costBF = editDistanceBruteForce(test.s1, test.s2, test.s1.length(), test.s2.length(), costs);
        auto end = chrono::high_resolution_clock::now();
        auto durationBF = chrono::duration_cast<chrono::microseconds>(end - start);
        
        // Medir tiempo para programación dinámica
        start = chrono::high_resolution_clock::now();
        int costDP = editDistanceDP(test.s1, test.s2, costs);
        end = chrono::high_resolution_clock::now();
        auto durationDP = chrono::duration_cast<chrono::microseconds>(end - start);
        
        cout << "Costo (Fuerza Bruta): " << costBF << " - Tiempo: " << durationBF.count() << " microsegundos" << endl;
        cout << "Costo (Prog. Dinámica): " << costDP << " - Tiempo: " << durationDP.count() << " microsegundos" << endl;
        cout << "-------------------\n";
    }

    for (const auto& test : testCases3) {
        cout << "Caso: " << test.description << endl;
        cout << "Cadena 1: \"" << test.s1 << "\"" << endl;
        cout << "Cadena 2: \"" << test.s2 << "\"" << endl;
        
        // Medir tiempo para fuerza bruta
        auto start = chrono::high_resolution_clock::now();
        int costBF = editDistanceBruteForce(test.s1, test.s2, test.s1.length(), test.s2.length(), costs);
        auto end = chrono::high_resolution_clock::now();
        auto durationBF = chrono::duration_cast<chrono::microseconds>(end - start);
        
        // Medir tiempo para programación dinámica
        start = chrono::high_resolution_clock::now();
        int costDP = editDistanceDP(test.s1, test.s2, costs);
        end = chrono::high_resolution_clock::now();
        auto durationDP = chrono::duration_cast<chrono::microseconds>(end - start);
        
        cout << "Costo (Fuerza Bruta): " << costBF << " - Tiempo: " << durationBF.count() << " microsegundos" << endl;
        cout << "Costo (Prog. Dinámica): " << costDP << " - Tiempo: " << durationDP.count() << " microsegundos" << endl;
        cout << "-------------------\n";
    }
    
    return 0;
}