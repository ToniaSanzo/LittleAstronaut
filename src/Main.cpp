/*
* Copyright 2020 Goblin Games
* 
* Engineering Director: Tonia Sanzo
* Art Director:  Bharati Mahajan
* 
* Date: 11/20/2020
* file: LittleAstronaut Main.cpp
* 
* Special Shoutout to Dark Fantasy Studio for providing their music
*                     Royalty Free through HumbleBundle
* Special Shoutout to Vilkas_Sound for the sound effects licensed under
* Creative Commons 3, https://creativecommons.org/licenses/by/3.0/, the
* sound effects were not edited but converted from a .mp3 file to a .wav
* file
*/
#include "LTexture.h"
#include "LPlanet.h"
#include "PTile.h"
#include "TStar.h"
#include "BMusic.h"
#include <SDL_mixer.h>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

// Screen Dimensions
const int SCREEN_WIDTH = 1900;
const int SCREEN_HEIGHT = 1200;

// The window we will be rendering to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

// The music
Mix_Music *gMusic = NULL;
Mix_Chunk *gClick = NULL;
Mix_Chunk *gPop = NULL;

// The font we will be using
TTF_Font *gFont  = NULL;
TTF_Font *gCFont = NULL;

// The text textures
string fontPath = "assets/default_font.ttf";

// Controls the music
BMusic gMusicBox;

// The background texture's
LTexture gBackgroundTexture;
LTexture gMenuTexture;

// Prints level 2 texture
const int LVL2FRAMES = 9;
LTexture gLvl2Texture;
LTexture gUrsaMajorText;
SDL_Rect gLvl2Clips[LVL2FRAMES];


// Press message texture's
LTexture gPSpaceTexture;
LTexture gCongratsTexture;
LTexture gDirectionTexture;


// The class collections
vector<LPlanet*> planets;
vector<LTexture*> textures;
vector<PTile*> tiles;

LPlanet gSun;
PTile gSunTile;
LTexture gNSunText;

LPlanet gMercury;
PTile gMercuryTile;
LTexture gNMercuryText;

LPlanet gVenus;
PTile gVenusTile;
LTexture gNVenusText;

LPlanet gEarth;
PTile gEarthTile;
LTexture gNEarthText;

LPlanet gMars;
PTile gMarsTile;
LTexture gNMarsText;

LPlanet gJupiter;
PTile gJupiterTile;
LTexture gNJupiterText;

LPlanet gSaturn;
PTile gSaturnTile;
LTexture gNSaturnText;

LPlanet gUranus;
PTile gUranusTile;
LTexture gNUranusText;

LPlanet gNeptune;
PTile gNeptuneTile;
LTexture gNNeptuneText;


// The game states
bool menu();
void dissolveMenu();
bool level1();
bool win();
bool level2();
void dissolveLvl2();
bool finalScreen();


