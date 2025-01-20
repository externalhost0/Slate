//
// Created by Hayden Rivas on 11/5/24.
//

#pragma once

#include <vector>
#include <Slate/Expect.h>

namespace Slate {
    enum class FramebufferTextureFormat{
        None = 0,
        RGBA8,
        RGBA16F,
        RED_INTEGER,
        DEPTH24STENCIL8,
    };

    struct FramebufferTextureSpecification {
        FramebufferTextureSpecification() = default;
        FramebufferTextureSpecification(FramebufferTextureFormat format) : TextureFormat(format) {}
        FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
    };


    struct FramebufferAttachmentSpecification {
        FramebufferAttachmentSpecification() = default;
        FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments) : Attachments(attachments) {}

        std::vector<FramebufferTextureSpecification> Attachments;
    };

    struct FramebufferSpecification {
        uint32_t Width = 0, Height = 0;
        int Samples = 1;
        bool SwapChainTarget = false;

        FramebufferAttachmentSpecification Attachments;
    };

    class Framebuffer {
    private:
        uint32_t m_RendererID = 0;
        FramebufferSpecification m_Specification;

        std::vector<uint32_t> m_ColorAttachments;
        std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;

        uint32_t m_DepthAttachment = 0;
        FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;
    public:
        explicit Framebuffer(FramebufferSpecification spec);
        ~Framebuffer();

        void Invalidate();

        void Bind() const;
        void Unbind() const;

        void Resize(uint32_t width, uint32_t height);

        int ReadPixel(uint32_t attachmentIndex, int x, int y);

        void ClearAttachment(uint32_t attachmentIndex, int value);


        uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const { EXPECT(index < m_ColorAttachments.size(), "Color attachment at given index not available!") return m_ColorAttachments[index]; };
        uint32_t GetDepthAttachmentRendererID() const { EXPECT(m_DepthAttachment, "Depth attachment not available!") return m_DepthAttachment; };

        const FramebufferSpecification &GetSpecification() const { return m_Specification; };
    };
}
