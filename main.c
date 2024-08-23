#include "raylib.h"
#include "raymath.h"
#include "stdio.h"
#include "math.h"

    const int screenWidth = 1200;
    const int screenHeight = 800;
#define magazine 200
#define MAX_ASTEROIDS 300
static bool GameOver = false;

typedef struct {
    bool active ;
    Vector2 position;
    int size;
    int rotation;
    float velocity;
    int health;

} Asteroid;


Vector2 SpaceShip = {screenWidth/2,screenHeight/2};
Asteroid _asteroids[MAX_ASTEROIDS] = {0};

typedef struct main
{
    bool active;
    Vector2 position;
}bullet;


static bullet max_fire[magazine] = {0};

int count = 0;
int asteroid_count = 0;
int wait = 0;
int reSpawnTimerAsteroids = 0;
int reSpawnTimerBullets = 0;
float score = 0;
int difficulty = 8; // lower the higher
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------


    InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");

    //Load textures
    Texture2D background = LoadTexture("assets/backgroundmg2.png");
    Texture2D SpaceShipSprite = LoadTexture("assets/spaceship.png");
    Texture2D AsteroidSprite = LoadTexture("assets/asteroid.png");


    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        

         
        
        // Key press detection
        if (IsKeyDown(KEY_RIGHT) & SpaceShip.x < screenWidth-20) SpaceShip.x += 5.0f;
        if (IsKeyDown(KEY_LEFT) & SpaceShip.x > 20) SpaceShip.x -= 5.0f;
        if (IsKeyDown(KEY_UP) & SpaceShip.y > 20) {SpaceShip.y -= 5.0f;}
        if (IsKeyDown(KEY_DOWN) &  SpaceShip.y < screenHeight-20 ) SpaceShip.y += 5.0f;
        if (IsKeyPressed(KEY_SPACE)){
            const Vector2 v = SpaceShip;
            printf("v %f\n",v.x);
            if(count< magazine){
                max_fire[count].position = v;
                max_fire[count].active = true;
                count++;
            }
        };
        printf("%d \n",count);

        for(int i = 0;i<= count; i++){
           max_fire[i].position.y -= 8.0f;          
        }
        for (int i = 0; i < asteroid_count; i++) {
            _asteroids[i].position.y += _asteroids[i].velocity;
        }


        if(wait % difficulty == 0 && asteroid_count < MAX_ASTEROIDS){
            // create asteroid
            Vector2 position ;
            position.y = -15.0f;
            position.x = GetRandomValue(0,screenWidth);

            int bulk = GetRandomValue(2,6);

            _asteroids[asteroid_count] = (Asteroid){
                .active = true,
                .position = position,
                .rotation = GetRandomValue(0,10000000) % 360,
                .velocity = GetRandomValue(4,10),
                .size = bulk,
                .health = (int)bulk/2
            };
            asteroid_count ++;
        }

        // Collision Detection

        // Ship collision
        for(int i = 0; i < asteroid_count; i++){
            // _asteroids[i].size
            Vector2 pos = _asteroids[i].position; 
            float distance = sqrt(pow((SpaceShip.x - pos.x),2) + pow((SpaceShip.y - pos.y),2));

            if(distance < (_asteroids[i].size*10+20) & _asteroids[i].active){
                GameOver = true;
                break;
            }  
        }
        
        // Bullet Collision
        for(int i = 0; i < asteroid_count; i++){
            // _asteroids[i].size
            for(int j = 0; j< count; j++){
            Vector2 bullet = max_fire[j].position;
            Vector2 pos = _asteroids[i].position; 
            float distance = sqrt(pow((bullet.x - pos.x),2) + pow((bullet.y - pos.y),2));

            if(distance < _asteroids[i].size*10 & _asteroids[i].active){
                 max_fire[j].position = (Vector2){0,0};
                _asteroids[i].health --;
                if(_asteroids[i].health == 0) {_asteroids[i].active = false;}
            }  
            }
        }

        //Respawn asteroids
        if(asteroid_count == MAX_ASTEROIDS){
            if(difficulty!=4){
                difficulty -=2;
            }
            reSpawnTimerAsteroids ++;
            if(reSpawnTimerAsteroids >= 300){

            asteroid_count = 0;
            reSpawnTimerAsteroids = 0;
            }
        }

        //Respawn Bullets
        if(count == magazine){
            reSpawnTimerBullets ++;
            if(reSpawnTimerBullets >= 100){

            count = 0;
            reSpawnTimerBullets = 0;
            }
            // asteroid_count ++;
        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);
        // drawing background texture
        background.height = screenHeight;
        background.width = screenWidth;
        DrawTexture(background, 0, 0, WHITE);

        // drawing spaceship sprite
        SpaceShipSprite.width = 80;
        SpaceShipSprite.height = 80;
        DrawTexture(SpaceShipSprite, SpaceShip.x-40, SpaceShip.y-40, WHITE);

         
        //  drawing spaceship bullets
        for(int i = 0;i< count; i++){
            if(max_fire[i].active){

            DrawEllipse(max_fire[i].position.x,max_fire[i].position.y-10.0f,2,10,ORANGE);
            }
        }

        // drawing asteroid
        for(int i = 0; i< asteroid_count; i++){
            Asteroid asteroid = _asteroids[i];
            if(asteroid.active){

            // DrawPoly(asteroid.position,5,10*asteroid.size,6,GRAY);
            AsteroidSprite.width = asteroid.size*16;
            AsteroidSprite.height = asteroid.size*16;
            DrawTexture(AsteroidSprite, asteroid.position.x-(AsteroidSprite.width/2), asteroid.position.y-(AsteroidSprite.height/2), WHITE);
            
            }
        }
        
        
            

        // game over logic
        if(GameOver) {
            int final = (int)score/100;
            DrawRectangle(0,0,screenWidth,screenHeight,RAYWHITE);
            DrawText("GAME OVER!",screenWidth/2-100.0f,screenHeight/2-10.0f,30,RED);
            DrawText("Press Enter to play again",screenWidth/2-100.0f,screenHeight/2+20,20,GRAY);
            DrawText("Spacebar to shoot, arrow keys to move",screenWidth/2-100.0f,screenHeight/2+40,20,GRAY);
            difficulty = 8;
            if(IsKeyDown(KEY_ENTER)){
                GameOver = false;
                count = 0;
                asteroid_count = 0;
                SpaceShip = (Vector2){screenWidth/2,screenHeight/2};
            }
        };

        EndDrawing();
        wait++;
        score += 0.1;
        if(wait >= 600000) wait = 0;

        
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(background);  // Unload background texture
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

