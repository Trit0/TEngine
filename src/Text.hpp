#ifndef TEXT_HPP
#define TEXT_HPP

#include <SDL2/SDL_ttf.h>

namespace GEngine {
class Text {
   private:
    TTF_Font *ttfFont;
    double fontWidth;
    double fontHeight;
    unsigned int textureId;
    SDL_Color fontColor;

   public:
    // Needed in engine
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Text(string text, string fontName, int fontSize = 42, SDL_Color fontColor = {255, 255, 255, 255}) {
        GenText(text, fontName, fontSize, fontColor);
    }

    ~Text() {
        TTF_CloseFont(ttfFont);
        glDeleteTextures(1, &textureId);
    }

    void GenText(string text, string fontName, int fontSize, SDL_Color fontColor) {
        ttfFont = TTF_OpenFont(fontName.c_str(), fontSize);
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        this->fontColor = fontColor;
        SDL_Surface *sdlSurface = TTF_RenderText_Blended(ttfFont, text.c_str(), fontColor);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sdlSurface->w, sdlSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sdlSurface->pixels);
        fontWidth = sdlSurface->w;
        fontHeight = sdlSurface->h;
        SDL_FreeSurface(sdlSurface);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    void SetText(string newText) {
        glBindTexture(GL_TEXTURE_2D, textureId);
        SDL_Surface *sdlSurface = TTF_RenderText_Blended(ttfFont, newText.c_str(), fontColor);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sdlSurface->w, sdlSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, sdlSurface->pixels);
        fontWidth = sdlSurface->w;
        fontHeight = sdlSurface->h;
        SDL_FreeSurface(sdlSurface);
    }

    void Draw(double x = 0.0, double y = 0.0) {
        glBindTexture(GL_TEXTURE_2D, textureId);
        glBegin(GL_QUADS);
        glTexCoord2d(0.0, 0.0);
        glVertex3d(x, y, 0.0);
        glTexCoord2d(1.0, 0.0);
        glVertex3d(x + fontWidth, y, 0.0);
        glTexCoord2d(1.0, 1.0);
        glVertex3d(x + fontWidth, y + fontHeight, 0.0);
        glTexCoord2d(0.0, 1.0);
        glVertex3d(x, y + fontHeight, 0.0);
        glEnd();
    }

    double GetFontWidth() {
        return fontWidth;
    }

    double GetFontHeight() {
        return fontHeight;
    }
};
}  // namespace GEngine

#endif