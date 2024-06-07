#define screenWidth 2400 //1560//2400
#define screenHeight 1080 //720//1080

#include <stdio.h>
#include "include/raylib.h"

int main(){
	InitWindow(screenWidth, screenHeight,"");
	SetTargetFPS(60);

	while (!WindowShouldClose()){
		BeginDrawing();
			ClearBackground(BLACK);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
