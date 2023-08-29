#pragma once
#include <string>
#include <cstddef>
#include <unordered_map>

namespace chippy {
        class Shader {
                struct UniformInfo {
                        GLint location{0};
                        GLsizei count{0};
                };
                
        public:  // public constructors/destructors/overloads
                Shader(const char* vertex_shader_filepath, const char* fragment_shader_filepath);
        public:  // public member functions
                void use() const;
                [[nodiscard]] int32_t getUniformLocation(const std::string& uniform_name) const;
                [[nodiscard]] uint32_t id() const;
                
        public:  // public member variables
        private: // private member functions
        private: // private member variables
                uint32_t m_shader_program = 0;
                std::unordered_map<std::string, int32_t> m_uniforms;
        };
}