#ifndef _TEXT_H_
#define _TEXT_H_

#include <cglm/cglm.h>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    vec2   Size;       // Size of glyph
    vec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

void createTextures();
void initText();
void RenderText(char *text, float x, float y, float scale, vec3 color);

#endif // _TEXT_H_
