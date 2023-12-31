#include "shader.h"
#include <iostream>
#include <memory>
#include "io.h"
#include "glad.h"

chippy::Shader::Shader(const char* vertex_shader_filepath, const char* fragment_shader_filepath) {
        std::string vertex_shader_string = readFile(vertex_shader_filepath);
        const char* vertex_shader_source = vertex_shader_string.c_str();
        uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
        glCompileShader(vertex_shader);
        
        std::string fragment_shader_string = readFile(fragment_shader_filepath);
        const char* fragment_shader_source = fragment_shader_string.c_str();
        uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
        glCompileShader(fragment_shader);
        
        m_shader_program = glCreateProgram();
        glAttachShader(m_shader_program, vertex_shader);
        glAttachShader(m_shader_program, fragment_shader);
        glLinkProgram(m_shader_program);
        
        GLint uniform_count = 0;
        glGetProgramiv(m_shader_program, GL_ACTIVE_UNIFORMS, &uniform_count);
        if (uniform_count != 0) {
                int32_t max_name_len = 0;
                glGetProgramiv(m_shader_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);
                
                auto uniform_name = std::make_unique<char[]>(max_name_len);
                
                for (GLint i = 0; i < uniform_count; ++i) {
                        glGetActiveUniform(m_shader_program, i, max_name_len, nullptr, nullptr, nullptr, uniform_name.get());
                        m_uniforms[uniform_name.get()] = glGetUniformLocation(m_shader_program, uniform_name.get());
                }
        }
        
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
}
void chippy::Shader::use() const {
        glUseProgram(m_shader_program);
}
uint32_t chippy::Shader::id() const {
        return m_shader_program;
}
int32_t chippy::Shader::getUniformLocation(const std::string &uniform_name) const {
        return m_uniforms.at(uniform_name);
}
