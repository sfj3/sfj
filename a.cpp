#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>  // Add this line for setprecision

class WaveGrub {
private:
    std::vector<double> t;
    std::vector<double> wave1;
    std::vector<double> wave2;
    double amp1, freq1, phase1;
    double amp2, freq2, phase2;
    static const int SIZE = 256;

public:
    WaveGrub() : 
        t(SIZE), wave1(SIZE), wave2(SIZE),
        amp1(1), freq1(1), phase1(0),
        amp2(1), freq2(1), phase2(0) {
        for (int i = 0; i < SIZE; ++i) {
            t[i] = 2 * M_PI * i / SIZE;
        }
        update_waves();
    }

    void update_waves() {
        for (int i = 0; i < SIZE; ++i) {
            wave1[i] = amp1 * std::sin(freq1 * t[i] + phase1);
            wave2[i] = amp2 * std::cos(freq2 * t[i] + phase2);
        }
    }

    void interpret(const std::string& code) {
        for (char cmd : code) {
            switch (cmd) {
                case 'A': amp1 = std::min(amp1 + 0.1, 2.0); break;
                case 'B': amp2 = std::min(amp2 + 0.1, 2.0); break;
                case 'F': freq1 = std::min(freq1 + 0.5, 10.0); break;
                case 'G': freq2 = std::min(freq2 + 0.5, 10.0); break;
                case 'P': phase1 = std::fmod(phase1 + 0.2, 2 * M_PI); break;
                case 'Q': phase2 = std::fmod(phase2 + 0.2, 2 * M_PI); break;
                case '*':
                    for (int j = 0; j < SIZE; ++j) wave1[j] *= wave2[j];
                    break;
                case '+':
                    for (int j = 0; j < SIZE; ++j) wave1[j] += wave2[j];
                    break;
                case '=':
                    print_waves();
                    break;
            }
        }
    }

    void print_waves() {
        std::cout << "Wave1: ";
        for (int i = 0; i < SIZE; i += SIZE/8) 
            std::cout << std::fixed << std::setprecision(2) << wave1[i] << " ";
        std::cout << "\nWave2: ";
        for (int i = 0; i < SIZE; i += SIZE/8) 
            std::cout << std::fixed << std::setprecision(2) << wave2[i] << " ";
        std::cout << std::endl;
    }
};

int main() {
    WaveGrub wg;
    std::string input;

    std::cout << "Welcome to a Interactive Interpreter!" << std::endl;
    std::cout << "Commands: A (increase amp1), B (increase amp2), F (increase freq1), G (increase freq2)" << std::endl;
    std::cout << "          P (increase phase1), Q (increase phase2), * (multiply waves), + (add waves)" << std::endl;
    std::cout << "          = (print waves)" << std::endl;
    std::cout << "Enter commands (or 'quit' to exit):" << std::endl;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        
        if (input == "quit") {
            break;
        }

        wg.interpret(input);
    }

    std::cout << "Thank you for using a!" << std::endl;
    return 0;
}