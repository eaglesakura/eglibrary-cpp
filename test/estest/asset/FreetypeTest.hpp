#pragma once

#include "estest/eglibrary-test.hpp"
#include "es/util/StringUtil.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "es/system/string/IStringConverter.h"

namespace es {
namespace test {

TEST(FreetypeTest, UtilWideStringConvert) {
    const char *jpn_c = "日本語あいうえお";
    ASSERT_NE(strlen(jpn_c), 8);

    std::wstring wstr = IProcessContext::getInstance()->getStringConverter()->toWideString(std::string(jpn_c));

    int count = 0;
    for (const wchar_t &w : wstr) {
        if (w) {
            ++count;
        }
    }

    eslog("count(%d) len(%d)", wstr.c_str(), count, wcslen(wstr.c_str()));
    ASSERT_TRUE(wstr.length() == 8);
}


#if 0
TEST(FreetypeTest, WstringCheck) {
//    setlocale(LC_CTYPE, "JPN");
    eslog("sizeof(wchar_t) = %d bytes", sizeof(wchar_t));
    const wchar_t *wide = L"ABC";
    ASSERT_EQ(wcslen(wide), 3);

    const wchar_t *jpn = L"日本語あいうえお";
    ASSERT_EQ(wcslen(jpn), 8);

    const char *jpn_c = "日本語あいうえお";
    ASSERT_NE(strlen(jpn_c), 8);

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conversion;
    std::wstring str = conversion.from_bytes(jpn_c);

    ASSERT_TRUE(str.length() == 8);
    ASSERT_TRUE(memcmp(str.c_str(), jpn, sizeof(wchar_t) * str.length()) == 0);
}

TEST(FreetypeTest, LoadFont) {
    FT_Library library = nullptr;
    FT_Face face = nullptr;

    ASSERT_TRUE(FT_Init_FreeType(&library) == 0);
    ASSERT_TRUE(library);

    ASSERT_TRUE(FT_New_Face(library, "shared/test-assets/font/font-jpn.otf", 0, &face) == 0);
    ASSERT_TRUE(face);

    ASSERT_TRUE(FT_Set_Pixel_Sizes(face, 64, 128) == 0);

    ASSERT_TRUE(FT_Load_Char(face, (L"A")[0], 0) == 0);
    ASSERT_TRUE(FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL) == 0);
    eslog("A width(%d) height(%d) left(%d)", face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.pitch);

//    ASSERT_TRUE(FT_Load_Char(face, (L"あ")[0], FT_LOAD_DEFAULT) == 0);
    ASSERT_TRUE(FT_Load_Char(face, 0xE38182, FT_LOAD_DEFAULT) == 0);
//    ASSERT_TRUE(FT_Load_Char(face, 0xE38182, FT_LOAD_DEFAULT) == 0);
    ASSERT_TRUE(FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL) == 0);
    eslog("あ width(%d) height(%d) left(%d)", face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.pitch);

    FT_Done_Face(face);
    FT_Done_FreeType(library);
//    ASSERT_EQ(FT_Set_Char_Size(face, 16, 48, 512, 512), 0);
}
#endif

}
}