// Initialize the SDL subsystems
bool init() {
    // Initilization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        // Create window
        gWindow = SDL_CreateWindow("Little Astronaut", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!gWindow) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else {
            // Create renderer for the window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else {
                // Set renderer color to onyx
                SDL_SetRenderDrawColor(gRenderer, 53, 56, 57, 255);

                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }

                // Initialize music
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                    printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                    success = false;
                }

                // Initialize fonts
                if (TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s \n", TTF_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}


// Load's the assets into the game
bool loadMedia() {
    // Loading success flag
    bool success = true;
    // Seed rand
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Open the font
    gFont = TTF_OpenFont(fontPath.c_str(), 24);
    if (gFont == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    else {
        gCFont = TTF_OpenFont(fontPath.c_str(), 64);
        if (gCFont == NULL) {
            printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
            success = false;
        }
        else {
            // Load background texture
            gBackgroundTexture.initLTexture(gRenderer);
            textures.push_back(&gBackgroundTexture);
            if (!gBackgroundTexture.loadFromFile("assets/space_bg.png")) {
                printf("Failed to load intro texture!\n");
                success = false;
            }
            else {
                gBackgroundTexture.setBlendMode(SDL_BLENDMODE_BLEND);
                gBackgroundTexture.setAlpha(255);
            }

            // Load menu screen
            gMenuTexture.initLTexture(gRenderer);
            textures.push_back(&gMenuTexture);
            if (!gMenuTexture.loadFromFile("assets/little_intro.png")) {
                printf("Failed to load intro texture!\n");
                success = false;
            }
            else {
                gMenuTexture.setBlendMode(SDL_BLENDMODE_BLEND);
                gMenuTexture.setAlpha(255);
            }

            // Load level 2 texture and clips
            gLvl2Texture.initLTexture(gRenderer);
            textures.push_back(&gLvl2Texture);
            if (!gLvl2Texture.loadFromFile("assets/ursa_major.png")) {
                printf("Failed to load ursa major texture!\n");
                success = false;
            }
            else {
                // Load ursa major clips
                for (int row = 0; row < 3; ++row) {
                    for (int col = 0; col < 3; ++col) {
                        gLvl2Clips[(row * 3) + col].x = col * SCREEN_WIDTH;
                        gLvl2Clips[(row * 3) + col].y = row * SCREEN_HEIGHT;
                        gLvl2Clips[(row * 3) + col].w = SCREEN_WIDTH;
                        gLvl2Clips[(row * 3) + col].h = SCREEN_HEIGHT;
                    }
                }
            }

            // Load press space
            gPSpaceTexture.initLTexture(gRenderer);
            textures.push_back(&gPSpaceTexture);
            if (!gPSpaceTexture.loadFromFile("assets/space.png")) {
                printf("Failed to load intro texture!\n");
                success = false;
            }
            else {
                gPSpaceTexture.setBlendMode(SDL_BLENDMODE_BLEND);
                gPSpaceTexture.setAlpha(255);
            }

            // Load the congratulations texture
            gCongratsTexture.initLTexture(gRenderer, gCFont);
            textures.push_back(&gCongratsTexture);
            if (!gCongratsTexture.loadFromRenderedText("Congratulations!", SDL_Color{0xFF, 0xFF, 0xFF})) {
                printf("Failed to load congratulation text!\n");
                success = false;
            }
            else {
                printf("Successfully loaded congratulations text!\n");
            }

            // Load the direction texture
            gDirectionTexture.initLTexture(gRenderer, gCFont);
            textures.push_back(&gDirectionTexture);
            if (!gDirectionTexture.loadFromRenderedText("Place the planets in correct order.", SDL_Color{ 0xFF, 0xFF, 0xFF })) {
                printf("Failed to load direction text!\n");
                success = false;
            }
            else {
                gDirectionTexture.setBlendMode(SDL_BLENDMODE_BLEND);
                gDirectionTexture.setAlpha(255);
                printf("Successfully loaded direction text!\n");
            }

            // Load the ursa major texture
            gUrsaMajorText.initLTexture(gRenderer, gCFont);
            textures.push_back(&gUrsaMajorText);
            if (!gUrsaMajorText.loadFromRenderedText("Ursa Major.", SDL_Color{ 0xFF, 0xFF, 0xFF })) {
                printf("Failed to load ursa major text!\n");
                success = false;
            }
            else {
                gUrsaMajorText.setBlendMode(SDL_BLENDMODE_BLEND);
                gUrsaMajorText.setAlpha(255);
                printf("Successfully loaded ursa major text!\n");
            }


            // Load the music box
            gMusicBox.initBMusic(gRenderer);
            if (!gMusicBox.loadFromFile("assets/sound_icon.png")) {
                printf("Failed to load music box!\n");
                success = false;
            }
            else {
                printf("Succesfully loaded the music box.\n");
            }

            // Load the sun
            gSun.initLPlanet(gRenderer, "Sun");
            planets.push_back(&gSun);
            if (!gSun.loadFromFile("assets/sun.png")) {
                printf("Failed to load sun planet!\n");
                success = false;
            }
            else {
                printf("Successfully loaded sun.\n");
            }
            gSunTile.initPTile(gRenderer, "Sun", &gNSunText);
            tiles.push_back(&gSunTile);
            if (!gSunTile.loadFromFile("assets/sun_tile.png", "assets/sun_infon.png", "assets/sun_info.png")) {
                printf("Failed to load sun tile!\n");
                success = false;
            }
            else {
                gSunTile.move(222, SCREEN_HEIGHT / 2);
                printf("Successfully loaded sun tile.\n");
            }
            // Load Sun name text
            gNSunText.initLTexture(gRenderer, gFont);
            textures.push_back(&gNSunText);
            SDL_Color textColor = { 251, 226, 110 };
            if (!gNSunText.loadFromRenderedText(gSun.name, textColor)) {
                printf("Failed to load Sun text texture!\n");
                success = false;
            }
            else {
                printf("Successfully loaded Sun text.\n");
            }
            

            // Load mercury
            gMercury.initLPlanet(gRenderer, "Mercury");
            planets.push_back(&gMercury);
            if (!gMercury.loadFromFile("assets/mercury.png")) {
                printf("Failed to load mercury planet!\n");
                success = false;
            }
            else {
                printf("Successfully loaded mercury.\n");
            }
            gMercuryTile.initPTile(gRenderer, "Mercury", &gNMercuryText);
            tiles.push_back(&gMercuryTile);
            if (!gMercuryTile.loadFromFile("assets/mercury_tile.png", "assets/mercury_infon.png", "assets/mercury_info.png")) {
                printf("Failed to load sun tile!\n");
                success = false;
            }
            else {
                gMercuryTile.move(470, SCREEN_HEIGHT / 2);
                printf("Successfully loaded mercury tile.\n");
            }
            // Load Mercury name text
            gNMercuryText.initLTexture(gRenderer, gFont);
            textColor = { 176, 203, 222 };
            textures.push_back(&gNMercuryText);
            if (!gNMercuryText.loadFromRenderedText(gMercury.name, textColor)) {
                printf("Failed to load Mercury text texture!\n");
                success = false;
            }
            else {
                printf("Successfully loaded Mercury text.\n");
            }
            
                
            // Load venus
            gVenus.initLPlanet(gRenderer, "Venus");
            if (!gVenus.loadFromFile("assets/venus.png")) {
                printf("Failed to load venus planet!\n");
                success = false;
            }
            else {
                planets.push_back(&gVenus);
                printf("Successfully loaded venus.\n");
            }
            gVenusTile.initPTile(gRenderer, "Venus", &gNVenusText);
            tiles.push_back(&gVenusTile);
            if (!gVenusTile.loadFromFile("assets/venus_tile.png", "assets/venus_infon.png", "assets/venus_info.png")) {
                printf("Failed to load sun tile!\n");
                success = false;
            }
            else {
                gVenusTile.move(594, SCREEN_HEIGHT / 2);
                printf("Successfully loaded venus tile.\n");
            }
            // Load Venus name text
            gNVenusText.initLTexture(gRenderer, gFont);
            textures.push_back(&gNVenusText);
            textColor = { 247, 222, 191 };
            if (!gNVenusText.loadFromRenderedText(gVenus.name, textColor)) {
                printf("Failed to load Venus text texture!\n");
                success = false;
            }
            else {
                printf("Successfully loaded Venus text.\n");
            }
            

            // Load earth
            gEarth.initLPlanet(gRenderer, "Earth");
            tiles.push_back(&gEarthTile);
            if (!gEarth.loadFromFile("assets/earth.png")) {
                printf("Failed to load earth planet!\n");
                success = false;
            }
            else {
                planets.push_back(&gEarth);
                printf("Successfully loaded earth.\n");
            }
            gEarthTile.initPTile(gRenderer, "Earth", &gNEarthText);
            if (!gEarthTile.loadFromFile("assets/earth_tile.png", "assets/earth_infon.png", "assets/earth_info.png")) {
                printf("Failed to load sun tile!\n");
                success = false;
            }
            else {
                gEarthTile.move(735, SCREEN_HEIGHT / 2);
                printf("Successfully loaded earth tile.\n");
            }
            // Load Earth name text
            gNEarthText.initLTexture(gRenderer, gFont);
            textures.push_back(&gNEarthText);
            textColor = { 46, 176, 68 };
            if (!gNEarthText.loadFromRenderedText(gEarth.name, textColor)) {
                printf("Failed to load Earth text texture!\n");
                success = false;
            }
            else {
                printf("Successfully loaded Earth text.\n");
            }
            

            // Load mars
            gMars.initLPlanet(gRenderer, "Mars");
            planets.push_back(&gMars);
            if (!gMars.loadFromFile("assets/mars.png")) {
                printf("Failed to load mars planet!\n");
                success = false;
            }
            else {
                printf("Successfully loaded mars.\n");
            }
            gMarsTile.initPTile(gRenderer, "Mars", &gNMarsText);
            tiles.push_back(&gMarsTile);
            if (!gMarsTile.loadFromFile("assets/mars_tile.png", "assets/mars_infon.png", "assets/mars_info.png")) {
                printf("Failed to load sun tile!\n");
                success = false;
            }
            else {
                gMarsTile.move(871, SCREEN_HEIGHT / 2);
                printf("Successfully loaded mars tile.\n");
            }
            // Load Mars name text
            gNMarsText.initLTexture(gRenderer, gFont);
            textColor = { 245, 129, 52 };
            textures.push_back(&gNMarsText);
            if (!gNMarsText.loadFromRenderedText(gMars.name, textColor)) {
                printf("Failed to load Mars text texture!\n");
                success = false;
            }
            else {
                printf("Successfully loaded Mars text.\n");
            }
            

            // Load jupiter
            gJupiter.initLPlanet(gRenderer, "Jupiter");
            planets.push_back(&gJupiter);
            if (!gJupiter.loadFromFile("assets/jupiter.png")) {
                printf("Failed to load jupiter planet!\n");
                success = false;
            }
            else {
                printf("Successfully loaded jupiter.\n");
            }
            gJupiterTile.initPTile(gRenderer, "Jupiter", &gNJupiterText);
            tiles.push_back(&gJupiterTile);
            if (!gJupiterTile.loadFromFile("assets/jupiter_tile.png", "assets/jupiter_infon.png", "assets/jupiter_info.png")) {
                printf("Failed to load sun tile!\n");
                success = false;
            }
            else {
                gJupiterTile.move(1060, SCREEN_HEIGHT / 2);
                printf("Successfully loaded jupiter tile.\n");
            }
            // Load Jupiter name text
            gNJupiterText.initLTexture(gRenderer, gFont);
            textures.push_back(&gNJupiterText);
            textColor = { 245, 254, 225 };
            if (!gNJupiterText.loadFromRenderedText(gJupiter.name, textColor)) {
                printf("Failed to load Jupiter text texture!\n");
                success = false;
            }
            else {
                printf("Successfully loaded Jupiter text.\n");
            }
            

            // Load saturn
            gSaturn.initLPlanet(gRenderer, "Saturn");
            planets.push_back(&gSaturn);
            if (!gSaturn.loadFromFile("assets/saturn.png")) {
                printf("Failed to load saturn planet!\n");
                success = false;
            }
            else {
                printf("Successfully loaded saturn.\n");
            }
            gSaturnTile.initPTile(gRenderer, "Saturn", &gNSaturnText);
            tiles.push_back(&gSaturnTile);
            if (!gSaturnTile.loadFromFile("assets/saturn_tile.png", "assets/saturn_infon.png", "assets/saturn_info.png")) {
                printf("Failed to load sun tile!\n");
                success = false;
            }
            else {
                gSaturnTile.move(1339, SCREEN_HEIGHT / 2);
                printf("Successfully loaded saturn tile.\n");
            }
            // Load Saturn name text
            gNSaturnText.initLTexture(gRenderer, gFont);
            textures.push_back(&gNSaturnText);
            textColor = { 213, 193, 186 };
            if (!gNSaturnText.loadFromRenderedText(gSaturn.name, textColor)) {
                printf("Failed to load Saturn text texture!\n");
                success = false;
            }
            else {
                printf("Successfully loaded Saturn text.\n");
            }
            

            // Load uranus
            gUranus.initLPlanet(gRenderer, "Uranus");
            planets.push_back(&gUranus);
            if (!gUranus.loadFromFile("assets/uranus.png")) {
                printf("Failed to load uranus planet!\n");
                success = false;
            }
            else {
                printf("Successfully loaded uranus.\n");
            }
            gUranusTile.initPTile(gRenderer, "Uranus", &gNUranusText);
            tiles.push_back(&gUranusTile);
            if (!gUranusTile.loadFromFile("assets/uranus_tile.png", "assets/uranus_infon.png", "assets/uranus_info.png")) {
                printf("Failed to load sun tile!\n");
                success = false;
            }
            else {
                gUranusTile.move(1568, SCREEN_HEIGHT / 2);
                printf("Successfully loaded uranus tile.\n");
            }
            // Load Uranus name text
            gNUranusText.initLTexture(gRenderer, gFont);
            textures.push_back(&gNUranusText);
            textColor = { 196, 231, 250 };
            if (!gNUranusText.loadFromRenderedText(gUranus.name, textColor)) {
                printf("Failed to load Uranus text texture!\n");
                success = false;
            }
            else {
                printf("Successfully loaded Uranus text.\n");
            }
            

            // Load neptune
            gNeptune.initLPlanet(gRenderer, "Neptune");
            planets.push_back(&gNeptune);
            if (!gNeptune.loadFromFile("assets/neptune.png")) {
                printf("Failed to load neptune planet!\n");
                success = false;
            }
            else {
                printf("Successfully loaded neptune.\n");
            }
            gNeptuneTile.initPTile(gRenderer, "Neptune", &gNNeptuneText);
            tiles.push_back(&gNeptuneTile);
            if (!gNeptuneTile.loadFromFile("assets/neptune_tile.png", "assets/neptune_infon.png", "assets/neptune_info.png")) {
                printf("Failed to load sun tile!\n");
                success = false;
            }
            else {
                gNeptuneTile.move(1749, SCREEN_HEIGHT / 2);
                printf("Successfully loaded neptune tile.\n");
            }
            // Load Neptune name text
            gNNeptuneText.initLTexture(gRenderer, gFont);
            textures.push_back(&gNNeptuneText);
            textColor = { 109, 195, 254 };
            if (!gNNeptuneText.loadFromRenderedText(gNeptune.name, textColor)) {
                printf("Failed to load Neptune text texture!\n");
                success = false;
            }
            else {
                printf("Successfully loaded Neptune text.\n");
            }

            // Load the music
            gMusic = Mix_LoadMUS("assets/music.wav");
            if (gMusic == NULL) {
                printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
                success = false;
            }

            // Load sound effects
            gClick = Mix_LoadWAV("assets/click.wav");
            if (gClick == NULL) {
                printf("Failed to load click sound! SDL_mixer Error: %s\n", Mix_GetError());
                success = false;
            }
            gPop = Mix_LoadWAV("assets/pop.wav");
            if (gPop == NULL) {
                printf("Failed to load pop sound! SDL_mixer Error: %s\n", Mix_GetError());
                success = false;
            }
        }
    }

    return success;
}


// Closes and frees all the resources used to run the game
void close() {
    printf("Cleaning up...");
    // Free textures
    for (LTexture *&texture : textures) {
        texture->free();
        texture = NULL;
    }
    textures.clear();
    
    // Free planets
    for (LPlanet*& planet : planets) {
        planet->free();
        planet = NULL;
    }
    planets.clear();

    // Free tiles
    for (PTile *&tile : tiles) {
        tile->free();
        tile = NULL;
    }
    tiles.clear();

    // Free global fonts
    TTF_CloseFont(gFont);
    gFont = NULL;
    TTF_CloseFont(gCFont);
    gCFont = NULL;

    // Free the sounds
    Mix_FreeMusic(gMusic);
    gMusic = NULL;
    Mix_FreeChunk(gPop);
    Mix_FreeChunk(gClick);
    gPop = NULL;
    gClick = NULL;

    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    printf(" done.\n");
}

 
int main(int argc, char *args[])
{
    // Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        // Load media
        if (!loadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {
            // Main loop flag
            bool quit = false;
            Mix_PlayMusic(gMusic, -1);

            // While application is running
            while (!quit)
            {
                quit = menu();

                // Go to level1 if the user did not quit
                if (!quit) {
                    quit = level1();
                }

                // Go to the win screen if the user aligned all the planets
                if (!quit) {
                    quit = win();
                }

                // Go to level 2 after the user finishes the win screen
                if (!quit) {
                    quit = level2();
                }

                // Go to the final screen
                if (!quit) {
                    quit = finalScreen();
                }
            }
        }
    }

    //Free resources and close SDL
    close();
    return 0;
}

// The menu game state
bool menu() {
    // Reset the planets and tiles
    for (LPlanet *&planet : planets) { planet->reset(); }
    for (PTile *&tile : tiles) { tile->reset(); }

    // Booleans
    bool running = true;

    // Event handler
    SDL_Event e;

    // Used to blink the press space button
    bool decrease = true;
    Uint8 a = 255;

    // While the application is running
    while (running) {
        SDL_Delay(50);
        
        if (decrease) {
            a -= 8;
            if (a >= 248) {
                a = 0;
                decrease = false;
            }
        }
        else {
            a += 8;
            if (a <= 7) {
                a = 255;
                decrease = true;
            }
        }

        // Handle the events on the queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) { 
                return true;
            }
            // On Keypress start the game
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    Mix_PlayChannel(-1, gClick, 0);
                    running = false;
                }
            }

            gMusicBox.handleEvent(&e);
        }

        // Clear screen to onyx
        SDL_SetRenderDrawColor(gRenderer, 53, 56, 57, 255);
        SDL_RenderClear(gRenderer);

        // Render the Menu screen
        gMenuTexture.render(0, 0);
        gPSpaceTexture.setAlpha(a);
        gPSpaceTexture.render(((SCREEN_WIDTH / 2) - (gPSpaceTexture.getWidth() / 2)) - 68,  (2 * SCREEN_HEIGHT / 3) - 32);

        gMusicBox.render();

        // Update screen
        SDL_RenderPresent(gRenderer);
    }


    dissolveMenu();
    return false;
}

