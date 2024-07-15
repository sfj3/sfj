#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <iomanip>
#include <random>
#include <chrono>

class WaveGrub {
private:
    std::vector<double> t;
    std::vector<double> wave;
    std::vector<double> target_wave;
    double amp, freq, phase;
    static const int SIZE = 256;

public:
    WaveGrub() : 
        t(SIZE), wave(SIZE), target_wave(SIZE),
        amp(1), freq(1), phase(0) {
        for (int i = 0; i < SIZE; ++i) {
            t[i] = 2 * M_PI * i / SIZE;
        }
        generate_target_wave();
        update_wave();
    }

    void generate_target_wave() {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::uniform_real_distribution<double> dist(0.5, 1.5);
        
        double target_amp = dist(generator);
        double target_freq = dist(generator);
        double target_phase = dist(generator) * M_PI;

        for (int i = 0; i < SIZE; ++i) {
            target_wave[i] = target_amp * std::sin(target_freq * t[i] + target_phase);
        }
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
                case 'a': amp = std::max(amp - 0.1, 0.1); break;
                case 'F': freq = std::min(freq + 0.1, 2.0); break;
                case 'f': freq = std::max(freq - 0.1, 0.1); break;
                case 'P': phase = std::fmod(phase + 0.1, 2 * M_PI); break;
                case 'p': phase = std::fmod(phase - 0.1 + 2 * M_PI, 2 * M_PI); break;
                case '=': print_waves(); break;
                case 'R': reset_wave(); break;
                case 'C': std::cout << "Current error: " << calculate_error() << std::endl; break;
            }
            update_wave();
        }
    }

    void reset_wave() {
        amp = 1;
        freq = 1;
        phase = 0;
    }

    double calculate_error() {
        double error = 0;
        for (int i = 0; i < SIZE; ++i) {
            error += std::pow(wave[i] - target_wave[i], 2);
        }
        return std::sqrt(error / SIZE);
    }

    void print_waves() {
        std::cout << "Current wave parameters: Amp = " << amp << ", Freq = " << freq << ", Phase = " << phase << std::endl;
        std::cout << "Wave:        ";
        for (int i = 0; i < SIZE; i += SIZE/8) 
            std::cout << std::fixed << std::setprecision(2) << wave[i] << " ";
        std::cout << "\nTarget Wave: ";
        for (int i = 0; i < SIZE; i += SIZE/8) 
            std::cout << std::fixed << std::setprecision(2) << target_wave[i] << " ";
        std::cout << "\nCurrent error: " << calculate_error() << std::endl;
    }
};

int main() {
    WaveGrub wg;
    std::string input;

    std::cout << "Welcome to WaveGrub Wave Matching Game!" << std::endl;
    std::cout << "Try to match the target wave by adjusting the parameters." << std::endl;
    std::cout << "Commands: A/a (increase/decrease amplitude)" << std::endl;
    std::cout << "          F/f (increase/decrease frequency)" << std::endl;
    std::cout << "          P/p (increase/decrease phase)" << std::endl;
    std::cout << "          = (print waves), R (reset wave), C (check current error)" << std::endl;
    std::cout << "Enter commands (or 'quit' to exit):" << std::endl;

    wg.print_waves();

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        
        if (input == "quit") {
            break;
        }

        wg.interpret(input);

        if (wg.calculate_error() < 0.1) {
            std::cout << "Congratulations! You've matched the wave!" << std::endl;
            break;
        }
    }

    std::cout << "Thank you for playing WaveGrub Wave Matching Game!" << std::endl;
    return 0;
}
