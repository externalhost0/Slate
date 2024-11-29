//
// Created by Hayden Rivas on 11/21/24.
//
#include <glad/glad.h>
#include <iostream>
#include "Slate/Mesh.h"
namespace Slate {
    void Mesh::DrawMesh() const {
        // draw mesh, accounts if the binded vertex array has indices or not
        glBindVertexArray(VAO);
        if (m_ElementData.m_Data.empty()) {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_VertexData.m_Data.size()));
        } else {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_ElementData.m_Data.size()), GL_UNSIGNED_INT, nullptr);
        }
        glBindVertexArray(0);
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