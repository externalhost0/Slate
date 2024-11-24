//
// Created by Hayden Rivas on 11/5/24.
//

#include <GL/glew.h>
#include <iostream>
#include <utility>
#include <glm/vec3.hpp>

#include "Framebuffer.h"

namespace Slate {
    namespace Utils {
        // helper functions
        static GLenum TextureTarget(bool multisampled) { return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D; }

        static bool IsDepthFormat(FramebufferTextureFormat format) {
            switch (format) {
                case FramebufferTextureFormat::DEPTH24STENCIL8:
                    return true;
                default:
                    return false;
            }
        }
        static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index) {
            bool multisampled = samples > 1;
            if (multisampled) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, (GLsizei) width, (GLsizei) height, GL_FALSE);
            }
            else {
                glTexImage2D(GL_TEXTURE_2D, 0, (GLint) internalFormat, (GLsizei) width, (GLsizei) height, 0, format, GL_UNSIGNED_BYTE, nullptr);

                // cause our red int is for selection, must be nearest or won't work
                if (format == GL_RED_INTEGER) {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                } else {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                }

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
        }
        static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {
            bool multisampled = samples > 1;
            if (multisampled)
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, (GLsizei) width, (GLsizei) height, GL_FALSE);
            else {
                glTexStorage2D(GL_TEXTURE_2D, 1, format, (GLsizei) width, (GLsizei) height);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
        }

        static GLenum CustomFBTextureFormatToGL(FramebufferTextureFormat format) {
            switch (format) {
                case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
                case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
                default:
                    EXPECT(false, "A case needs to be implemented in FormatToGL")
                    return 0;
            }
        }
    }


    Framebuffer::Framebuffer(FramebufferSpecification spec) : m_Specification(std::move(spec)) {
        for (auto attachmentSpec : m_Specification.Attachments.Attachments) {
            if (!Utils::IsDepthFormat(attachmentSpec.TextureFormat))
                m_ColorAttachmentSpecifications.emplace_back(attachmentSpec);
            else
                m_DepthAttachmentSpecification = attachmentSpec;
        }
        Invalidate();
    }

    void Framebuffer::Bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, (GLsizei) m_Specification.Width, (GLsizei) m_Specification.Height);
    }

    void Framebuffer::Unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::ClearAttachment(uint32_t attachmentIndex, int value) {
        EXPECT(attachmentIndex < m_ColorAttachments.size(), "Index is out of availble attachment range")
        glClearBufferiv(GL_COLOR, (GLint) attachmentIndex, &value);
    }

    void Framebuffer::Resize(uint32_t width, uint32_t height) {
        m_Specification.Width = width;
        m_Specification.Height = height;
        Invalidate();
    }

    int Framebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) {
        EXPECT(attachmentIndex < m_ColorAttachments.size(), "Index is out of availble attachment range")
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }

    Framebuffer::~Framebuffer() {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures((GLsizei) m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);
    }
    void Framebuffer::Invalidate() {
        if (m_RendererID) {
            // if the rendererID already exists we have to wipe it clean for a new rendererID
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures((GLsizei) m_ColorAttachments.size(), m_ColorAttachments.data());
            glDeleteTextures(1, &m_DepthAttachment);

            m_ColorAttachments.clear();
            m_DepthAttachment = 0;
        }
        glGenFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        bool multisample = m_Specification.Samples > 1;
        if (!m_ColorAttachmentSpecifications.empty()) {
            m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
            glGenTextures((GLsizei) m_ColorAttachments.size(), m_ColorAttachments.data());

            for (size_t i = 0; i < m_ColorAttachments.size(); i++) {
                glBindTexture(Utils::TextureTarget(multisample), m_ColorAttachments[i]);
                switch (m_ColorAttachmentSpecifications[i].TextureFormat) {
                    case FramebufferTextureFormat::RGBA8:
                        Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, (int) i);
                        break;
                    case FramebufferTextureFormat::RED_INTEGER:
                        Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, (int) i);
                        break;
                    default:
                        std::cerr << "Not a supported color format!" << std::endl;
                        break;
                }
            }
        }

        if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None) {
            glGenTextures(1, &m_DepthAttachment);
            glBindTexture(Utils::TextureTarget(multisample), m_DepthAttachment);
            switch (m_DepthAttachmentSpecification.TextureFormat) {
                case FramebufferTextureFormat::DEPTH24STENCIL8:
                    Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
                    break;
                default:
                    std::cerr << "Not a supported depth format!" << std::endl;
                    break;
            }
        }

        if (m_ColorAttachments.size() > 1) {
            EXPECT(m_ColorAttachments.size() <= 4, "Size exceeding max allowed color attachments!")
            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glDrawBuffers((int) m_ColorAttachments.size(), buffers);
        }
        else if (m_ColorAttachments.empty()) {
            // Only depth-pass
            glDrawBuffer(GL_NONE);
        }

        EXPECT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete!")

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}



