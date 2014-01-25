#ifndef PIC_H
#define PIC_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Pic {
public:
	static SDL_Texture* LoadImage(const std::string &file);
	void Init(SDL_Renderer * ren, const std::string *file, int num = 0);
	void Paint();
	void SetPos(int x, int y);
	void MoveOne(int id, int x, int y);
	void Select(int id, int x, int y);
	void Release();
	static void Draw(SDL_Texture*, SDL_Rect &dstRect, SDL_Rect *srcRect = NULL,
				float angle = 0.0, int xPivot = 0, int yPivot = 0,
				SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
	int xx, yy, width, height, moving, cx, cy, finger, circle;
};
#endif
