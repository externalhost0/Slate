//
// Created by Hayden Rivas on 11/28/24.
//

#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>
#include <utility>

#include "Expect.h"
#include "Ref.h"

namespace Slate {
    enum class ShaderDataType : unsigned char {
        None = 0,

        Bool,

        Float,
        Float2,
        Float3,
        Float4,

        Int,
        Int2,
        Int3,
        Int4,

        UnsignedInt,
        UnsignedInt2,
        UnsignedInt3,
        UnsignedInt4,

        Mat3,
        Mat4
    };
    static uint16_t ShaderDataTypeSize(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Bool: return 1;

            case ShaderDataType::Float:  return 4;
            case ShaderDataType::Float2: return 4 * 2;
            case ShaderDataType::Float3: return 4 * 3;
            case ShaderDataType::Float4: return 4 * 4;

            case ShaderDataType::Int:  return 4;
            case ShaderDataType::Int2: return 4 * 2;
            case ShaderDataType::Int3: return 4 * 3;
            case ShaderDataType::Int4: return 4 * 4;

            case ShaderDataType::UnsignedInt:  return 4;
            case ShaderDataType::UnsignedInt2: return 4 * 2;
            case ShaderDataType::UnsignedInt3: return 4 * 3;
            case ShaderDataType::UnsignedInt4: return 4 * 4;

            case ShaderDataType::Mat3: return 4 * 3 * 3;
            case ShaderDataType::Mat4: return 4 * 4 * 4;
            default: {
                EXPECT(false, "Unknown ShaderDataType!");
                return 0;
            }
        }
    }


    struct BufferElement {
        std::string Name;
        ShaderDataType Type;
        uint16_t Size;
        std::size_t Offset;
        bool Normalized;

        BufferElement(ShaderDataType type, std::string name, bool normalized = false)
        : Type(type), Name(std::move(name)), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

        uint16_t GetElementCount() const {
            switch (Type) {
                case ShaderDataType::Bool: return 1;

                case ShaderDataType::Float:  return 1;
                case ShaderDataType::Float2: return 2;
                case ShaderDataType::Float3: return 3;
                case ShaderDataType::Float4: return 4;

                case ShaderDataType::Int:  return 1;
                case ShaderDataType::Int2: return 2;
                case ShaderDataType::Int3: return 3;
                case ShaderDataType::Int4: return 4;

                case ShaderDataType::UnsignedInt:  return 1;
                case ShaderDataType::UnsignedInt2: return 2;
                case ShaderDataType::UnsignedInt3: return 3;
                case ShaderDataType::UnsignedInt4: return 4;

                case ShaderDataType::Mat3: return 3 * 3;
                case ShaderDataType::Mat4: return 4 * 4;

                default: {
                    EXPECT(false, "Unknown ShaderDataType!");
                    return 0;
                }
            }
        }
        GLenum ShaderDataTypeToOpenGLBaseType() const {
            switch (Type) {
                case ShaderDataType::Bool: return GL_BOOL;

                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4:
                    return GL_FLOAT;

                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                    return GL_INT;

                case ShaderDataType::UnsignedInt:
                case ShaderDataType::UnsignedInt2:
                case ShaderDataType::UnsignedInt3:
                case ShaderDataType::UnsignedInt4:
                    return GL_UNSIGNED_INT;

                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4:
                    return GL_FLOAT;

                default: {
                    EXPECT(false, "Unknown ShaderDataType!");
                    return 0;
                }
            }
        }
    };

    class LayoutBuffer {
    public:
        LayoutBuffer(const std::initializer_list<BufferElement>& elements)
        : m_Elements(elements) {
            CalculateOffsetAndStride();
        }
        inline unsigned int GetCountPerVertex() const { return m_CountStride; };
        inline uint16_t GetStride() const { return m_Stride; }

        inline const std::vector<BufferElement>& GetElements() const { return m_Elements; };

        // iterator operators
        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); };
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); };
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); };
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); };

    private:
        void CalculateOffsetAndStride() {
            std::size_t offset = 0;
            for (auto& el : m_Elements) {
                el.Offset = offset;
                offset += el.Size;
                m_Stride += el.Size;
                m_CountStride += el.GetElementCount();
            }
        }
        std::vector<BufferElement> m_Elements;
        uint16_t m_Stride{0};
        unsigned int m_CountStride{0};
    };

    class UniformBuffer {
    public:
        UniformBuffer(const void* data, uint32_t size, uint32_t offset) {
            glGenBuffers(1, &UBO_ID);
            glBindBuffer(GL_UNIFORM_BUFFER, UBO_ID);
            glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }
        void Bind() const { glBindBuffer(GL_UNIFORM_BUFFER, UBO_ID ); }
        void Unbind() const { glBindBuffer(GL_UNIFORM_BUFFER, 0); }
    private:
        unsigned int UBO_ID{};
    };

    // simple wrappers for vertices and indices that allows their data to be easily converted and read
    struct Vertices {
        Vertices() = default;
        explicit Vertices(std::vector<float> data)
                : m_Data(std::move(data)), m_Size(data.size() * sizeof (float)) {}

        // removes requirement for passing size
        template <std::size_t N>
        explicit Vertices(const float (&data)[N])
                : m_Data(data, data + N), m_Size(N * sizeof(float)) {}

        Vertices(const float* data, std::size_t size)
                : m_Data(std::vector<float>(data, data + size / sizeof (float))), m_Size(size){}

        std::size_t m_Size{};
        std::vector<float> m_Data;
    };
    struct Elements {
        Elements() = default;
        explicit Elements(std::vector<unsigned int> data)
                : m_Data(std::move(data)), m_Size(data.size() * sizeof (unsigned int)) {}

        // removes requirement for passing size
        template <std::size_t N>
        explicit Elements(const unsigned int (&data)[N])
                : m_Data(data, data + N), m_Size(N * sizeof(unsigned int)) {}

        Elements(const unsigned int* data, std::size_t size)
                : m_Data(std::vector<unsigned int>(data, data + size / sizeof (unsigned int))), m_Size(size){}

        std::size_t m_Size{};
        std::vector<unsigned int> m_Data;
    };
}
