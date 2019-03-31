/*******************************************************************************************
*
*   raylib [audio] example - Music playing (streaming)
*
*   NOTE: This example requires OpenAL Soft library installed
*
*   This example has been created using raylib 1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "list.h"

#define DARKRED     CLITERAL{ 117, 14, 14, 255 }
#define LIGHTGRAY2  CLITERAL{ 168, 168, 168, 255 }

int main()
{
    // Initialization
    //-------------------------------------------------------------------------------------------------------------------
    int screenWidth = 450;
    int screenHeight = 180;


    InitWindow(screenWidth, screenHeight, "Linked List Music Player");

    InitAudioDevice();                                   // Initialize audio device
    
    Camera3D camera;
    camera.position = (Vector3){ 0.01f, 10.0f, 25.0f };  // Camera position
    camera.target = (Vector3){ 0.01f, -8.0f, 0.0f };     // Camera looking at point
    camera.up = (Vector3){ 0.0f, -1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                 // Camera field-of-view Y
    camera.type = CAMERA_PERSPECTIVE;                    // Camera mode type

    Music music[8] = {
        LoadMusicStream("xystus.ogg"),
        LoadMusicStream("seventh_wonder.ogg"),
        LoadMusicStream("serpentine.ogg"),
        LoadMusicStream("martyrs_soen.ogg"),
        LoadMusicStream("labyrinth.ogg"),
        LoadMusicStream("dream_theater.ogg"),
        LoadMusicStream("darkwater.ogg"),
        LoadMusicStream("circus_maximus.ogg")
    };
    
    char titles[8][64] = {
        "Circus Maximus - Isolate",
        "Darkwater - The Play II",
        "Dream Theater - Yste Jam",
        "Libyrinth - L.Y.A.F.H.",
        "Soen - Martyrs",
        "Serpentine - Whatever Heartache",
        "Seventh Wonder - Tears for a Father",
        "Xystus - End of the Line"
    };
    
    List *list = new_list( &UnloadMusicStream );
    for (int i = 0; i < 8; i++) {
        list_insert_next(list, NULL, music[i]);
    }
    Node *curr_node = list_head(list);
    Music current = (Music)list_data(curr_node);
    int count = 0;
    
    PlayMusicStream(current);

    float timePlayed = 0.0f;
    bool pause = false;
    
    
    Image images[4] = {                                  // Initialize button images/textures
        LoadImage("play.png"),
        LoadImage("pause.png"),
        LoadImage("back.png"),
        LoadImage("forward.png")
    };
    
    for (int i = 0; i < 4; i++) {
        ImageResize(&images[i], 100, 100);
    }
    
    Texture textures[4] = {
        LoadTextureFromImage(images[0]),
        LoadTextureFromImage(images[1]),
        LoadTextureFromImage(images[2]),
        LoadTextureFromImage(images[3])
    };
    
    for (int i = 0; i < 4; i++) {
        UnloadImage(images[i]);
    }


    
    int buttonXOff = 25;                                 // Button position offset
    int buttonYOff = 10;
    
    // Collision rectangle for the forward button
    Rectangle forward_rect;
    forward_rect.x = 250 + buttonXOff + (float) buttonXOff;
    forward_rect.y = buttonYOff + (float) buttonXOff;
    forward_rect.width = 50.f;
    forward_rect.height = 50.f;

    // Collision circle for play/pause button
    Vector2 play_pause_vec;
    play_pause_vec.x = 175 + buttonXOff + (float) buttonXOff;
    play_pause_vec.y = 25 + buttonYOff + (float) buttonXOff;
    
    // Collision rectangle for restart button
    Rectangle back_rect;
    back_rect.x = 50 + buttonXOff + (float) buttonXOff;
    back_rect.y = buttonYOff + (float) buttonXOff;
    back_rect.width = 50.f;
    back_rect.height = 50.f;

    // 3D Vector for song timeline
    Vector3 cube_center = {0, 0, 0};
    
    SetTargetFPS(60);                                    // Set our game to run at 60 frames-per-second
    // Main game loop
    //-------------------------------------------------------------------------------------------------------------------
    while (!WindowShouldClose())                         // Detect window close button or ESC key
    {
        // Update
        //---------------------------------------------------------------------------------------------------------------
        UpdateMusicStream(current);                      // Update music buffer with new stream data
        
        // Restart music playing (stop and play)
        if (IsKeyPressed(KEY_SPACE) || (CheckCollisionPointCircle(GetMousePosition(), play_pause_vec, 50) && IsMouseButtonPressed(0))) 
        {
            pause = !pause;
            
            if (pause) PauseMusicStream(current);
            else ResumeMusicStream(current);
        }
        
        SetWindowTitle(titles[count]);
        // Get timePlayed scaled to bar dimensions (400 pixels)
        timePlayed = GetMusicTimePlayed(current)/GetMusicTimeLength(current)*400;
        
        // check the forward button to skip
        if ((CheckCollisionPointRec(GetMousePosition(), forward_rect) && IsMouseButtonPressed(0))
            || (GetMusicTimeLength(current) <= (GetMusicTimePlayed(current) + 1))) {
            if (curr_node->next == NULL) {
                curr_node = list_head(list);
                count = 0;
            } else {
                curr_node = list_next(curr_node);
                count++;
            }
            
            current = (Music)list_data(curr_node);
            StopMusicStream(current);
            PlayMusicStream(current);
           
        }
        // Check the back button to restart the song
        if (CheckCollisionPointRec(GetMousePosition(), back_rect) && IsMouseButtonPressed(0)) {
            StopMusicStream(current);
            PlayMusicStream(current);
        }
        //---------------------------------------------------------------------------------------------------------------

        // Draw
        //---------------------------------------------------------------------------------------------------------------
        BeginDrawing();
        
        ClearBackground(LIGHTGRAY2);
        
        BeginMode3D(camera);
            DrawCube(cube_center, 1, 2, 2, DARKRED);
            //DrawCubeWires(cube_center, 1.05f, 2.05f, 2.05f, RED);
            DrawCube(cube_center, (float)timePlayed / 8.f, 1, 1, DARKGRAY);
            DrawCubeWires(cube_center, (float)timePlayed / 8.f + 0.5, 1.5f, 1.5f, DARKRED);
        EndMode3D();

        if (pause)
            DrawTexture(textures[0], 150 + buttonXOff, buttonYOff, GRAY);
        else
            DrawTexture(textures[1], 150 + buttonXOff, buttonYOff, WHITE);
        DrawTexture(textures[2], 50 + buttonXOff, buttonYOff, WHITE);
        DrawTexture(textures[3], 250 + buttonXOff, buttonYOff, WHITE);
        
        EndDrawing();
        //---------------------------------------------------------------------------------------------------------------
    }

    // De-Initialization
    //-------------------------------------------------------------------------------------------------------------------
    destroy_list(list);

    for (int i = 0; i < 4; i++) {
        UnloadTexture(textures[i]);
    }
    
    CloseAudioDevice();                                  // Close audio device (music streaming is automatically stopped)

    CloseWindow();                                       // Close window and OpenGL context
    //-------------------------------------------------------------------------------------------------------------------

    return 0;
}
