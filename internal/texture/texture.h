#pragma once
#include <cstddef>
#include <string_view>

class Texture {
public:  // public constructors/destructors/overloads
        Texture() = default;
        explicit Texture(uint8_t *data, int32_t width, int32_t height, int32_t channels, GLint min_filter = GL_NEAREST_MIPMAP_NEAREST, GLint mag_filter = GL_NEAREST, GLint wrap_s = GL_REPEAT, GLint wrap_t = GL_REPEAT);
        explicit Texture(int32_t width, int32_t height, int32_t channels, GLint min_filter = GL_NEAREST_MIPMAP_NEAREST, GLint mag_filter = GL_NEAREST, GLint wrap_s = GL_REPEAT, GLint wrap_t = GL_REPEAT);
public:  // public member functions
        [[nodiscard]] int32_t width() const;
        [[nodiscard]] int32_t height() const;
        [[nodiscard]] int32_t channels() const;
        [[nodiscard]] uint32_t tex() const;
        
        void set(uint8_t *data, int32_t width, int32_t height, int32_t x_offset = 0, int32_t y_offset = 0) const;
        void set(uint8_t *data) const;
        void bind(int32_t texture_unit) const;
public:  // public member variables
private: // private member functions
private: // private member variables
        int32_t m_width = 0;
        int32_t m_height = 0;
        int32_t m_channels = 0;
        uint32_t m_tex = 0;
};