/*
* Engineering Director: Tonia Sanzo
* Art Director:  Bharati Mahajan
* 
* Date: 11/20/2020
* file: LittleAstronaut LTexture.cpp
*/
#include "LTexture.h"


// LTexture constructor
LTexture::LTexture() {
    // Initialize
    mTexture = NULL;
    mRenderer = NULL;
    mFont = NULL;
    mWidth = 0;
    mHeight = 0;
    mScale = 1;
}


// LTexture destructor
LTexture::~LTexture() {
    // Deallocate texture
    free();

    // Free renderer and font if necessary
    if (mRenderer) {
        mRenderer = NULL;
    }
    if (mFont) {
        mFont = NULL;
    }
}


// Load LTexture from a file
bool LTexture::loadFromFile(std::string path) {
    // Get rid of preexisting texture
    free();

    // Exit prematuraly if LTexture has not been properly initialized
    if (!mRenderer) {
        printf("Attempted to render a texture without initializing a renderer!\n");
        return false;
    }

    // The final texture
    SDL_Texture *newTexture = NULL;

    // Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) {
        printf("unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else {
        // Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
        if (!newTexture) {
            printf("Unable to create texture from %s! SDL Error: %s\n:", path.c_str(), SDL_GetError());
        }
        else {
            // Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    // Return success
    mTexture = newTexture;
    return mTexture != NULL;
}


// Load a font texture
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor) {
    // Get rid of preexisting texture
    free();

    // Exit prematuraly if LTexture has not been properly initialized
    if (!mFont) {
        printf("Attempted to render a text texture without initializing a font!\n");
        return false;
    } 
    if (!mRenderer) {
        printf("Attempted to render a texture without initializing a renderer!\n");
        return false;
    }

    // Render text surface
    SDL_Surface *textSurface = TTF_RenderText_Blended(mFont, textureText.c_str(), textColor);
    if (textSurface == NULL) {
        printf("Unable to render text surface!  SDL_ttfError: %s\n", TTF_GetError());
    }
    else {
        // Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
        if (mTexture == NULL) {
            printf("Unable to create texture from rendered text! SDL_Error: %s\n", SDL_GetError());
        }
        else {
            // Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        // Get rid of old surface
        SDL_FreeSurface(textSurface);
    }

    // Return success
    return mTexture != NULL;
}


// Deallocate the texture
void LTexture::free() {
    // Free texture if it exists
    if (mTexture) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
        mScale = 0;
    }
}


// Define whether this texture can blend
void LTexture::setColor(Uint8 r, Uint8 g, Uint8 b) {
    // Modulate texture
    SDL_SetTextureColorMod(mTexture, r, g, b);
}


// Define whether this texture can blend
void LTexture::setBlendMode(SDL_BlendMode blending) {
    // Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}


// Define the texture's opacity
void LTexture::setAlpha(Uint8 alpha) {
    // Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}


// Render texture
void LTexture::render(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip) {
    // Set Rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth * mScale, mHeight * mScale };

    // Set clip rendering dimensions
    if (clip != NULL) {
        renderQuad.w = clip->w * mScale;
        renderQuad.h = clip->h * mScale;
    }

    // Render to screen
    SDL_RenderCopyEx(mRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}


// initialize LTexture with a renderer
void LTexture::initLTexture(SDL_Renderer *rend) {
    mRenderer = rend;
}


// initialize LTexture with a renderer and font
void LTexture::initLTexture(SDL_Renderer *rend, TTF_Font *font) {
    mRenderer = rend;
    mFont = font;
}


int LTexture::getHeight() { return mHeight; }
int LTexture::getWidth() { return mWidth; }
int LTexture::getScale() { return mScale; }

// Set image scale
void LTexture::updateScale(double sc) {
    mScale = sc;
    mWidth *= sc;
    mHeight *= sc;
}