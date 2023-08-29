#include "chip8.h"
#include <cstring>
#include "io.h"

chippy::chip8::chip8() : m_display_texture(Texture(64, 32, 4)) {
        // Initialize Font
        static constexpr std::array<std::array<uint8_t, 5>, 16> font = {
                std::array<uint8_t, 5>{0xF0, 0x90, 0x90, 0x90, 0xF0}, // 0
                std::array<uint8_t, 5>{0x20, 0x60, 0x20, 0x20, 0x70}, // 1
                std::array<uint8_t, 5>{0xF0, 0x10, 0xF0, 0x80, 0xF0}, // 2
                std::array<uint8_t, 5>{0xF0, 0x10, 0xF0, 0x10, 0xF0}, // 3
                std::array<uint8_t, 5>{0x90, 0x90, 0xF0, 0x10, 0x10}, // 4
                std::array<uint8_t, 5>{0xF0, 0x80, 0xF0, 0x10, 0xF0}, // 5
                std::array<uint8_t, 5>{0xF0, 0x80, 0xF0, 0x90, 0xF0}, // 6
                std::array<uint8_t, 5>{0xF0, 0x10, 0x20, 0x40, 0x40}, // 7
                std::array<uint8_t, 5>{0xF0, 0x90, 0xF0, 0x90, 0xF0}, // 8
                std::array<uint8_t, 5>{0xF0, 0x90, 0xF0, 0x10, 0xF0}, // 9
                std::array<uint8_t, 5>{0xF0, 0x90, 0xF0, 0x90, 0x90}, // A
                std::array<uint8_t, 5>{0xE0, 0x90, 0xE0, 0x90, 0xE0}, // B
                std::array<uint8_t, 5>{0xF0, 0x80, 0x80, 0x80, 0xF0}, // C
                std::array<uint8_t, 5>{0xE0, 0x90, 0x90, 0x90, 0xE0}, // D
                std::array<uint8_t, 5>{0xF0, 0x80, 0xF0, 0x80, 0xF0}, // E
                std::array<uint8_t, 5>{0xF0, 0x80, 0xF0, 0x80, 0x80}, // F
        };
        std::memcpy(m_memory.data(), font.data(), sizeof(std::array<uint8_t, 5>) * font.size());
}

uint8_t chippy::chip8::at(chip8::Registers reg) {
        return m_registers.at(static_cast<uint8_t>(reg));
}
void chippy::chip8::set(chippy::chip8::Registers reg, uint8_t value) {
        m_registers[static_cast<uint8_t>(reg)] = value;
}

void chippy::chip8::setKeyPressed(uint32_t key_index) {
        pressed_keys |= (1 << key_index);
}
void chippy::chip8::setKeyReleased(uint32_t key_index) {
        pressed_keys &= ~(1 << key_index);
}
bool chippy::chip8::keyIsPressed(uint32_t key_index) const {
        return pressed_keys & (1 << key_index);
}
void chippy::chip8::render(int32_t texture_unit) {
        m_display_texture.set(m_display.data());
        m_display_texture.bind(texture_unit);
        m_vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 1 * 1 * 6);
}
uint16_t chippy::chip8::fetchInstruction() {
        uint16_t instruction = (m_memory[m_program_counter] << 8) | m_memory[m_program_counter + 1];
        return instruction;
}

[[nodiscard]] uint32_t xorshift32(uint32_t &state) noexcept {
        uint32_t x = state;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        return state = x;
}