void dissolveMenu()
{
    for (Uint8 a = 255; a > 0; a -= 15) {
        SDL_Delay(50);

        // Clear screen, set alpha and render
        SDL_SetRenderDrawColor(gRenderer, 53, 56, 57, 255);
        SDL_RenderClear(gRenderer);

        // Render the directions
        gBackgroundTexture.render(0, 0);
        gDirectionTexture.render(((SCREEN_WIDTH - gDirectionTexture.getWidth()) / 2), ((SCREEN_HEIGHT - gDirectionTexture.getHeight()) / 2));

        //// Render level 1
        //gBackgroundTexture.render(0, 0);
        //for (PTile *tile : tiles) {
        //    tile->render();
        //}
        //for (LPlanet *planet : planets) {
        //    planet->render();
        //}
        
        // Render the menu
        gMenuTexture.setAlpha(a);
        gMenuTexture.render(0, 0);

        // Update screen
        SDL_RenderPresent(gRenderer);
    }

    // Handle's the events
    bool running = true;
    // Event handler
    SDL_Event e;

    while (running) {
        // Handle the events on the queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                running = false;
            }
            // On Keypress start the game
            else if (e.type == SDL_KEYDOWN) {
                running = false;
            }
        }

        // Clear screen to onyx
        SDL_SetRenderDrawColor(gRenderer, 53, 56, 57, 255);
        SDL_RenderClear(gRenderer);

        // Render the directions
        gBackgroundTexture.render(0, 0);
        gDirectionTexture.render(((SCREEN_WIDTH - gDirectionTexture.getWidth()) / 2), ((SCREEN_HEIGHT - gDirectionTexture.getHeight()) / 2));

        // Update screen
        SDL_RenderPresent(gRenderer);
    }

    for (Uint8 a = 255; a > 0; a -= 15) {
        SDL_Delay(50);

        // Clear screen, set alpha and render
        SDL_SetRenderDrawColor(gRenderer, 53, 56, 57, 255);
        SDL_RenderClear(gRenderer);

        

        // Render level 1
        gBackgroundTexture.setAlpha(255);
        gBackgroundTexture.render(0, 0);
        gBackgroundTexture.render(0, 0);
        for (PTile *tile : tiles) {
            tile->render();
        }
        for (LPlanet *planet : planets) {
            planet->render();
        }

        // Render the fading directiosn
        gBackgroundTexture.setAlpha(a);
        gBackgroundTexture.render(0, 0);
        gDirectionTexture.setAlpha(a);
        gDirectionTexture.render(((SCREEN_WIDTH - gDirectionTexture.getWidth()) / 2), ((SCREEN_HEIGHT - gDirectionTexture.getHeight()) / 2));
        // Update screen
        SDL_RenderPresent(gRenderer);
    }
    

    // Reset the menu alpha values
    gMenuTexture.setAlpha(255);
    gDirectionTexture.setAlpha(255);
    gBackgroundTexture.setAlpha(255);
}


