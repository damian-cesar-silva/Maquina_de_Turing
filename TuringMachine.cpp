#include "TuringMachine.h"
#include "Rule110.h"
#include <iostream>
#include <iomanip>
#include <omp.h> 


TuringMachineRule110::TuringMachineRule110() 
    : step_count(0), current_state(Q_START), head_position(0) {}

std::string TuringMachineRule110::stateToString(State st) const {
    switch (st) {
        case Q_START:        return "q_START";
        case Q_DECREMENT_K:  return "q_DEC_K";
        case Q_SCAN_TAPE:    return "q_SCAN";
        case Q_EVAL_RULE110: return "q_EVAL_R110";
        case Q_HALT:         return "q_HALT";
        default:             return "q_UNKNOWN";
    }
}

long long TuringMachineRule110::binaryToDecimal(const std::string& bin) const {
    long long val = 0;
    for (char bit : bin) {
        val = (val << 1) + (bit - '0');
    }
    return val;
}

std::string TuringMachineRule110::decrementBinary(std::string bin) const {
    int i = static_cast<int>(bin.length()) - 1;
    while (i >= 0 && bin[i] == '0') {
        bin[i] = '1';
        i--;
    }
    if (i >= 0) {
        bin[i] = '0';
    }
    size_t first_one = bin.find_first_of('1');
    if (first_one != std::string::npos) {
        return bin.substr(first_one);
    }
    return "0";
}

bool TuringMachineRule110::loadInput(const std::string& input_line) {
    size_t first_hash = input_line.find('#');
    size_t last_hash = input_line.rfind('#');

    if (first_hash == std::string::npos || last_hash == std::string::npos || first_hash == last_hash) {
        std::cerr << "Error: Formato invalido. Formato esperado: K_binario#cinta_inicial#\n";
        return false;
    }

    k_binary = input_line.substr(0, first_hash);
    tape = input_line.substr(first_hash + 1, last_hash - first_hash - 1);
    
    full_tape = input_line;
    head_position = 0;
    current_state = Q_START;

    if (tape.length() < 8) {
        std::cerr << "Advertencia: Longitud N (" << tape.length() << ") menor al minimo N >= 8.\n";
    }

    step_count = 0;
    return true;
}

void TuringMachineRule110::step() {
    int n = static_cast<int>(tape.length());
    std::string next_tape = tape;

    // Distribución paralela del cálculo de la Regla 110 en todos los núcleos
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; ++i) {
        char left   = tape[(i - 1 + n) % n];
        char center = tape[i];
        char right  = tape[(i + 1) % n];

        next_tape[i] = Rule110::evaluate(left, center, right);
    }

    tape = next_tape;
    k_binary = decrementBinary(k_binary);
    step_count++;
}

void TuringMachineRule110::run(bool verbose) {
    if (verbose) {
        std::cout << "====================================================\n";
        std::cout << " Gen 0: #" << tape << "# | K = " << k_binary << "\n";
        std::cout << "====================================================\n";
    }

    while (binaryToDecimal(k_binary) > 0) {
        step();
        if (verbose) {
            std::cout << " Gen " << std::setw(3) << step_count 
                      << ": #" << tape << "# | K restante = " << k_binary << "\n";
        }
    }

    if (verbose) {
        std::cout << "----------------------------------------------------\n";
        std::cout << " Estado Final (HALT): #" << tape << "#\n";
        std::cout << "====================================================\n\n";
    }
}

