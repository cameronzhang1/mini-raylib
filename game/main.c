#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;

typedef struct Sprite {
    Texture2D texture;
    Rectangle rect;
    Vector2 position;
    int state;
} Sprite;

//------------------------------------------------------------------------------------------
// Global Variables
//------------------------------------------------------------------------------------------

const int screenWidth = 1920;
const int screenHeight = 1080;

static float *delayBuffer = NULL;

// Required variables to manage screen transitions (fade-in, fade-out)
float transAlpha = 0;
bool onTransition = false;
bool transFadeOut = false;
int transFromScreen = -1;
int transToScreen = -1;
int framesCounter = 0;

Music music;
GameScreen currentScreen;
Sprite cursor;
Texture2D background; 

//------------------------------------------------------------------------------------------
// Local Functions Declaration
//------------------------------------------------------------------------------------------

// DrawTransition draws a fade animation
void DrawTransition();

// TransitionToScreen initiates transition process and changes state to trans
void TransitionToScreen(int screen);

// UpdateDrawFrame draws one frame to the window
void UpdateDrawFrame();

// UpdateTransition updates transition process
void UpdateTransition();

//------------------------------------------------------------------------------------------
// Main 
//------------------------------------------------------------------------------------------
int main()
{
    //--------------------------------------------------------------------------------------
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "screenManager");
    InitAudioDevice();

    currentScreen = LOGO;
    cursor = (Sprite) {
      .texture = LoadTexture("assets/cursor.png"),
      .position = {0.0f, 0.0f},
      .rect = {0.0f, 32.0f, 32.0f, 32.0f},
    };

    // TODO: Initialize all required variables and load all required data here!

    music = LoadMusicStream("assets/Kingdom_OST_Rest.mp3");
    background = LoadTexture("assets/8-bit-pixel-art-city.png");

    // Allocate buffer for the delay effect
    delayBuffer = (float *)RL_CALLOC(48000*5, sizeof(float));   // 1 second delay (device sampleRate*channels)

    PlayMusicStream(music);
    SetTargetFPS(60);               // Set desired framerate (frames-per-second)
    HideCursor();

    //--------------------------------------------------------------------------------------
    // Main game loop
    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
    //--------------------------------------------------------------------------------------
    // De-Initialization
    //--------------------------------------------------------------------------------------

    // TODO: Unload all loaded data (textures, fonts, audio) here!
    UnloadMusicStream(music);   // Unload music stream buffers from RAM

    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)

    UnloadTexture(background);
    UnloadTexture(cursor.texture);

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}

void DrawTransition(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}

void TransitionToScreen(int screen)
{
    onTransition = true;
    transFromScreen = currentScreen;
    transToScreen = screen;
}

void UpdateTransition(void)
{
    if (!transFadeOut)
    {
        transAlpha += 0.02f;
        if (transAlpha >= 1.0)
        {
            transAlpha = 1.0;
            currentScreen = transToScreen;
            transFadeOut = true;
            framesCounter = 0;
        }
    }
    else  // Transition fade out logic
    {
        transAlpha -= 0.02f;
        if (transAlpha <= 0)
        {
            transAlpha = 0;
            transFadeOut = false;
            onTransition = false;
            transFromScreen = -1;
            transToScreen = -1;
        }
    }
}

void UpdateDrawFrame() {
        // Update music buffer with new stream data
        UpdateMusicStream(music);

        // Update and fetch cursor data
        cursor.position = (Vector2){ .x = GetMousePosition().x, .y = GetMousePosition().y };
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            cursor.rect.y = 0.0f;
        } else {
            cursor.rect.y = 32.0f;
        }

        switch(currentScreen)
        {
            case LOGO:
            {
                // TODO: Update LOGO screen variables here!
                framesCounter++;    // Count frames

                // Wait for 2 seconds (120 frames) before jumping to TITLE screen
                if (framesCounter > 120)
                {
                    currentScreen = TITLE;
                }
            } break;
            case TITLE:
            {
                // TODO: Update TITLE screen variables here!

                // Press enter to change to GAMEPLAY screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = GAMEPLAY;
                }
            } break;
            case GAMEPLAY:
            {
                // TODO: Update GAMEPLAY screen variables here!

                // Press enter to change to ENDING screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = ENDING;
                }
            } break;
            case ENDING:
            {
                // TODO: Update ENDING screen variables here!

                // Press enter to return to TITLE screen
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
                {
                    currentScreen = TITLE;
                }
            } break;
            default: break;
        }
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);
            switch(currentScreen)
            {
                case LOGO:
                {
                    // TODO: Draw LOGO screen here!
                    DrawTextureRec(cursor.texture,cursor.rect,cursor.position,WHITE);
                    DrawText("LOADING", screenWidth/2 - 88, screenHeight/2, 40, LIGHTGRAY);

                } break;
                case TITLE:
                {
                    // TODO: Draw TITLE screen here!
                    DrawTexture(background, screenWidth/2 - background.width/2, screenHeight/2 - background.height/2, WHITE);
                    DrawText("TITLE SCREEN", 20, 20, 50, WHITE);
                    
                } break;
                case GAMEPLAY:
                {
                    // TODO: Draw GAMEPLAY screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
                    DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
                    DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);

                } break;
                case ENDING:
                {
                    // TODO: Draw ENDING screen here!
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                    DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
                    DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);

                } break;
                default: break;
            }
            
        DrawTextureRec(cursor.texture,cursor.rect,cursor.position,WHITE);
        EndDrawing();
        //----------------------------------------------------------------------------------
    if (!onTransition) {
        // TODO: seperate between transition and non-transition 
    } else {
        UpdateTransition();
    }
    if (onTransition) DrawTransition();
}