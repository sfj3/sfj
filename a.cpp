#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <iomanip>

class WaveGrub {
private:
    std::vector<double> t;
    std::vector<double> wave;
    std::vector<double> ref_wave;
    double amp, freq, phase;
    static const int SIZE = 256;

public:
    WaveGrub() : 
        t(SIZE), wave(SIZE), ref_wave(SIZE),
        amp(1), freq(1), phase(0) {
        for (int i = 0; i < SIZE; ++i) {
            t[i] = 2 * M_PI * i / SIZE;
            ref_wave[i] = std::sin(t[i]);  // Reference wave is a simple sine wave
        }
        update_wave();
    }

    void update_wave() {
        for (int i = 0; i < SIZE; ++i) {
            wave[i] = amp * std::sin(freq * t[i] + phase);
        }
    }

    void interpret(const std::string& code) {
        for (char cmd : code) {
            switch (cmd) {
                case 'A': amp = std::min(amp + 0.1, 2.0); break;
                case 'F': freq = std::min(freq + 0.5, 10.0); break;
                case 'P': phase = std::fmod(phase + 0.2, 2 * M_PI); break;
                case '*':
                    for (int j = 0; j < SIZE; ++j) wave[j] *= ref_wave[j];
                    break;
                case '+':
                    for (int j = 0; j < SIZE; ++j) wave[j] += ref_wave[j];
                    break;
                case '=':
                    print_waves();
                    break;
                case 'R':
                    reset_wave();
                    break;
            }
            update_wave();
        }
    }

    void reset_wave() {
        amp = 1;
        freq = 1;
        phase = 0;
    }

    void print_waves() {
        std::cout << "Current wave parameters: Amp = " << amp << ", Freq = " << freq << ", Phase = " << phase << std::endl;
        std::cout << "Wave:    ";
        for (int i = 0; i < SIZE; i += SIZE/8) 
            std::cout << std::fixed << std::setprecision(2) << wave[i] << " ";
        std::cout << "\nRef Wave:";
        for (int i = 0; i < SIZE; i += SIZE/8) 
            std::cout << std::fixed << std::setprecision(2) << ref_wave[i] << " ";
        std::cout << std::endl;
    }
};

int main() {
    WaveGrub wg;
    std::string input;

    std::cout << "Welcome to a Interactive Interpreter!" << std::endl;
    std::cout << "Commands: A (increase amplitude), F (increase frequency), P (increase phase)" << std::endl;
    std::cout << "          * (multiply with reference wave), + (add reference wave)" << std::endl;
    std::cout << "          = (print waves), R (reset wave to initial state)" << std::endl;
    std::cout << "Enter commands (or 'quit' to exit):" << std::endl;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        
        if (input == "quit") {
            break;
        }

        wg.interpret(input);
    }

    std::cout << "Thank you for using WaveGrub!" << std::endl;
    return 0;
}
