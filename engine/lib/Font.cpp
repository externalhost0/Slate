//
// Created by Hayden Rivas on 12/6/24.
//

#include <glad/glad.h>

#include "Slate/Debug.h"
#include "Slate/Font.h"
#include "Slate/Expect.h"

namespace Slate {
    void FontManager::Initialize() {
        if (FT_Init_FreeType(&m_FTLibrary)) {
            fprintf(stderr, "Could not initialize FreeType Library!\n");
            return;
        }
    }

    void FontManager::Shutdown() {
        // clear all caches fonts and ft lib
        for (const auto& [_, fontFace] : m_FontCache)
            FT_Done_Face(fontFace->GetFace());
        FT_Done_FreeType(m_FTLibrary);
    }

    bool FontManager::Exists(const std::string& name) {
        return m_FontCache.find(name) != m_FontCache.end();
    }
    Ref<Font> FontManager::GetFont(const std::string &name) {
        EXPECT(Exists(name), "Font does not exist!")
        return m_FontCache[name];
    }
    void FontManager::AddFont(const std::filesystem::path& path) {
        AddFont(path.stem().string(), path);
    }
    void FontManager::AddFont(const std::string& name, const std::filesystem::path &path) {
        EXPECT(!Exists(name), "Font already exists!")
        FT_Face face;
        if (FT_New_Face(m_FTLibrary, path.string().c_str(), 0, &face)) {
            fprintf(stderr, "Failed to load font!\n");
            return;
        }
        m_FontCache[name] = CreateRef<Font>(face);
    }

    void Font::SetFontSize() {
        // width = 0 means dynamically calculate it
        FT_Set_Pixel_Sizes(m_FTFace, 0, m_FontSize);
    }
    void Font::BuildGlyph(unsigned char glyph) {
        // load it
        if (FT_Load_Char(m_FTFace, glyph, FT_LOAD_RENDER)) {
            fprintf(stderr, "Failed to load glyph!\n");
            return;
        }
        if (!m_FTFace->glyph->bitmap.buffer) {
            fprintf(stderr, "Warning: Glyph buffer is null for glyph: '%c' (ASCII: %d)\n",
                    isprint(glyph) ? glyph : '?', static_cast<int>(glyph));
            return;
        }
        fprintf(stderr, "Logging: Glyph successfully loaded for glyph: '%c' (ASCII: %d)\n",
                isprint(glyph) ? glyph : '?', static_cast<int>(glyph));

        // construct and insert the lightbulbTexture
        unsigned int texture;
        GL_CALL(glGenTextures(1, &texture));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, texture));
        GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                     static_cast<int>(m_FTFace->glyph->bitmap.width), static_cast<int>(m_FTFace->glyph->bitmap.rows),
                     0, GL_RED, GL_UNSIGNED_BYTE, m_FTFace->glyph->bitmap.buffer
                     ));

        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        m_Characters.insert(std::pair<unsigned char, Character>(glyph, {
            texture,
            glm::ivec2(m_FTFace->glyph->bitmap.width, m_FTFace->glyph->bitmap.rows),
            glm::ivec2(m_FTFace->glyph->bitmap_left, m_FTFace->glyph->bitmap_top),
            m_FTFace->glyph->advance.x
        }));
    }
}