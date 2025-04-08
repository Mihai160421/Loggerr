#include "Application.h"

int main(int, char**)
{
    //APPLICATION_NAME::Application::getInstance()->Run();
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not init FreeType Library\n";
        return -1;
    }
    FT_Face face;
    if (FT_New_Face(ft, "fonts/Roboto-Medium.ttf", 0, &face)) {
        std::cerr << "Failed to load font\n";
        return -1;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    if (FT_Load_Char(face, 'A', FT_LOAD_RENDER)) {
        std::cerr << "Failed to load Glyph\n";
        return -1;
    }
    
    // Accesează bitmap-ul glyph-ului
    FT_GlyphSlot g = face->glyph;
    std::cout << "Bitmap Width: " << g->bitmap.width << "\n";
    std::cout << "Bitmap Height: " << g->bitmap.rows << "\n";

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return 0;
}