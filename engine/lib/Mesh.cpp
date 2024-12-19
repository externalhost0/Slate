//
// Created by Hayden Rivas on 11/21/24.
//
#include <glad/glad.h>
#include "Slate/Mesh.h"
namespace Slate {
    Mesh::Mesh(const Vertices& vertices, const LayoutBuffer& layout) {
        m_VertexData = vertices;
        m_Layout = layout;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(m_VertexData.m_Size), m_VertexData.m_Data.data(), GL_STATIC_DRAW);

        for (const auto& el : layout)
            AddVertexAttribArray(el);
        glBindVertexArray(0);
    }
    Mesh::Mesh(const Vertices& vertices, const Elements& element, const LayoutBuffer& layout) {
        m_VertexData = vertices;
        m_ElementData = element;
        m_Layout = layout;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(m_VertexData.m_Size), m_VertexData.m_Data.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(m_ElementData.m_Size), m_ElementData.m_Data.data(), GL_STATIC_DRAW);

        for (const auto& el : layout)
            AddVertexAttribArray(el);
        glBindVertexArray(0);
    }

    void Mesh::Bind() const { glBindVertexArray(VAO); }
    void Mesh::Unbind() const { glBindVertexArray(0); }

    void Mesh::DrawMesh() const {
        // draw mesh, accounts if the binded vertex array has indices or not
        if (m_ElementData.m_Data.empty()) {
            glDrawArrays(mode, 0, static_cast<GLsizei>(m_VertexData.m_Data.size() / m_Layout.GetCountPerVertex()));
        } else {
            glDrawElements(mode, static_cast<GLsizei>(m_ElementData.m_Data.size()), GL_UNSIGNED_INT, nullptr);
        }
    }


    // helper func for setupping up vao
    void Mesh::AddVertexAttribArray(const BufferElement& element) {
        switch (element.Type) {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            {
                glEnableVertexAttribArray(m_AttribIndex);
                glVertexAttribPointer(
                        m_AttribIndex,
                        element.GetElementCount(),
                        element.ShaderDataTypeToOpenGLBaseType(),
                        element.Normalized ? GL_TRUE : GL_FALSE,
                        m_Layout.GetStride(),
                        (const void*)element.Offset
                );
                m_AttribIndex++;
                break;
            }
            case ShaderDataType::Bool:
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::UnsignedInt:
            case ShaderDataType::UnsignedInt2:
            case ShaderDataType::UnsignedInt3:
            case ShaderDataType::UnsignedInt4:
            {
                glEnableVertexAttribArray(m_AttribIndex);
                glVertexAttribIPointer(
                        m_AttribIndex,
                        element.GetElementCount(),
                        element.ShaderDataTypeToOpenGLBaseType(),
                        m_Layout.GetStride(),
                        (const void*)element.Offset
                );
                m_AttribIndex++;
                break;
            }
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
            {
                uint8_t count = element.GetElementCount();
                for (uint8_t i = 0; i < count; i++) {
                    glEnableVertexAttribArray(m_AttribIndex);
                    glVertexAttribPointer(
                            m_AttribIndex,
                            count,
                            element.ShaderDataTypeToOpenGLBaseType(),
                            element.Normalized ? GL_TRUE : GL_FALSE,
                            m_Layout.GetStride(),
                            (const void*)(element.Offset + sizeof(float) * count * i)
                    );
                    glVertexAttribDivisor(m_AttribIndex, 1);
                    m_AttribIndex++;
                }
                break;
            }
            default: EXPECT(false, "Unknown ShaderDataType!");
        }
    }

}