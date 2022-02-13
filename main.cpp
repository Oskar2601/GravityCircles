#include <iostream>
#include <string.h>
#include <sstream>
#include <vector>
#include <cmath>
#include <math.h>

#include "include/raylib.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;


class GBall {
public:
   Vector2 gBallPosition;
   Color ballColour = RED;
   float gravity = 200.f;
   float ballSize = 25.0f;
   float pullDistance = 600.f;
   bool locked = false;
   int id = 0;

   void Init();
   void Update();
private:
   Vector2 gBallFrameVelocity = Vector2{0.f, 0.f};
};

std::vector<GBall> gBallArray(0);

void GBall::Init() {
   gBallPosition = Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
   gBallArray.resize(gBallArray.size() + 1);
   this->id = gBallArray.size();
   gBallArray[gBallArray.size() - 1] = *this;
}

float GetDistance(Vector2 startPoint, Vector2 endPoint) {
   return sqrt(pow((endPoint.x - startPoint.x), 2) + pow((endPoint.y - startPoint.y), 2));
}

float Map(float val, float in1, float in2, float out1, float out2)
{
   return out1 + (val - in1) * (out2 - out1) / (in2 - in1);
}

Vector2 GetAngle(Vector2 startAngle, Vector2 endAngle) {
   float radiansAngle = atan2(endAngle.y - startAngle.y, endAngle.x - startAngle.x);
   Vector2 vectorAngle = Vector2{(float)cos(radiansAngle), (float)sin(radiansAngle)};

   return vectorAngle;
}


void GBall::Update() {   
   Vector2 averageAngles = Vector2{0.f, 0.f};
   float deltaTime = GetFrameTime();

   for(unsigned int i = 0; i < gBallArray.size(); i++) {
      float playerDistance = GetDistance(gBallPosition, gBallArray[i].gBallPosition);
      int inverseVelocity = 1;

      if(gBallArray[i].id == id || locked)
         continue;
      
      float pullPower = 0.f;
      Vector2 currentAngle = GetAngle(gBallPosition, gBallArray[i].gBallPosition);

      if(playerDistance - 1.5f < ballSize + gBallArray[i].ballSize) {
         inverseVelocity = playerDistance - ballSize - gBallArray[i].ballSize;
      }

      if(playerDistance <= gBallArray[i].pullDistance) {
         pullPower = Map(playerDistance, 0, gBallArray[i].pullDistance, 1.f, 0.f);
      }

      averageAngles.x += currentAngle.x * gBallArray[i].gravity * pullPower * inverseVelocity * deltaTime;
      averageAngles.y += currentAngle.y * gBallArray[i].gravity * pullPower * inverseVelocity * deltaTime;
   }

   averageAngles.x /= gBallArray.size(); averageAngles.y /= gBallArray.size();

   gBallFrameVelocity = Vector2{averageAngles.x, averageAngles.y};
   gBallPosition = Vector2{gBallPosition.x + (gBallFrameVelocity.x),
                           gBallPosition.y + (gBallFrameVelocity.y)};
   gBallFrameVelocity = Vector2{0.f, 0.f};
}



int main(void)
{
   // Initialization
   //--------------------------------------------------------------------------------------
   const int screenWidth = 1200; // OLD: 800
   const int screenHeight = 750; // OLD: 450
   
   std::stringstream sNum;
   std::stringstream sTime;

   SetConfigFlags(FLAG_MSAA_4X_HINT);
   InitWindow(screenWidth, screenHeight, "GBalls");
   SetTargetFPS(144);


   GBall mainBall;
   mainBall.gBallPosition = Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 4};
   mainBall.ballColour = RED;
   mainBall.Init();

   GBall secondBall;
   secondBall.gBallPosition = Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 4};
   secondBall.ballColour = ORANGE;
   secondBall.Init();
   
   GBall thirdBall;
   thirdBall.gBallPosition = Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 4};
   thirdBall.ballColour = YELLOW;
   thirdBall.Init();
   


   //---------------------------------------------------------------------------------------


   // Main game loop
   while (!WindowShouldClose())    // Detect window close button or ESC key
   {
      sTime.str(std::string());
      sNum.str(std::string());
      sTime << round(GetTime());
      sNum << GetFPS();

      for(unsigned int i = 0; i < gBallArray.size(); i++)
         gBallArray[i].Update();

      if(IsMouseButtonPressed(0))
         gBallArray[0].gBallPosition = Vector2{(float)GetMouseX(), (float)GetMouseY()};
      
      if(IsMouseButtonPressed(1))
         gBallArray[1].gBallPosition = Vector2{(float)GetMouseX(), (float)GetMouseY()};

      //----------------------------------------------------------------------------------
      // Draw
      //----------------------------------------------------------------------------------
      BeginDrawing();

         ClearBackground(Color{0, 0, 9, 0});

         for(unsigned int i = 0; i < gBallArray.size(); i++) {
            DrawCircleV(Vector2{gBallArray[i].gBallPosition.x,
                               gBallArray[i].gBallPosition.y},
                               gBallArray[i].ballSize,
                               gBallArray[i].ballColour);
         }

         DrawText(sNum.str().c_str(), 10, 40, 20, DARKGRAY);
         DrawText(sTime.str().c_str(), 10, 10, 20, DARKGRAY);

      EndDrawing();
      //----------------------------------------------------------------------------------
      // End drawing
      //----------------------------------------------------------------------------------
   }

   // De-Initialization
   //--------------------------------------------------------------------------------------
   CloseWindow();        // Close window and OpenGL context
   //--------------------------------------------------------------------------------------

   return 0;
}