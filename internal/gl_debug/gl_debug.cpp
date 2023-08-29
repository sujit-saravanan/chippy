#include "gl_debug.h"
#include <iostream>

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const *message, void const *user_param) {
        auto const src_str = [source]() {
                switch (source) {
                        case GL_DEBUG_SOURCE_API:
                                return "API";
                        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                                return "WINDOW SYSTEM";
                        case GL_DEBUG_SOURCE_SHADER_COMPILER:
                                return "SHADER COMPILER";
                        case GL_DEBUG_SOURCE_THIRD_PARTY:
                                return "THIRD PARTY";
                        case GL_DEBUG_SOURCE_APPLICATION:
                                return "APPLICATION";
                        default:
                                return "OTHER";
                }
        }();
        
        auto const type_str = [type]() {
                switch (type) {
                        case GL_DEBUG_TYPE_ERROR:
                                return "ERROR";
                        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                                return "DEPRECATED_BEHAVIOR";
                        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                                return "UNDEFINED_BEHAVIOR";
                        case GL_DEBUG_TYPE_PORTABILITY:
                                return "PORTABILITY";
                        case GL_DEBUG_TYPE_PERFORMANCE:
                                return "PERFORMANCE";
                        case GL_DEBUG_TYPE_MARKER:
                                return "MARKER";
                        default:
                                return "OTHER";
                }
        }();
        
        auto const severity_str = [severity]() {
                switch (severity) {
                        case GL_DEBUG_SEVERITY_NOTIFICATION:
                                return "NOTIFICATION";
                        case GL_DEBUG_SEVERITY_LOW:
                                return "LOW";
                        case GL_DEBUG_SEVERITY_MEDIUM:
                                return "MEDIUM";
                        case GL_DEBUG_SEVERITY_HIGH:
                                return "HIGH";
                        default:
                                return "OTHER";
                }
        }();
        std::cout << "Source: " << src_str << "\nType: " << type_str << "\nSeverity: " << severity_str << "\nID: " << id << "\nMessage: " << message << "\n\n";
        if (type == GL_DEBUG_TYPE_ERROR)
                std::exit(-1);
}
