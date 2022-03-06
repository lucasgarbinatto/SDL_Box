#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <list>
#include <SDL.h>

#define speed 10
#define recth 50
#define rectw 60
#define TAM 10

void randomizar(std::list<SDL_Rect*> rectangles, SDL_Renderer* renderer, SDL_Rect* current) {
    for (auto& rect : rectangles) {
        if (SDL_HasIntersection(rect, current))
        {
            SDL_SetRenderDrawColor(renderer, 0, 9, 255, 255);
        }
        else
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (auto& rect2 : rectangles) {
            if (SDL_HasIntersection(rect, rect2) && rect != rect2) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, rect2);
            }
        }
        SDL_RenderFillRect(renderer, rect);
    }
}
/*colisao automatica, gerar outro retangulo ao encostar random, tocar som , -> colocar vector de rect p os pintados tirando os nao pintados*/
int main(int argc, char** argv)
{
    srand(time(NULL));
    bool quit = false, leftMouseButtonDown = false;
    SDL_Event event;
    SDL_Point mousePos;
    // init SDL

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SDL2 Bounding Box Collision Detection",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Renderer* x = SDL_CreateRenderer(window, -1, 0);

    SDL_Rect* selectedRect = NULL;
    SDL_Rect* rectAtual = nullptr;
    SDL_Rect rect3[TAM];
    for (int i = 0; i < TAM; i++) {
        rect3[i] = { rand() % 88 * i, rand() % 120 * i,rectw, recth };
    }

    std::list<SDL_Rect*> rectangles;
    for (int i = 0; i < TAM; i++)
        rectangles.push_back(&rect3[i]);

    while (!quit)
    {
        SDL_Delay(10);
        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_MOUSEBUTTONUP:
            if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT)
            {
                leftMouseButtonDown = false;
                selectedRect = NULL;
            }
            break;
        case SDL_MOUSEMOTION:
        {
            mousePos = { event.motion.x, event.motion.y };
            if (selectedRect != NULL) { //erro c6011
                break;
                if (leftMouseButtonDown && selectedRect == nullptr)
                {
                    selectedRect->x = mousePos.x;
                    selectedRect->y = mousePos.y;
                }
            }
        }
        break;
        case SDL_MOUSEBUTTONDOWN:
            if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT)
            {
                leftMouseButtonDown = true;
                for (auto rect : rectangles)
                {
                    if (SDL_PointInRect(&mousePos, rect))
                    {
                        selectedRect = rect;
                        rectAtual = selectedRect;
                        break;
                    }
                }
            }
            break;
        case SDL_KEYDOWN:
            if (rectAtual != NULL) {
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    if (rectAtual->x <= 10)
                        rectAtual->x = 10;
                    rectAtual->x -= speed;
                    break;
                case SDLK_RIGHT:
                    if (rectAtual->x >= 580)
                        rectAtual->x = 570;
                    rectAtual->x += speed;
                    break;
                case SDLK_UP:
                    if (rectAtual->y <= 10)
                        rectAtual->y = 10;
                    rectAtual->y -= speed;
                    break;
                case SDLK_DOWN:
                    if (rectAtual->y >= 420)
                        rectAtual->y = 420;
                    rectAtual->y += speed;
                    break;
                }
            }
            break;
        }
        SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
        SDL_RenderClear(renderer);
        randomizar(rectangles, renderer, rectAtual);
        SDL_RenderPresent(renderer);
    }
    // cleanup SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}