bool level1() {
    // Booleans
    bool running = true;
    int constexpr PLANET_COUNT = 9;
    vector<bool> tileClicked;
    for (int i = 0; i < PLANET_COUNT; ++i) { tileClicked.push_back(false); }

    // Event handler
    SDL_Event e;
    Uint8 aCnt = 0;

    // While the application is running
    while (running) {
        // Handle the events on the queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                return true;
            }
            // On Keypress start the game
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    return true;
                }
            }
            else {
                // Play the sound effect on clicks
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    Mix_PlayChannel(-1, gClick, 0);
                }

                bool pickedUp = false;
                // Handle planet events
                for (LPlanet *planet : planets) {
                    // If a planet was picked up
                    pickedUp = planet->handleEvent(&e);
                    if (pickedUp) { break; }
                }

                // Handle tile events
                for (int i = 0; i < PLANET_COUNT; ++i) {
                    tileClicked[i] = tiles[i]->handleEvent(&e);
                }

                // If the user released the mouse, check to see if a planet was dropped on a tile 
                if (e.type == SDL_MOUSEBUTTONUP) {
                    
                    for (PTile *tile : tiles) {
                        for (LPlanet *planet : planets) {
                            if (planet->aligned) { continue; }

                            printf("checking %s planet and %s tile.\n", planet->name.c_str(), tile->name.c_str());
                            if (tile->aligned(*planet)) {
                                ++aCnt;
                                Mix_PlayChannel(-1, gPop, 0);
                                printf("%s planet and %s tile aligned!\n", planet->name.c_str(), tile->name.c_str());
                            }
                        }
                    }
                }
            }
        }

        // Clear screen to onyx
        SDL_SetRenderDrawColor(gRenderer, 53, 56, 57, 255);
        SDL_RenderClear(gRenderer);

        gBackgroundTexture.render(0, 0);

        // Render level 1
        for (PTile *tile : tiles) {
            tile->render();
        }
        for (LPlanet *planet : planets) {
            planet->render();
        }
        
        for (int i = 0; i < PLANET_COUNT; ++i) {
            if (tileClicked[i]) {
                tiles[i]->renderInfo();
            }
        }

        // Update screen
        SDL_RenderPresent(gRenderer);
        if (aCnt == 9) {
            printf("Congrats all aligned!.");
            running = false;
        }
    }
    return false;
}


