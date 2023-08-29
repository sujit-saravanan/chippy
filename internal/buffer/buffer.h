#pragma once
#include <cstddef>
#include <cstring>
#include "glad.h"

namespace chippy {
        template<GLenum T>
        static inline consteval uint32_t glTypeSize() {
                switch (T) {
                        case GL_BYTE:
                                return sizeof(GLbyte);
                        case GL_UNSIGNED_BYTE:
                                return sizeof(GLubyte);
                        case GL_SHORT:
                                return sizeof(GLshort);
                        case GL_UNSIGNED_SHORT:
                                return sizeof(GLfloat);
                        case GL_INT:
                                return sizeof(GLint);
                        case GL_UNSIGNED_INT:
                                return sizeof(GLuint);
                        case GL_FIXED:
                                return sizeof(GLfixed);
                        case GL_HALF_FLOAT:
                                return sizeof(GLhalf);
                        case GL_FLOAT:
                                return sizeof(GLfloat);
                        case GL_DOUBLE:
                                return sizeof(GLdouble);
                        default:
                                return 0;
                }
        }
        
        class VertexAttributeObject {
        public:  // public constructors/destructors/overloads
                VertexAttributeObject() {
                        glCreateVertexArrays(1, &m_vao);
                }
        public:  // public member functions
                template<GLenum T, size_t count>
                inline void addAttribute() {
                        glEnableVertexArrayAttrib(m_vao, m_attribute_count);
                        glVertexArrayAttribBinding(m_vao, m_attribute_count, 0);
                        glVertexArrayAttribFormat(m_vao, m_attribute_count, count, T, GL_FALSE, m_stride);
                        m_stride += glTypeSize<T>() * count;
                        m_attribute_count++;
                }
                
                inline void vertexBuffer(uint32_t VBO, uint32_t binding_index = 0, uint32_t offset = 0) const {
                        glVertexArrayVertexBuffer(m_vao, binding_index, VBO, offset, m_stride);
                }
                
                inline void indexBuffer(uint32_t IBO) const {
                        glVertexArrayElementBuffer(m_vao, IBO);
                }
                
                inline void bind() const {
                        glBindVertexArray(m_vao);
                }
        public:  // public member variables
        private: // private member functions
        private: // private member variables
                uint32_t m_vao = 0;
                int32_t m_stride = 0;
                uint32_t m_attribute_count = 0;
        };
        
        template<uint32_t Size>
        class HostBuffer {
        public:  // public constructors/destructors/overloads
                HostBuffer();
        public:  // public member functions
        public:  // public member variables
                void write(void *data, uint32_t size, uint32_t buffer_offset = 0, uint32_t source_offset = 0) {
                        glNamedBufferSubData(m_buffer, buffer_offset, size, static_cast<uint8_t *>(data) + source_offset);
                };
                [[nodiscard]] uint32_t buffer() const {
                        return m_buffer;
                }
        private: // private member functions
        private: // private member variables
                uint32_t m_buffer = 0;
        };
        template<uint32_t Size>
        HostBuffer<Size>::HostBuffer() {
                glCreateBuffers(1, &m_buffer);
                glNamedBufferData(m_buffer, Size, nullptr, GL_STATIC_DRAW);
        }
        
        template<uint32_t Size>
        class MappedBuffer {
                static constexpr GLbitfield mapping_flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
                static constexpr GLbitfield storage_flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_CLIENT_STORAGE_BIT;
        public:  // public constructors/destructors/overloads
                MappedBuffer();
        public:  // public member functions
        public:  // public member variables
                void write(void *data, uint32_t size, uint32_t buffer_offset = 0, uint32_t source_offset = 0) {
                        memcpy(static_cast<uint8_t *>(m_data) + buffer_offset, static_cast<uint8_t *>(data) + source_offset, size);
                };
                void push(uint32_t size, uint32_t client_offset = 0, uint32_t host_offset = 0) {
                        glCopyNamedBufferSubData(m_client_staging_buffer, m_host_target_buffer, client_offset, host_offset, size);
                };
                [[nodiscard]] uint32_t host_buffer() const {
                        return m_host_target_buffer;
                }
                [[nodiscard]] uint32_t client_buffer() const {
                        return m_client_staging_buffer;
                }
                [[nodiscard]] void *data() const {
                        return m_data;
                }
        
        private: // private member functions
        private: // private member variables
                uint32_t m_client_staging_buffer = 0;
                uint32_t m_host_target_buffer = 0;
                void *m_data = nullptr;
        };
        template<uint32_t Size>
        MappedBuffer<Size>::MappedBuffer() {
                glCreateBuffers(1, &m_client_staging_buffer);
                glNamedBufferStorage(m_client_staging_buffer, Size, nullptr, storage_flags);
                m_data = glMapNamedBufferRange(m_client_staging_buffer, 0, Size, mapping_flags);
                
                glCreateBuffers(1, &m_host_target_buffer);
                glNamedBufferData(m_host_target_buffer, Size, nullptr, GL_STATIC_DRAW);
        }
}