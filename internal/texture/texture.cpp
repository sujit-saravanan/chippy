#include "texture.h"
#include "glad.h"

Texture::Texture(uint8_t *data, int32_t width, int32_t height, int32_t channels, GLint min_filter, GLint mag_filter, GLint wrap_s, GLint wrap_t) : m_width(width), m_height(height), m_channels(channels) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_tex);
        
        glTextureParameteri(m_tex, GL_TEXTURE_MIN_FILTER, min_filter);
        glTextureParameteri(m_tex, GL_TEXTURE_MAG_FILTER, mag_filter);
        glTextureParameteri(m_tex, GL_TEXTURE_WRAP_S, wrap_s);
        glTextureParameteri(m_tex, GL_TEXTURE_WRAP_T, wrap_t);
        
        glTextureStorage2D(m_tex, 1, GL_RGBA8, m_width, m_height);
        glTextureSubImage2D(m_tex, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(m_tex);
}
Texture::Texture(int32_t width, int32_t height, int32_t channels, GLint min_filter, GLint mag_filter, GLint wrap_s, GLint wrap_t) : m_width(width), m_height(height), m_channels(channels){
        glCreateTextures(GL_TEXTURE_2D, 1, &m_tex);
        
        glTextureParameteri(m_tex, GL_TEXTURE_MIN_FILTER, min_filter);
        glTextureParameteri(m_tex, GL_TEXTURE_MAG_FILTER, mag_filter);
        glTextureParameteri(m_tex, GL_TEXTURE_WRAP_S, wrap_s);
        glTextureParameteri(m_tex, GL_TEXTURE_WRAP_T, wrap_t);
        
        glTextureStorage2D(m_tex, 1, GL_RGBA8, m_width, m_height);
        glGenerateTextureMipmap(m_tex);
}
int32_t Texture::width() const {
        return m_width;
}
int32_t Texture::height() const {
        return m_height;
}
int32_t Texture::channels() const {
        return m_channels;
}
uint32_t Texture::tex() const {
        return m_tex;
}
void Texture::bind(int32_t texture_unit) const {
        glBindTextureUnit(texture_unit, m_tex);
}
void Texture::set(uint8_t *data, int32_t width, int32_t height, int32_t x_offset, int32_t y_offset) const {
        glTextureSubImage2D(m_tex, 0, x_offset, y_offset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}
void Texture::set(uint8_t *data) const {
        glTextureSubImage2D(m_tex, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}
