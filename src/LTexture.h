#pragma once
/*
* Engineering Director: Tonia Sanzo
* Art Director:  Bharati Mahajan
* 
* Date: 11/20/2020
* file: LittleAstronaut LTexture.h
*/
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

// Texture wrapper class
class LTexture {
public:
    // Initialize variables
    LTexture();

    // Deallocate memory
    ~LTexture();

    // Loads image at specified path
    bool loadFromFile(std::string path);

    // Creates image from font string
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

    // Deallocates texture
    void free();

    // Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    // Set blending
    void setBlendMode(SDL_BlendMode blending);

    // Set alpha modulation
    void setAlpha(Uint8 alpha);

    // Renders texture at given point
    void render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip = SDL_FLIP_NONE);

    // Initialize LTexture 
    void initLTexture(SDL_Renderer *rend);
    void initLTexture(SDL_Renderer *rend, TTF_Font *font);

    // Gets image dimensions
    int getWidth();
    int getHeight();
    int getScale();

    // Set image scale
    void updateScale(double sc);
private:
    // The actual hardware texture, and the games renderer
    SDL_Texture *mTexture;
    SDL_Renderer *mRenderer;
    TTF_Font *mFont;

    // Image dimensions
    int mWidth, mHeight, mScale;
};