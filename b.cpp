#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

class WaveGrub {
private:
    static const int SIZE = 256;
    std::vector<double> t;
    std::vector<double> wave1;
    std::vector<double> wave2;
    double amp1, freq1, phase1;
    double amp2, freq2, phase2;

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
            }
        }
        update_waves();
    }

    int find_best_match() {
        double best_match = std::numeric_limits<double>::lowest();
        int best_index = -1;
        std::vector<double> original_wave1 = wave1;  // Save original wave1

        for (int i = 0; i < SIZE; ++i) {
            double sum = 0;
            for (int j = 0; j < SIZE; ++j) {
                sum += wave1[j] * wave2[j];  // Cross-correlation
            }
            if (sum > best_match) {
                best_match = sum;
                best_index = i;
            }
            // Rotate wave1
            std::rotate(wave1.begin(), wave1.begin() + 1, wave1.end());
        }

        wave1 = original_wave1;  // Restore original wave1
        return best_index;
    }

    void print_waves() {
        std::cout << "Wave1: ";
        for (int i = 0; i < SIZE; i += SIZE/8) 
            std::cout << wave1[i] << " ";
        std::cout << "\nWave2: ";
        for (int i = 0; i < SIZE; i += SIZE/8) 
            std::cout << wave2[i] << " ";
        std::cout << std::endl;
    }
};

int main() {
    WaveGrub wg;

    // Modify wave1
    wg.interpret("AAFFPP");

    // Modify wave2
    wg.interpret("BBGGQQ");

    std::cout << "Initial waves:" << std::endl;
    wg.print_waves();

    // Find best match
    int best_index = wg.find_best_match();
    std::cout << "Best match found at index: " << best_index << std::endl;

    // Perform some operations
    wg.interpret("*+");

    std::cout << "Waves after operations:" << std::endl;
    wg.print_waves();

    // Find best match again
    best_index = wg.find_best_match();
    std::cout << "Best match found at index: " << best_index << std::endl;

    return 0;
}
