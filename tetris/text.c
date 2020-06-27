#include <glad/glad.h>

#include "shader.h"
#include "text.h"

struct Character characters[128];

FT_Library ft;
FT_Face face;
unsigned int VAO2, VBO2, program2;
mat4 projection2;

void createTextures ()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++) {
        // load character glyph
        FT_Error error = FT_Load_Char(face, c, FT_LOAD_RENDER);
        if (error) {
            const char *e_str = FT_Error_String(error);
            printf("ERROR: FT_Load_Char %c %d %s\n", c, error, e_str);
            continue;
        }

        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        struct Character character = {
            texture,
            {face->glyph->bitmap.width, face->glyph->bitmap.rows},
            {face->glyph->bitmap_left, face->glyph->bitmap_top},
            face->glyph->advance.x
        };
        //Characters.insert(std::pair<char, Character>(c, character));
        characters[c] = character;
    }
}

void initText ()
{
    if (FT_Init_FreeType(&ft)) {
        printf("ERROR: FT_Init_FreeType\n");
        exit(EXIT_FAILURE);
    }

    //if (FT_New_Face(ft, "SnesItalic-vmAPZ.ttf", 0, &face)) {
    if (FT_New_Face(ft, "arial.ttf", 0, &face)) {
        printf("ERROR: FT_New_Face\n");
        exit(EXIT_FAILURE);
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    createTextures();

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // shaders
    program2 = createProgram("tetris/text.vert", "tetris/text.frag");

    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm_ortho(0.0f, 800.0f, 0.0f, 600.0f, -1000.0f, 1000.0f, projection2);
}

void RenderText(char *text, float x, float y, float scale, vec3 color)
{
    // activate corresponding render state
    glUseProgram(program2);
    glUniformMatrix4fv(glGetUniformLocation(program2, "projection"), 1, GL_FALSE, (float *) projection2);
    glUniform3fv(glGetUniformLocation(program2, "textColor"), 1, color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO2);

    // iterate through all characters
    for (char *c = text; *c != '\0'; c++)
    {
        struct Character ch = characters[*c];

        float xpos = x + ch.Bearing[0] * scale;
        float ypos = y - (ch.Size[1] - ch.Bearing[1]) * scale;

        float w = ch.Size[0] * scale;
        float h = ch.Size[1] * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f },
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO2);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