bool win() {
    // Booleans
    bool running = true;
    int constexpr PLANET_COUNT = 9;
    vector<bool> tileClicked;
    for (int i = 0; i < PLANET_COUNT; ++i) { tileClicked.push_back(false); }

    // Event handler
    SDL_Event e;

    // While the application is running
    while (running) {
        // Handle the events on the queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                return true;
            }
            // On Keypress start the game
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    running = false;
                }
                else if(e.type == SDLK_ESCAPE) {
                    return true;
                }
            }
            else {
                // Play the sound effects
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    Mix_PlayChannel(-1, gClick, 0);
                }

                // Handle tile events
                for (int i = 0; i < PLANET_COUNT; ++i) {
                    tileClicked[i] = tiles[i]->handleEvent(&e);
                }
            }
            
        }

        // Clear screen to onyx
        SDL_SetRenderDrawColor(gRenderer, 53, 56, 57, 255);
        SDL_RenderClear(gRenderer);

        gBackgroundTexture.render(0, 0);
        // Render level 1
        for (PTile *tile : tiles) {
            tile->render();
        }
        for (LPlanet *planet : planets) {
            planet->render();
        }

        for (int i = 0; i < PLANET_COUNT; ++i) {
            if (tileClicked[i]) {
                tiles[i]->renderInfo();
            }
        }

        // Render Congratulation!
        gCongratsTexture.render((SCREEN_WIDTH / 2) - (gCongratsTexture.getWidth() / 2), 0);

        // Update screen
        SDL_RenderPresent(gRenderer);
    }
    return false;
}

