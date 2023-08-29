#include <iostream>
#include "window.h"

static constexpr uint8_t OPENGL_MAJOR_VERSION = 4;
static constexpr uint8_t OPENGL_MINOR_VERSION = 6;

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
        auto *user_window = static_cast<chippy::Window *>(glfwGetWindowUserPointer(window));
        user_window->updateDimensions(width, height);
        glViewport(0, 0, width, height);
}

chippy::Window::Window(int32_t window_width, int32_t window_height, const char* window_title) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        
        m_window = glfwCreateWindow(window_width, window_height, window_title, nullptr, nullptr);
        if (m_window == nullptr) {
                std::cout << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                std::exit(1);
        }
        
        glfwSetWindowUserPointer(m_window, this);
        
        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(1);
        glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
        m_width = window_width;
        m_height = window_height;
        
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
                std::cout << "Failed to initialize GLAD" << std::endl, std::exit(1);
}
chippy::Window::~Window() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
}
chippy::Window::Window(const Window &other) {
        this->m_window = other.m_window;
        this->m_width = other.m_width;
        this->m_height = other.m_height;
}
chippy::Window::Window(Window &&other) noexcept {
        this->m_window = other.m_window;
        this->m_width = other.m_width;
        this->m_height = other.m_height;
        other.m_window = nullptr;
        other.m_width = 0;
        other.m_height = 0;
}
chippy::Window &chippy::Window::operator=(const Window &other) = default;
chippy::Window &chippy::Window::operator=(Window &&other) noexcept {
        this->m_window = other.m_window;
        this->m_width = other.m_width;
        this->m_height = other.m_height;
        other.m_window = nullptr;
        other.m_width = 0;
        other.m_height = 0;
        return *this;
}
bool chippy::Window::isRunning() {
        return !glfwWindowShouldClose(m_window);
}
void chippy::Window::swapBuffer() {
        glfwSwapBuffers(m_window);
}
void chippy::Window::updateDimensions(int32_t width, int32_t height) {
        m_width = width;
        m_height = height;
}
int32_t chippy::Window::width() const {
        return m_width;
}
int32_t chippy::Window::height() const {
        return m_height;
}
GLFWwindow *chippy::Window::window() const {
        return this->m_window;
}
chippy::Window::operator GLFWwindow *() {
        return this->m_window;
}

