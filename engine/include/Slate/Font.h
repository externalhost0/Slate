//
// Created by Hayden Rivas on 12/6/24.
//

#ifndef SLATEEDITOR_FONT_H
#define SLATEEDITOR_FONT_H

#include "Ref.h"
#include "IManager.h"

#include <map>
#include <cstdio>
#include <filesystem>
#include <glm/vec2.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Slate {
    // struct to hold individual characters associated, given to Font class
    struct Character {
        unsigned int TextureID;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        long Advance;
    };

    class Font {
    public:
        FT_Face GetFace() const { return this->m_FTFace; }
        void BuildGlyph(unsigned char glyph);
        void SetFontSize();
        explicit Font(FT_Face face)
                : m_FTFace(face) {}
        std::map<unsigned char, Character> m_Characters;
    private:
        FT_Face m_FTFace;
        std::filesystem::path m_FilePath;
        unsigned int m_FontSize{48};

        friend class FontManager;
    };
    class FontManager : public IManager {
    public:
        // we only want ONE FontManager to exist
        FontManager(const FontManager&) = delete;
        FontManager& operator=(const FontManager&) = delete;
        // initalizes freetype when constructing the FontManager
        FontManager() = default;

        // register a new font from file path
        void AddFont(const std::string& name, const std::filesystem::path& path);
        void AddFont(const std::filesystem::path& path);
        // retrieve font from library
        Ref<Font> GetFont(const std::string& name);
    private:
        void Initialize() override;
        void Shutdown() override;
        friend class Application;
    private:
        FT_Library m_FTLibrary{};
        std::unordered_map<std::string, Ref<Font>> m_FontCache;

        // chceks cache for font inclusion
        bool Exists(const std::string &name);
    };
}
#endif //SLATEEDITOR_FONT_H