bool level2() {
    // Booleans
    bool running = true;
    bool quit = false;

    // Event handler
    SDL_Event e;

    vector<TStar> stars;

    // star 0
    stars.push_back(TStar(0, 308, 503, 39, 37));

    // star 1
    stars.push_back(TStar(1, 538, 404, 39, 37));

    // star 2
    stars.push_back(TStar(2, 697, 447, 39, 37));

    // star 3
    stars.push_back(TStar(3, 890, 491, 39, 37));

    // star 4
    stars.push_back(TStar(4, 1257, 389, 39, 37));

    // star 5
    stars.push_back(TStar(5, 1250, 577, 39, 37));

    // star 6
    stars.push_back(TStar(6, 975, 635, 39, 37));

    vector<TStar>::size_type currentStar = 0;
    Uint8 nextStarState = TStar::STAR_OUT_OF_RANGE, currStarState = TStar::STAR_OUT_OF_RANGE;

    // While the application is running
    while (running) {

        // Handle the events on the queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                running = false;
                quit = true;
            }
            else {
                // handle mouse events
                nextStarState = stars[currentStar + 1].handleEvent(&e);
                if ((nextStarState == TStar::STAR_RELEASED) && (currStarState == TStar::STAR_CLICKED)) {
                    Mix_PlayChannel(-1, gPop, 0);
                    ++currentStar;
                }
                currStarState = stars[currentStar].handleEvent(&e);

                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    Mix_PlayChannel(-1, gPop, 0);
                }
            }
        }

        // Clear screen to onyx
        SDL_SetRenderDrawColor(gRenderer, 53, 56, 57, 255);
        SDL_RenderClear(gRenderer);

        // Render the Menu screen
        gLvl2Texture.render(0, 0, &gLvl2Clips[currentStar]);

        // Update screen
        SDL_RenderPresent(gRenderer);
        if (currentStar == 6) {
            running = false;
        }
    }

    if (!quit) {
        dissolveLvl2();
    }
    return quit;
}


