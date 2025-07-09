#include "raylib.h"
//#include <math.h>
//#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int SCREEN_WIDTH  = 960;
int SCREEN_HEIGHT = 540;

const int RENDER_WIDTH = 960;
const int RENDER_HEIGHT = 540;

typedef struct {
	Rectangle shape;
	Color color;
	Color usedColor;
	int outline; // If not zero acts has bool and outline width
	int id;
} Button;


void AddButton(Button bt, Button* btArray) {
	return;
};

void BulkDrawButton(Button* btArray) {
	//int brLength = sizeof(btArray)*64;

	for (int i = 64; i > 0; i--) {
		if (btArray[i].id != -1) {
			DrawRectangleRec(btArray[i].shape, btArray[i].color);
			if (btArray[i].outline != 0) {
				DrawRectangleLinesEx(btArray[i].shape, btArray[i].outline, btArray[i].usedColor);
			}
			TraceLog(LOG_DEBUG, TextFormat("Drawed Button %d", btArray[i].id));
		};
	};
};


bool ColorEquals(Color a, Color b) {
	return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}


void CenterRectangleRec(Rectangle* rec) {
	rec->x = (float)RENDER_WIDTH/2-rec->width/2;
	rec->y = (float)RENDER_HEIGHT/2-rec->height/2;
}

void DrawCenterText(const char* text, int fontsize, Color color) {
	DrawText(text, RENDER_WIDTH/2-MeasureText(text, fontsize)/2, RENDER_HEIGHT/2-fontsize/2, fontsize, color);
}

void DrawXCenterText(const char* text, int y, int fontsize, Color color) {
	DrawText(text, RENDER_WIDTH/2-MeasureText(text, fontsize)/2, y, fontsize, color);
};

void DrawRecCenterText(Rectangle rec, const char* text, int fontsize, Color color) {
	//Draw text at a rectangle's center
	int cx = rec.x + rec.width /2;
	int cy = rec.y + rec.height/2;
	DrawText(text, cx-MeasureText(text, fontsize)/2, cy-fontsize/2, fontsize, color);
}


Vector2 GetVirtualMouse() {
	Vector2 mouse = GetMousePosition();
	float scaleX = (float)RENDER_WIDTH / SCREEN_WIDTH;
	float scaleY = (float)RENDER_HEIGHT / SCREEN_HEIGHT;
	//TraceLog(LOG_DEBUG, TextFormat("scale: %.2f %.2f %.2f", scaleX, scaleY));
	//TraceLog(LOG_DEBUG, TextFormat("virtual: %.2f %.2f", scaleX*mouse.x, scaleY*mouse.y));
	return (Vector2){ mouse.x * scaleX, mouse.y * scaleY };
}

int main()
{
	//Start
	//SCREEN_WIDTH  = GetScreenWidth();
	//SCREEN_HEIGHT = GetScreenHeight();
	float TimeRemaining = 0;
	int score = 0;
	int scorePerClick = 1;

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Clicker Game");
	//SetWindowMonitor(0);
	TraceLog(LOG_INFO, TextFormat("%d", GetMonitorCount()));
	SCREEN_WIDTH  = GetScreenWidth();
	SCREEN_HEIGHT = GetScreenHeight();
	SetTargetFPS(512);
	SetTraceLogLevel(LOG_DEBUG);

	//Texture2D texture = LoadTexture(ASSETS_PATH"test.png");

	Button btArray[64]  = {[0 ... 63] = (Button){(Rectangle){0, 0, 0, 0}, RED, RED, 0, -1}};
	btArray[1] = (Button){(Rectangle){500, 200, 100, 100}, RED, DARKBROWN, 5, 1};


	Color rectanglecolor = {255, 0, 0, 255};

	Rectangle rec = {0, 0, 200, 200};
	CenterRectangleRec(&rec);

	RenderTexture fb = LoadRenderTexture(960, 540);

	while (!WindowShouldClose())
	{   //Update

		const float DeltaTime = GetFrameTime();
		if (CheckCollisionPointRec(GetVirtualMouse(), rec)) {
			if (IsMouseButtonPressed(0)) {
					TraceLog(LOG_DEBUG, "Button was clicked");
					rectanglecolor = (Color){150, 0, 0 ,255};
					TimeRemaining = 0.1f;
					score++;



			}
		}

		if (TimeRemaining > 0) {
			TimeRemaining -= DeltaTime;
			if (TimeRemaining <= 0) {
				rectanglecolor = (Color){255, 0,0 ,255};
			}
		}

		// Draw
		BeginTextureMode(fb);



		ClearBackground(SKYBLUE);
		DrawFPS(10, 10);

		DrawXCenterText(TextFormat("%d", score), 75, 32, BLACK);

		DrawRectangleRec((Rectangle){25, 200, 100, 100}, (Color){255,0,0,255});
		DrawRectangleLinesEx((Rectangle){25, 200, 100, 100}, 3, (Color){125,0,0,255});
		DrawRectangleRec(rec, rectanglecolor);
		DrawRectangleLinesEx(rec, 5, (Color){125, 0, 0, 255});
		DrawCenterText("CLICK", 32, BLACK);
		DrawRecCenterText((Rectangle){25, 200, 100, 100}, "+1", 32, BLACK);
		BulkDrawButton(btArray);

		EndTextureMode();

		BeginDrawing();
		ClearBackground(RED);

		Rectangle source = { 0, 0, fb.texture.width, -fb.texture.height }; // flip Y
		Rectangle dest = { 0, 0, GetScreenWidth(), GetScreenHeight() };
		Vector2 origin = { 0, 0 };

		DrawTexturePro(fb.texture, source, dest, origin, 0.0f, WHITE);

		EndDrawing();
	}

	//UnloadTexture(texture);

	UnloadRenderTexture(fb);
	CloseWindow();
	return 0;
}
