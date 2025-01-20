//
// Created by Hayden Rivas on 11/5/24.
//

#include <glad/glad.h>

#include <cstdio>
#include <utility>
#include <glm/vec3.hpp>

#include <Slate/Debug.h>

#include "Framebuffer.h"
#include <iostream>

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
                GL_CALL(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, false));
            }
            else {
                GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr));

                // cause our red int is for selection, must be nearest or won't work
                if (format == GL_RED_INTEGER) {
                    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
                    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
                } else {
                    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
                    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                }

                GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
                GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
                GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            }
            GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0));
        }
        static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {
            bool multisampled = samples > 1;
            if (multisampled) {
                GL_CALL(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height,GL_TRUE));
            }
            else {
                GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));

                GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
                GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
                GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
                GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
                GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            }
            GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0));
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
        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
        GL_CALL(glViewport(0, 0, static_cast<GLsizei>(m_Specification.Width), static_cast<GLsizei>(m_Specification.Height)));
    }

    void Framebuffer::Unbind() const {
        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void Framebuffer::ClearAttachment(uint32_t attachmentIndex, int value) {
        EXPECT(attachmentIndex < m_ColorAttachments.size(), "Index is out of availble attachment range")
        GL_CALL(glClearBufferiv(GL_COLOR, static_cast<GLint>(attachmentIndex), &value));
    }

    void Framebuffer::Resize(uint32_t width, uint32_t height) {
        m_Specification.Width = width;
        m_Specification.Height = height;
        Invalidate();
    }

    int Framebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) {
        EXPECT(attachmentIndex < m_ColorAttachments.size(), "Index is out of availble attachment range")
        GL_CALL(glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex));
        int pixelData;
        GL_CALL(glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData));
        return pixelData;
    }

    Framebuffer::~Framebuffer() {
        GL_CALL(glDeleteFramebuffers(1, &m_RendererID));
        GL_CALL(glDeleteTextures(static_cast<GLsizei>(m_ColorAttachments.size()), m_ColorAttachments.data()));
        GL_CALL(glDeleteTextures(1, &m_DepthAttachment));
    }
    void Framebuffer::Invalidate() {
        if (m_RendererID) {
            // if the rendererID already exists we have to wipe it clean for a new rendererID
            GL_CALL(glDeleteFramebuffers(1, &m_RendererID));
            GL_CALL(glDeleteTextures(static_cast<GLsizei>(m_ColorAttachments.size()), m_ColorAttachments.data()));
            GL_CALL(glDeleteTextures(1, &m_DepthAttachment));

            m_ColorAttachments.clear();
            m_DepthAttachment = 0;
        }
        GL_CALL(glGenFramebuffers(1, &m_RendererID));
        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

        bool multisample = m_Specification.Samples > 1;
        if (!m_ColorAttachmentSpecifications.empty()) {
            m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
            GL_CALL(glGenTextures(static_cast<GLsizei>(m_ColorAttachments.size()), m_ColorAttachments.data()));

            for (int i = 0; i < m_ColorAttachments.size(); i++) {
                GL_CALL(glBindTexture(Utils::TextureTarget(multisample), m_ColorAttachments[i]));
                switch (m_ColorAttachmentSpecifications[i].TextureFormat) {
                    case FramebufferTextureFormat::RGBA8:
                        Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
                        break;
                    case FramebufferTextureFormat::RED_INTEGER:
                        Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
                        break;
                    case FramebufferTextureFormat::RGBA16F:
                        Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA16F, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
                        break;
                    default:
                        fprintf(stderr, "Not a supported color format!\n");
                        break;
                }
            }
        }

        if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None) {
            GL_CALL(glGenTextures(1, &m_DepthAttachment));
            GL_CALL(glBindTexture(Utils::TextureTarget(multisample), m_DepthAttachment));
            switch (m_DepthAttachmentSpecification.TextureFormat) {
                case FramebufferTextureFormat::DEPTH24STENCIL8:
                    Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
                    break;
                default:
                    fprintf(stderr, "Not a supported depth format!\n");
                    break;
            }
        }

        if (m_ColorAttachments.size() > 1) {
            EXPECT(m_ColorAttachments.size() <= 4, "Size exceeding max allowed color attachments!")
            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            GL_CALL(glDrawBuffers(static_cast<int>(m_ColorAttachments.size()), buffers));
        }
        else if (m_ColorAttachments.empty()) {
            // Only depth-pass
            GL_CALL(glDrawBuffer(GL_NONE));
        }

        EXPECT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete!")

        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }
}



