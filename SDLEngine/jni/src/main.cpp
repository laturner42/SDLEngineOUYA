
#include <string>
#include <SDL_image.h>
#include "SDL.h"
#include "SDL_thread.h"
#include "SDL_timer.h"
#include "SDL_rwops.h"
#include "jni.h"
#include <android/log.h>

#include "pic.h"

#define LOG_TAG "SDLTests"
#define LOGI(...) __android_log_print(4, LOG_TAG, __VA_ARGS__);
#define LOGE(...) __android_log_print(6, LOG_TAG, __VA_ARGS__);

//using namespace std;
#ifdef __cplusplus
extern "C" {
#endif


// displaying
SDL_Window *pWindow;
static SDL_Renderer *pRenderer;
SDL_Texture *bmpTex;
int pitch, size, width = 1000, height = 1000;

int x, y;
int bgColor [3];

bool running;

// SDL event handling
SDL_Event event;

Pic *myImage;
Pic *myImage2;

using namespace std;

void begin();
void setBGColor(int r, int g, int b);

void Clear(){
	SDL_SetRenderDrawColor(pRenderer, bgColor[0], bgColor[1], bgColor[2], 255);
	SDL_RenderClear(pRenderer);
}

void Present(){
	SDL_RenderPresent(pRenderer);
}

int main(int argc, char* argv[]) {

	LOGI("application start.");

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		return -1;
	}

	LOGI("SDL initialize.");
//	width = pCodecCtx->width;
//	height = pCodecCtx->height;

	// allocate window, renderer, texture
	pWindow = SDL_CreateWindow("UA CAM", 0, 0, width, height, SDL_WINDOW_SHOWN);
	LOGI("SDL window created. %d, %d", width, height);

	pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	LOGI("SDL renderer created.");

	bmpTex = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_YV12,
			SDL_TEXTUREACCESS_STREAMING, width, height);
	LOGI("SDL texture created.");

	setBGColor(0,0,100);

	//size = pCodecCtx->width * pCodecCtx->height;
	size = width * height;
	if (pWindow == NULL | pRenderer == NULL | bmpTex == NULL) {
		fprintf(stderr, "Could not open window1\n%s\n", SDL_GetError());
		return -1;
	}

	// Simply SDL create a thread
//	thread = SDL_CreateThread(decodeAndRender, "decodeAndRender",(void *) NULL);
//	if (NULL == thread)
//		printf("\nSDL_CreateThread failed: %s\n", SDL_GetError());
//	else {
//		LOGI("SDL thread created.");
//		SDL_WaitThread(thread, &threadReturnValue);
//		printf("\nThread returned value:%d", threadReturnValue);
//	}

	// without a thread - using the main thread
	//decodeAndRender( (void*)NULL) ;

	begin();

	// release
	// *note SDL objects have to be freed before closing avcodec.
	// otherwise it causes segmentation fault for some reason.
	LOGI("destroying SDL references.");
	SDL_DestroyTexture(bmpTex);
	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);

	LOGI("SDL quit.");
	SDL_Quit();

	return 0;

}

void begin(){

	x = 0; y = 0;

	running = true;

	int fps = 1;
	int fpsCounter = 0;
	long start = SDL_GetTicks();

	LOGI("Begin!");

	const std::string fName = "image.png";
	myImage = new Pic();
	myImage->Init(pRenderer, &fName, 2);

	myImage2 = new Pic();
	myImage2->Init(pRenderer, &fName, 1);

	myImage->SetPos(width/2-100,height/2-100);
	myImage2->SetPos(width/2,height/2-100);

	while (running) {

		fpsCounter++;
		if (SDL_GetTicks() - start >= 1000){
			fps = fpsCounter;
			LOGI("FPS: %d", fps);
			start = SDL_GetTicks();
			fpsCounter = 0;
		}

		Clear();

		SDL_Rect rectToDraw = {0,0,width-1,height-1};
		SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(pRenderer, &rectToDraw);

		myImage->Paint();
		myImage2->Paint();

		Present();
	}
}

void JNICALL Java_org_libsdl_app_SDLActivity_onNativeTouch(JNIEnv* env, jobject t, int touchDevID, int pointerFingerID, int action, float px, float py, float p){
	int mousex = (int)(width*px);
	int mousey = (int)(height*py);

	if (action == 0 || action == 5){
		myImage->Select(pointerFingerID,mousex,mousey);
		myImage2->Select(pointerFingerID,mousex,mousey);
	} else if (action == 2){
		myImage->MoveOne(pointerFingerID,mousex,mousey);
		myImage2->MoveOne(pointerFingerID,mousex,mousey);
	} else {
		myImage->Release();
		myImage2->Release();
	}
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Info: %d, %d, %d", touchDevID, pointerFingerID, action);
}

JNIEXPORT void JNICALL Java_org_libsdl_app_SDLActivity_onNativeScreenSize(JNIEnv* env, jobject t, int w, int h){
	width = w;
	height = h;
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Screen size change: %d, %d", w, h);
}


void setBGColor(int r, int g, int b){
	bgColor[0] = r;
	bgColor[1] = g;
	bgColor[2] = b;
}

#ifdef __cplusplus
}
#endif