void dissolveLvl2() {
    for (Uint8 a = 255; a > 0; a -= 15) {
        SDL_Delay(150);

        // Clear screen, set alpha and render
        SDL_SetRenderDrawColor(gRenderer, 53, 56, 57, 255);
        SDL_RenderClear(gRenderer);

        // Render the directions
        gLvl2Texture.setAlpha(255);
        gLvl2Texture.render(0, 0, &gLvl2Clips[8]);

        //// Render level 1
        //gBackgroundTexture.render(0, 0);
        //for (PTile *tile : tiles) {
        //    tile->render();
        //}
        //for (LPlanet *planet : planets) {
        //    planet->render();
        //}

        // Render the final screen
        gLvl2Texture.setAlpha(a);
        gLvl2Texture.render(0, 0, &gLvl2Clips[7]);
        gUrsaMajorText.setAlpha(255 - a);
        gUrsaMajorText.render((SCREEN_WIDTH - gUrsaMajorText.getWidth()) / 2 , SCREEN_HEIGHT - gUrsaMajorText.getHeight());

        // Update screen
        SDL_RenderPresent(gRenderer);
    }

    gLvl2Texture.setAlpha(255);
    gUrsaMajorText.setAlpha(255);
}


bool finalScreen() {
    // Booleans
    bool running = true;
    bool quit = false;

    // Event handler
    SDL_Event e;

    // While the application is running
    while (running) {

        // Handle the events on the queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                running = false;
                quit = true;
            }
            // On Keypress start the game
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    Mix_PlayChannel(-1, gClick, 0);
                    running = false;
                }
            }
        }

        // Clear screen to onyx
        SDL_SetRenderDrawColor(gRenderer, 53, 56, 57, 255);
        SDL_RenderClear(gRenderer);

        // Render the background
        gLvl2Texture.render(0, 0, &gLvl2Clips[8]);
        gUrsaMajorText.render((SCREEN_WIDTH - gUrsaMajorText.getWidth()) / 2, SCREEN_HEIGHT - gUrsaMajorText.getHeight());

        // Update screen
        SDL_RenderPresent(gRenderer);
    }

    return quit;
}