void chippy::chip8::decodeInstruction(uint16_t instruction) {
        switch (instruction & 0xF000) {
                case 0x0000: {
                        switch (instruction & 0x00FF) {
                                case 0x00E0: { // Clear Screen
                                        memset(m_display.data(), 0, sizeof(m_display));
                                        m_program_counter += 2;
                                        break;
                                }
                                case 0x00EE: { // Ret from subroutine
                                        m_program_counter = m_address_stack[m_stack_pointer];
                                        m_stack_pointer--;
                                        m_program_counter += 2;
                                        break;
                                }
                        }
                        break;
                }
                
                
                case 0x1000: { // Jump to address NNN
                        m_program_counter = instruction & 0x0FFF;
                        break;
                }
                
                
                case 0x2000: { // Call subroutine at NNN
                        m_stack_pointer++;
                        m_address_stack[m_stack_pointer] = m_program_counter;
                        m_program_counter = instruction & 0x0FFF;
                        break;
                }
                
                
                case 0x3000: { // Skip next instruction if value at register x == NN
                        const uint16_t x = (instruction & 0x0F00) >> 8;
                        if (at(static_cast<Registers>(x)) == (instruction & 0x00FF))
                                m_program_counter += 2;
                        m_program_counter += 2;
                        break;
                }
                
                
                case 0x4000: { // Skip next instruction if value at register x != NN
                        const uint16_t x = (instruction & 0x0F00) >> 8;
                        if (at(static_cast<Registers>(x)) != (instruction & 0x00FF))
                                m_program_counter += 2;
                        m_program_counter += 2;
                        break;
                }
                
                
                case 0x5000: { // Skip next instruction of value at register x == value at register y
                        const uint16_t x = (instruction & 0x0F00) >> 8;
                        const uint16_t y = (instruction & 0x00F0) >> 4;
                        if (at(static_cast<Registers>(x)) == at(static_cast<Registers>(y)))
                                m_program_counter += 2;
                        m_program_counter += 2;
                        break;
                }
                
                
                case 0x6000: { // Sets the value of register x to NN
                        const uint16_t x = (instruction & 0x0F00) >> 8;
                        set(static_cast<Registers>(x), (instruction & 0x00FF));
                        m_program_counter += 2;
                        break;
                }
                
                
                case 0x7000: { // Adds NN to the value of register x
                        const uint16_t x = (instruction & 0x0F00) >> 8;
                        set(static_cast<Registers>(x), at(static_cast<Registers>(x)) + (instruction & 0x00FF));
                        m_program_counter += 2;
                        break;
                }
                
                
                case 0x8000: {
                        switch (instruction & 0x000F) {
                                case 0x0000: { // Sets the value at register x to the value at register y
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        const uint16_t y = (instruction & 0x00F0) >> 4;
                                        set(static_cast<Registers>(x), at(static_cast<Registers>(y)));
                                        m_program_counter += 2;
                                        break;
                                }
                                case 0x0001: { // Sets the value at register x to the value at register x | the value at register y
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        const uint16_t y = (instruction & 0x00F0) >> 4;
                                        set(static_cast<Registers>(x), at(static_cast<Registers>(x)) | at(static_cast<Registers>(y)));
                                        m_program_counter += 2;
                                        break;
                                }
                                case 0x0002: { // Sets the value at register x to the value at register x & the value at register y
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        const uint16_t y = (instruction & 0x00F0) >> 4;
                                        set(static_cast<Registers>(x), at(static_cast<Registers>(x)) & at(static_cast<Registers>(y)));
                                        m_program_counter += 2;
                                        break;
                                }
                                case 0x0003: { // Sets the value at register x to the value at register x ^ the value at register y
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        const uint16_t y = (instruction & 0x00F0) >> 4;
                                        set(static_cast<Registers>(x), at(static_cast<Registers>(x)) ^ at(static_cast<Registers>(y)));
                                        m_program_counter += 2;
                                        break;
                                }
                                case 0x0004: { // Adds the value at register y to the value at register x, and sets the value at register f to 1 if there's a carry
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        const uint16_t y = (instruction & 0x00F0) >> 4;
                                        set(Registers::VF, (at(static_cast<Registers>(x)) + at(static_cast<Registers>(y)) > 0xFF));
                                        set(static_cast<Registers>(x), at(static_cast<Registers>(x)) + at(static_cast<Registers>(y)));
                                        m_program_counter += 2;
                                        break;
                                }
                                case 0x0005: { // Subtracts the value at register y from the value at register x, and sets the value at register f to 0 if there's a borrow
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        const uint16_t y = (instruction & 0x00F0) >> 4;
                                        set(Registers::VF, (at(static_cast<Registers>(x)) > at(static_cast<Registers>(y))));
                                        set(static_cast<Registers>(x), at(static_cast<Registers>(x)) - at(static_cast<Registers>(y)));
                                        m_program_counter += 2;
                                        break;
                                }
                                case 0x0006: { // Stores the least significant bit of the value at register x in register f, then shifts the value at register x to the right
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        set(Registers::VF, (at(static_cast<Registers>(x)) & 0x1));
                                        set(static_cast<Registers>(x), at(static_cast<Registers>(x)) >> 1);
                                        m_program_counter += 2;
                                        break;
                                }
                                case 0x0007: { // Sets the value at register x to the value at register y minus the value at register x, and sets the value at register f to 0 if there's a borrow
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        const uint16_t y = (instruction & 0x00F0) >> 4;
                                        set(Registers::VF, (at(static_cast<Registers>(y)) > at(static_cast<Registers>(x))));
                                        set(static_cast<Registers>(x), at(static_cast<Registers>(y)) - at(static_cast<Registers>(x)));
                                        m_program_counter += 2;
                                        break;
                                }
                                case 0x000E: { // Stores the most significant bit of the value at register x in register f, then shifts the value at register x to the left
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        set(Registers::VF, ((at(static_cast<Registers>(x)) >> 7) & 0x1));
                                        set(static_cast<Registers>(x), at(static_cast<Registers>(x)) << 1);
                                        m_program_counter += 2;
                                        break;
                                }
                        }
                        break;
                }
                
                
                case 0x9000: { // Skips the next instruction if the value at register x != the value at register y
                        const uint16_t x = (instruction & 0x0F00) >> 8;
                        const uint16_t y = (instruction & 0x00F0) >> 4;
                        if (at(static_cast<Registers>(x)) != at(static_cast<Registers>(y)))
                                m_program_counter += 2;
                        m_program_counter += 2;
                        break;
                }
                
                
                
                case 0xA000: { // Sets the index register to the address NNN
                        m_index = (instruction & 0x0FFF);
                        m_program_counter += 2;
                        break;
                }
                
                
                case 0xB000: { // Sets the program counter to the address NNN + the value at register 0
                        m_program_counter = (instruction & 0x0FFF) + at(Registers::V0);
                        break;
                }
                
                
                case 0xC000: { // Sets the value at register x to random value | NN
                        const uint16_t x = (instruction & 0x0F00) >> 8;
                        set(static_cast<Registers>(x), (xorshift32(random_value) % 256) & (instruction & 0x00FF));
                        m_program_counter += 2;
                        break;
                }
                
                
                case 0xD000: { // Draws an 8 * (N + 1) pixel sprite located at the index register to the display starting from (value at register x, value at register y)
                        // The value at register f is set to 1 if any pixels are flipped from on to off, and 0 if not.
                        const uint16_t height = instruction & 0x000F;
                        const uint16_t x = (instruction & 0x0F00) >> 8;
                        const uint16_t y = (instruction & 0x00F0) >> 4;
                        
                        const uint16_t offset_x = at(static_cast<Registers>(x));
                        const uint16_t offset_y = at(static_cast<Registers>(y));
                        
                        bool flipped = false;
                        
                        uint16_t pixel;
                        for (uint32_t i = 0; i < height; i++) {
                                pixel = m_memory[m_index + i];
                                for (uint32_t j = 0; j < 8; j++) {
                                        if ((pixel & (0x80 >> j)) != 0) { // If pixel is on
                                                if (m_display[4 * ((offset_x + j + ((offset_y + i) * 64)))] > 0)
                                                        set(Registers::VF, true);
                                                
                                                m_display[4 * (offset_x + j + ((offset_y + i) * 64)) + 0] ^= 0xFF;
                                                m_display[4 * (offset_x + j + ((offset_y + i) * 64)) + 1] ^= 0xFF;
                                                m_display[4 * (offset_x + j + ((offset_y + i) * 64)) + 2] ^= 0xFF;
                                                m_display[4 * (offset_x + j + ((offset_y + i) * 64)) + 3] ^= 0xFF;
                                        }
                                }
                        }
                        
                        m_program_counter += 2;
                        break;
                }
                
                
                case 0xE000: {
                        switch (instruction & 0x00FF) {
                                case 0x009E: { // Skips next instruction if the key stored in the value of register x is pressed
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        if (keyIsPressed(at(static_cast<Registers>(x))))
                                                m_program_counter += 2;
                                        m_program_counter += 2;
                                        break;
                                }
                                case 0x00A1: { // Skips next instruction if the key stored in the value of register x isn't pressed
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        if (!keyIsPressed(at(static_cast<Registers>(x))))
                                                m_program_counter += 2;
                                        m_program_counter += 2;
                                        break;
                                }
                        }
                        break;
                }
                
                
                case 0xF000: {
                        switch (instruction & 0x00FF) {
                                case 0x0007: { // Sets the value of register x to the value of the delay timer
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        set(static_cast<Registers>(x), m_delay_timer);
                                        m_program_counter += 2;
                                        break;
                                }
                                case 0x000A: { // Wait for a keypress and store it in the value of register x
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        
                                        for (uint8_t i = 0; i < 16; i++) {
                                                if (keyIsPressed(i)) {
                                                        set(static_cast<Registers>(x), i);
                                                        m_program_counter += 2;
                                                        break;
                                                }
                                        }
                                        break;
                                }
                                case 0x0015: { // Sets the delay timer to the value stored in register x
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        m_delay_timer = at(static_cast<Registers>(x));
                                        m_program_counter += 2;
                                        break;
                                }
                                case 0x0018: { // Sets the sound timer to the value stored in register x
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        m_sound_timer = at(static_cast<Registers>(x));
                                        m_program_counter += 2;
                                        break;
                                }
                                case 0x001E: { // Increments the index pointer by the value stored in register x
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        m_index += at(static_cast<Registers>(x));
                                        m_program_counter += 2;
                                        break;
                                }
                                case 0x0029: { // Sets the index pointer to the sprite of the glyph stored in register x
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        m_index = at(static_cast<Registers>(x)) * 5;
                                        m_program_counter += 2;
                                        break;
                                }
                                case 0x0033: { // Store the three most significant digits of the decimal representation of the value stored in register x, and store them in three positions starting at the index pointer.
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        const uint8_t value = at(static_cast<Registers>(x));
                                        m_memory[m_index] = (value % 1000) / 100;
                                        m_memory[m_index + 1] = (value % 100) / 10;
                                        m_memory[m_index + 2] = (value % 10);
                                        m_program_counter += 2;
                                        break;
                                }
                                case 0x0055: { // Store the values of registers 0 to x inclusive in memory starting at the index pointer
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        for (uint8_t i = 0; i <= x; i++)
                                                m_memory[m_index + i] = at(static_cast<Registers>(i));
                                        m_program_counter += 2;
                                        break;
                                }
                                case 0x0065: { // Store x bytes of memory starting at the index pointer into registers 0 to x inclusive
                                        const uint16_t x = (instruction & 0x0F00) >> 8;
                                        for (uint8_t i = 0; i <= x; i++)
                                                set(static_cast<Registers>(i), m_memory[m_index + i]);
                                        m_program_counter += 2;
                                        break;
                                }
                        }
                        break;
                }
        }
        if (m_delay_timer > 0) m_delay_timer -= 1;
        if (m_sound_timer > 0) m_sound_timer -= 1;
}
void chippy::chip8::loadProgram(const char *filepath) {
        const std::string a = readFile(filepath);
        memcpy(m_memory.data() + 0x200, a.data(), a.size());
}


