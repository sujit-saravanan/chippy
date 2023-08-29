#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace chippy {
        class Window {
        public:  // public constructors/destructors/overloads
                Window(int32_t window_width, int32_t window_height, const char *window_title);
                ~Window();
                
                Window(const Window &other);
                Window(Window &&other) noexcept;
                Window &operator=(const Window &other);
                Window &operator=(Window &&other) noexcept;
                
                operator GLFWwindow *();
        public:  // public member functions
                [[nodiscard]] int32_t width() const;
                [[nodiscard]] int32_t height() const;
                [[nodiscard]] GLFWwindow* window() const;
                bool isRunning();
                void swapBuffer();
                void updateDimensions(int32_t width, int32_t height);
        public:  // public member variables
        private: // private member functions
        private: // private member variables
                GLFWwindow *m_window = nullptr;
                int32_t m_width{0};
                int32_t m_height{0};
        };
}