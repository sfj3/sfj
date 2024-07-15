#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>

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
        for (size_t i = 0; i < code.length(); ++i) {
            char cmd = code[i];
            switch (cmd) {
                case 'A': amp1 = std::min(amp1 + 0.1, 2.0); break;
                case 'a': amp1 = std::max(amp1 - 0.1, 0.0); break;
                case 'B': amp2 = std::min(amp2 + 0.1, 2.0); break;
                case 'b': amp2 = std::max(amp2 - 0.1, 0.0); break;
                case 'F': freq1 = std::min(freq1 + 0.5, 10.0); break;
                case 'f': freq1 = std::max(freq1 - 0.5, 0.5); break;
                case 'G': freq2 = std::min(freq2 + 0.5, 10.0); break;
                case 'g': freq2 = std::max(freq2 - 0.5, 0.5); break;
                case 'P': phase1 = std::fmod(phase1 + 0.2, 2 * M_PI); break;
                case 'p': phase1 = std::fmod(phase1 - 0.2 + 2 * M_PI, 2 * M_PI); break;
                case 'Q': phase2 = std::fmod(phase2 + 0.2, 2 * M_PI); break;
                case 'q': phase2 = std::fmod(phase2 - 0.2 + 2 * M_PI, 2 * M_PI); break;
                case '*':
                    for (int j = 0; j < SIZE; ++j) wave1[j] *= wave2[j];
                    break;
                case '+':
                    for (int j = 0; j < SIZE; ++j) wave1[j] += wave2[j];
                    break;
                case '-':
                    for (int j = 0; j < SIZE; ++j) wave1[j] -= wave2[j];
                    break;
                case '.':
                    std::cout << "Wave1: A=" << amp1 << " F=" << freq1 << " P=" << phase1 << std::endl;
                    std::cout << "Wave2: A=" << amp2 << " F=" << freq2 << " P=" << phase2 << std::endl;
                    break;
                case '>':
                    update_waves();
                    break;
            }
        }
    }

    void print_waves() {
        std::cout << "Wave1: ";
        for (int i = 0; i < SIZE; i += SIZE/16) std::cout << wave1[i] << " ";
        std::cout << "\nWave2: ";
        for (int i = 0; i < SIZE; i += SIZE/16) std::cout << wave2[i] << " ";
        std::cout << std::endl;
    }
};

int main() {
    WaveGrub wg;
    
    std::cout << "Initial state:" << std::endl;
    wg.print_waves();
    
    std::cout << "\nAfter AABBFFGGPPQQ>>>:" << std::endl;
    wg.interpret("AABBFFGGPPQQ>>>");
    wg.print_waves();
    
    WaveGrub wg_multiply = wg;
    WaveGrub wg_add = wg;
    
    std::cout << "\nAfter multiplication (*):" << std::endl;
    wg_multiply.interpret("*");
    wg_multiply.print_waves();
    
    std::cout << "\nAfter addition (+):" << std::endl;
    wg_add.interpret("+");
    wg_add.print_waves();
    
    return 0;
}
