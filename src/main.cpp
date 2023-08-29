#include <iostream>
#include "application.h"

static constexpr inline unsigned int Hash(std::string_view str) {
        unsigned int hash = 5381;
        for (char c: str)
                hash = ((hash << 5) + hash) + static_cast<unsigned int>(c);
        return hash;
}

static constexpr inline unsigned int operator "" _(char const *chr, size_t) noexcept { return Hash(chr); }

int main(int argc, char *argv[]) {
        int32_t scale = 16;
        const char *program_filepath = "assets/programs/Keypad Test [Hap, 2006].ch8";
        double tick_rate = 700.0;
        
        for (int i = 1; i < argc; i++) {
                switch (Hash(argv[i])) {
                        case "-scale"_: {
                                if (argc <= i + 1) {
                                        std::cout << "No value for scale\n";
                                        std::exit(1);
                                }
                                scale = atoi(argv[++i]);
                                if (scale == 0) {
                                        std::cout << "Invalid scale: " << argv[i + 1] << "\n";
                                        std::exit(1);
                                }
                                break;
                        }
                        case "-program"_: {
                                if (argc <= i + 1) {
                                        std::cout << "No value for program\n";
                                        std::exit(1);
                                }
                                program_filepath = argv[++i];
                                break;
                        }
                        case "-tickrate"_: {
                                if (argc <= i + 1) {
                                        std::cout << "No value for tickrate\n";
                                        std::exit(1);
                                }
                                tick_rate = atof(argv[++i]);
                                if (tick_rate == 0) {
                                        std::cout << "Invalid tickrate: " << argv[i + 1] << "\n";
                                        std::exit(1);
                                }
                                break;
                        }
                        default: {
                                std::cout << "Invalid prameter: " << argv[i] << "\n";
                                std::exit(1);
                        }
                }
        }
        
        
        
        // Application Creation
        chippy::Application application(scale, 64, 32, "Chippy");
        
        // Initialize Program
        application.load(program_filepath);
        
        // Main Loop
        application.run(1.0 / tick_rate);
        return 0;
}