void TuringMachineRule110::runDetailedTrace() {
    std::cout << "\n========================================================\n";
    std::cout << "  TRAZA PASO A PASO DE LA MAQUINA DE TURING (MICRO-PASOS) \n";
    std::cout << "========================================================\n\n";

    current_state = Q_START;
    size_t offset_tape = k_binary.length() + 1; // Inicio de la cinta binaria N tras el primer '#'
    size_t n = tape.length();

    while (current_state != Q_HALT) {
        
        switch (current_state) {
            case Q_START:
                head_position = 0;
                std::cout << "[Estado: " << std::setw(12) << std::left << stateToString(current_state) << "] Cinta: " << full_tape << "\n";
                std::cout << "                     Cabezal: ";
                for (size_t i = 0; i < head_position; ++i) std::cout << " ";
                std::cout << "^\n";
                std::cout << " -> Accion: delta(q_START, '" << full_tape[head_position] << "') -> (q_DEC_K, '" << full_tape[head_position] << "', R)\n\n";
                
                current_state = Q_DECREMENT_K;
                break;

            case Q_DECREMENT_K:
                std::cout << "[Estado: " << std::setw(12) << std::left << stateToString(current_state) << "] Cinta: " << full_tape << "\n";
                std::cout << "                     Cabezal: ";
                for (size_t i = 0; i < head_position; ++i) std::cout << " ";
                std::cout << "^\n";
                std::cout << " -> Accion: Restar 1 a K binario. Mover cabezal a la derecha (R) buscando '#'\n\n";
                
                k_binary = decrementBinary(k_binary);
                full_tape = k_binary + "#" + tape + "#";
                offset_tape = k_binary.length() + 1; // Recalcular desfase por cambio en longitud de K
                head_position = k_binary.length();
                current_state = Q_SCAN_TAPE;
                break;

            case Q_SCAN_TAPE:
                std::cout << "[Estado: " << std::setw(12) << std::left << stateToString(current_state) << "] Cinta: " << full_tape << "\n";
                std::cout << "                     Cabezal: ";
                for (size_t i = 0; i < head_position; ++i) std::cout << " ";
                std::cout << "^\n";
                std::cout << " -> Accion: Cabezal en '#'. Cruzar a la primera celda binaria del automata (R)\n\n";
                
                head_position = offset_tape;
                current_state = Q_EVAL_RULE110;
                break;

            case Q_EVAL_RULE110: {
                std::string next_tape = tape;
                
                // BARRIDO MICRO-PASO: El cabezal avanza celda por celda (i = 0 hasta n-1)
                for (size_t i = 0; i < n; ++i) {
                    head_position = offset_tape + i;
                    
                    char left   = tape[(i - 1 + n) % n];
                    char center = tape[i];
                    char right  = tape[(i + 1) % n];
                    char new_val = Rule110::evaluate(left, center, right);
                    next_tape[i] = new_val;

                    std::cout << "[Estado: " << std::setw(12) << std::left << stateToString(current_state) << "] Cinta: " << full_tape << "\n";
                    std::cout << "                     Cabezal: ";
                    for (size_t k = 0; k < head_position; ++k) std::cout << " ";
                    std::cout << "^\n";
                    std::cout << " -> Celda " << i << ": Vecindad (" << left << center << right << ") => Escribir '" << new_val << "', Mover R\n\n";
                }

                tape = next_tape;
                step_count++;
                full_tape = k_binary + "#" + tape + "#";
                head_position = offset_tape + n; // Queda sobre el '#' final

                std::cout << "[Estado: q_CHECK_END ] Cinta: " << full_tape << "\n";
                std::cout << "                     Cabezal: ";
                for (size_t k = 0; k < head_position; ++k) std::cout << " ";
                std::cout << "^\n";
                std::cout << " -> Fin de cinta alcanzado ('#'). Gen " << step_count << " completada. K restante: " << k_binary << "\n\n";

                if (binaryToDecimal(k_binary) > 0) {
                    current_state = Q_DECREMENT_K;
                    head_position = 0; // Rewind al inicio
                } else {
                    current_state = Q_HALT;
                }
                break;
            }

            default:
                current_state = Q_HALT;
                break;
        }
    }

    // Estado Final de Parada (HALT)
    std::cout << "[Estado: " << std::setw(12) << std::left << stateToString(Q_HALT) << "] Cinta: " << full_tape << "\n";
    std::cout << "                     Cabezal: ";
    for (size_t i = 0; i < full_tape.length() - 1; ++i) std::cout << " ";
    std::cout << "^\n";
    std::cout << " -> MAQUINA DETENIDA (HALT). K ha llegado a 0.\n";
    std::cout << "========================================================\n\n";
}