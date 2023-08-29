#pragma once
#include <array>
#include "window.h"
#include "shader.h"
#include "chip8.h"

namespace chippy {
        namespace Shaders {
                enum {
                        Screen,
                        Max
                };
        }
        
        class Application {
        public:  // public constructors/destructors/overloads
                Application(int32_t scale, int32_t window_width, int32_t window_height, const char *window_title);
        public:  // public member functions
                Window &window();
                void load(const char *filepath);
                void run(double update_interval);
                void update();
                void render();
        private: // private member variables
                Window m_window;
                std::array<Shader, Shaders::Max> m_shaders{
                        Shader("assets/shaders/screen/screen.vert", "assets/shaders/screen/screen.frag")
                };
        public:  // public member variables
                chip8 m_chip_8;
        };
}
