#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "pic.h"
#include <android/log.h>

using namespace std;

#define LOG_TAG "SDLTests"
#define LOGI(...) __android_log_print(4, LOG_TAG, __VA_ARGS__);
#define LOGE(...) __android_log_print(6, LOG_TAG, __VA_ARGS__);

static SDL_Renderer *pRenderer;

SDL_Texture *image;
int xx, yy, wwidth, hheight;
int cx, cy;
int finger;
int circle;

bool moving;
bool scaling;

void Pic::Draw(SDL_Texture *tex, SDL_Rect &dstRect, SDL_Rect *srcRect, float angle, int xPivot, int yPivot, SDL_RendererFlip flip){
	// Convert the pivot position from relative to the object's center
	// to relative to the screen space
	//xPivot += dstRect.w/2;
	//yPivot += dstRect.h/2;
	SDL_Point pivot = {xPivot, yPivot};

	// mRenderer is a unique pointer, so we use .get() to get the raw pointer
	SDL_RenderCopyEx(pRenderer, tex, srcRect, &dstRect, angle, &pivot, flip);
}

SDL_Texture* Pic::LoadImage(const std::string &file){
	SDL_Surface *surface = IMG_Load_RW(SDL_RWFromFile(file.c_str(), "rb"), 1);
	SDL_Texture *texture = nullptr;
	if(surface == nullptr) {
		printf("IMG_Load_RW: %s\n", IMG_GetError());
	} else {
		texture = SDL_CreateTextureFromSurface(pRenderer, surface);
		SDL_FreeSurface(surface);
	}
	return texture;
}

void Pic::Init(SDL_Renderer* ren, const std::string *file, int num){
	pRenderer = ren;
	image = LoadImage(*file);
	xx = 0, yy = 0;
	SDL_QueryTexture(image, NULL, NULL, &wwidth, &hheight);
	int circle = num;
	wwidth/=2;
	hheight/=2;
}

void Pic::MoveOne(int id, int x, int y){
	if (moving && id == finger) {
		xx = x + cx;
		yy = y + cy;
	}
}

void Pic::SetPos(int x, int y){
	xx = x;
	yy = y;
}

void Pic::Select(int id, int x, int y){
	if (!moving){
		if (x > xx && y > yy && x < xx+wwidth && y < yy+hheight){
			finger = id;
			LOGI("Selected.");
			moving = true;
			cx = xx-x;
			cy = yy-y;
		}
	}
}

void Pic::Release(){
	if (moving)
		LOGI("Deselected.");
	moving = false;
}

void Pic::Paint(){
	SDL_Rect pos = {xx, yy, wwidth, hheight};
	float angle = 0.0;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	SDL_Rect src;
	int imageWidth = wwidth;
	int imageHeight = hheight;
	src.x = (circle%2)*imageWidth;
	src.y = (circle/2)*imageHeight;
	src.w = imageWidth;
	src.h = imageHeight;
	Pic::Draw(image, pos, &src, angle, 0, 0, flip);
}
