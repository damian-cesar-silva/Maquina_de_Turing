#ifndef TURING_MACHINE_H
#define TURING_MACHINE_H

#include <string>

enum State {
    Q_START,
    Q_DECREMENT_K,
    Q_SCAN_TAPE,
    Q_EVAL_RULE110,
    Q_HALT
};

class TuringMachineRule110 {
private:
    std::string k_binary;
    std::string tape;
    int step_count;

    // Control de la Máquina de Turing Formal
    State current_state;
    size_t head_position;
    std::string full_tape; // Contiene "K_binario#cinta#"

    long long binaryToDecimal(const std::string& bin) const;
    std::string decrementBinary(std::string bin) const;
    std::string stateToString(State st) const;

public:
    TuringMachineRule110();

    bool loadInput(const std::string& input_line);
    void step();
    
    // Ejecución estándar (muestra resumen por generación)
    void run(bool verbose = true);
    
    // Ejecución detallada (muestra cada movimiento del cabezal y estado Q)
    void runDetailedTrace();

    std::string getTape() const { return tape; }
    std::string getK() const { return k_binary; }
    int getStepCount() const { return step_count; }
};

#endif // TURING_MACHINE_H