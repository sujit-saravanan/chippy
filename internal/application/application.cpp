#include <chrono>
#include <thread>
#include "application.h"
#include "gl_debug.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto application = reinterpret_cast<chippy::Application*>(glfwGetWindowUserPointer(window));
        switch (action) {
                case GLFW_PRESS: {
                        switch (key) {
                                case GLFW_KEY_1: application->m_chip_8.setKeyPressed(1); break;
                                case GLFW_KEY_2: application->m_chip_8.setKeyPressed(2); break;
                                case GLFW_KEY_3: application->m_chip_8.setKeyPressed(3); break;
                                case GLFW_KEY_4: application->m_chip_8.setKeyPressed(12); break;
                                case GLFW_KEY_Q: application->m_chip_8.setKeyPressed(4); break;
                                case GLFW_KEY_W: application->m_chip_8.setKeyPressed(5); break;
                                case GLFW_KEY_E: application->m_chip_8.setKeyPressed(6); break;
                                case GLFW_KEY_R: application->m_chip_8.setKeyPressed(13); break;
                                case GLFW_KEY_A: application->m_chip_8.setKeyPressed(7); break;
                                case GLFW_KEY_S: application->m_chip_8.setKeyPressed(8); break;
                                case GLFW_KEY_D: application->m_chip_8.setKeyPressed(9); break;
                                case GLFW_KEY_F: application->m_chip_8.setKeyPressed(14); break;
                                case GLFW_KEY_Z: application->m_chip_8.setKeyPressed(10); break;
                                case GLFW_KEY_X: application->m_chip_8.setKeyPressed(0); break;
                                case GLFW_KEY_C: application->m_chip_8.setKeyPressed(11); break;
                                case GLFW_KEY_V: application->m_chip_8.setKeyPressed(15); break;
                                case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(application->window(), true); break;
                                default: break;
                        }
                        break;
                }
                case GLFW_RELEASE: {
                        switch (key) {
                                case GLFW_KEY_1: application->m_chip_8.setKeyReleased(1); break;
                                case GLFW_KEY_2: application->m_chip_8.setKeyReleased(2); break;
                                case GLFW_KEY_3: application->m_chip_8.setKeyReleased(3); break;
                                case GLFW_KEY_4: application->m_chip_8.setKeyReleased(12); break;
                                case GLFW_KEY_Q: application->m_chip_8.setKeyReleased(4); break;
                                case GLFW_KEY_W: application->m_chip_8.setKeyReleased(5); break;
                                case GLFW_KEY_E: application->m_chip_8.setKeyReleased(6); break;
                                case GLFW_KEY_R: application->m_chip_8.setKeyReleased(13); break;
                                case GLFW_KEY_A: application->m_chip_8.setKeyReleased(7); break;
                                case GLFW_KEY_S: application->m_chip_8.setKeyReleased(8); break;
                                case GLFW_KEY_D: application->m_chip_8.setKeyReleased(9); break;
                                case GLFW_KEY_F: application->m_chip_8.setKeyReleased(14); break;
                                case GLFW_KEY_Z: application->m_chip_8.setKeyReleased(10); break;
                                case GLFW_KEY_X: application->m_chip_8.setKeyReleased(0); break;
                                case GLFW_KEY_C: application->m_chip_8.setKeyReleased(11); break;
                                case GLFW_KEY_V: application->m_chip_8.setKeyReleased(15); break;
                                default: break;
                        }
                        break;
                }
                default: {
                        break;
                }
        }
}

chippy::Application::Application(int32_t scale, int32_t window_width, int32_t window_height, const char *window_title) : m_window(window_width*scale, window_height*scale, window_title){
#ifndef NDEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(message_callback, nullptr);
#endif
        glViewport(0, 0, m_window.width(), m_window.height());
        
        glfwSetWindowUserPointer(m_window, this);
        glfwSetKeyCallback(m_window, keyCallback);
}
chippy::Window &chippy::Application::window() {
        return m_window;
}

void updateThread(chippy::Application* app, double update_interval) {
        auto prev_time = std::chrono::high_resolution_clock::now();
        auto curr_time = std::chrono::high_resolution_clock::now();
        
        while (app->window().isRunning()) {
                curr_time = std::chrono::high_resolution_clock::now();
                
                if (std::chrono::duration<double>(curr_time - prev_time).count() >= update_interval) {
                        app->update();
                        prev_time = curr_time;
                }
        }
}

void chippy::Application::run(double update_interval) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        m_shaders[Shaders::Screen].use();
        glUniform1i(m_shaders[Shaders::Screen].getUniformLocation("tex"), 0);
        
        std::jthread update_thread(updateThread, this, update_interval); // Runs emulation off of the render loop thread
        while (m_window.isRunning())
                render();
}
void chippy::Application::update() {
        glfwPollEvents();
        m_chip_8.decodeInstruction(m_chip_8.fetchInstruction());
}
void chippy::Application::render() {
        glClear(GL_COLOR_BUFFER_BIT);
        m_chip_8.render(0);
        
        m_window.swapBuffer();
}
void chippy::Application::load(const char *filepath) {
        m_chip_8.loadProgram(filepath);
}
