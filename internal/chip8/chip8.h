#pragma once
#include <cstdint>
#include <array>
#include "texture.h"
#include "buffer.h"

namespace chippy {
        class chip8 {
        public:
                enum class Registers : uint8_t {
                        V0, V1, V2, V3,
                        V4, V5, V6, V7,
                        V8, V9, VA, VB,
                        VC, VD, VE, VF
                };
        
        public: // Public Constructors/Destructors/Overloads
                chip8();
        
        public: // Public Member Functions
                uint8_t at(Registers reg); // Returns the value at a register
                void set(Registers reg, uint8_t value); // Sets the value of a register
                
                void setKeyPressed(uint32_t key_index);
                void setKeyReleased(uint32_t key_index);
                bool keyIsPressed(uint32_t key_index) const;
                
                uint16_t fetchInstruction();
                
                void decodeInstruction(uint16_t instruction);
                
                void loadProgram(const char* filepath);
                
                void render(int32_t texture_unit); // Binds display texture
        
        public: // Public Member Variables
        
        private: // Private Member Functions
        private: // Private Member Variables
                std::array<uint8_t, 4 * 1024> m_memory{0}; // 4KB
                Texture m_display_texture; // 64x32 monochrome resolution, RGBA8
                std::array<uint8_t, 64 * 4 * 32>  m_display{0}; // 64x32 monochrome resolution, RGBA8
                std::array<uint16_t, 16> m_address_stack{0};
                std::array<uint8_t, 16> m_registers{0};
                
                VertexAttributeObject m_vao;
                
                uint16_t m_program_counter{0x200};
                uint16_t m_index{0};
                
                uint16_t m_stack_pointer{0};
                uint8_t m_delay_timer{0};
                uint8_t m_sound_timer{0};
                uint16_t pressed_keys{0};
                
                uint32_t random_value{9473615};
        };
}