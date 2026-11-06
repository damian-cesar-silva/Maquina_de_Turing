#include "TuringMachine.h"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>

void processSingleInput(const std::string& input_line, bool detailed = false) {
    if (input_line.empty() || input_line[0] == ';') return;
    
    TuringMachineRule110 tm;
    if (tm.loadInput(input_line)) {
        if (detailed) {
            tm.runDetailedTrace();
        } else {
            tm.run(true);
        }
    }
}

void processTestCasesFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo '" << filename << "'\n";
        return;
    }

    std::string line;
    int test_num = 1;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == ';') continue;
        std::cout << "\n--- CASO DE PRUEBA #" << test_num++ << ": " << line << " ---\n";
        processSingleInput(line, false);
    }
    file.close();
}

void showMenu() {
    std::cout << "\n====================================================\n";
    std::cout << " SIMULADOR MAQUINA DE TURING - REGLA 110\n";
    std::cout << "====================================================\n";
    std::cout << "1. Ingresar cadena manualmente (Modo Resumen)\n";
    std::cout << "2. Ingresar cadena manualmente (Modo Traza Detallada de MT)\n";
    std::cout << "3. Cargar casos de prueba desde archivo (.txt)\n";
    std::cout << "4. Ejecutar ejemplos integrados de la consigna\n";
    std::cout << "5. Salir\n";
    std::cout << "----------------------------------------------------\n";
    std::cout << "Seleccione una opcion (1-5): ";
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        processTestCasesFile(argv[1]);
    }

    int option = 0;
    bool running = true;

    while (running) {
        showMenu();

        if (!(std::cin >> option)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nOpcion invalida.\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (option) {
            case 1: {
                std::cout << "\nIngrese hilera (K_binario#cinta#): ";
                std::string line;
                std::getline(std::cin, line);
                processSingleInput(line, false);
                break;
            }
            case 2: {
                std::cout << "\nIngrese hilera para Traza Detallada (K_binario#cinta#): ";
                std::string line;
                std::getline(std::cin, line);
                processSingleInput(line, true);
                break;
            }
            case 3: {
                std::cout << "\nNombre del archivo (.txt): ";
                std::string filepath;
                std::getline(std::cin, filepath);
                processTestCasesFile(filepath);
                break;
            }
            case 4:
                std::cout << "\n--- EJEMPLO CONSIGNA ---\n";
                processSingleInput("100#000000010000000#", false);
                break;
            case 5:
                std::cout << "\n¡Saliendo del simulador!\n";
                running = false;
                break;
            default:
                std::cout << "\nOpcion fuera de rango.\n";
                break;
        }

        if (running) {
            std::cout << "\nPresione ENTER para continuar...";
            std::cin.get();
        }
    }

    return 0;
}