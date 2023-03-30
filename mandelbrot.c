#include <SDL2/SDL.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITER 50

typedef struct imaginary {
    double real;
    double i;
} Imaginary;

Imaginary square(Imaginary i);
Imaginary add(Imaginary a, Imaginary b);
int is_in_set(Imaginary c);
double map(double, double, double, double, double);

int main() {

    double ymax = 1.2;
    double ymin = -1.2;
    double xmax = 1;
    double xmin = -2;

    int height = 800;
    int width = 1000;
    //int width = (int)(height*(fabs(xmax)+fabs(xmin)))/(fabs(ymax)+fabs(ymin));

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(width,height,0, &window, &renderer);

    double xinc = (fabs(xmin)+fabs(xmax))/width;
    double yinc = (fabs(ymin)+fabs(ymax))/height;

    for(double x = xmin; x < xmax; x += xinc) {
        for(double y = ymin; y < ymax; y += yinc) {
            Imaginary c = {x, y};
            int iters = is_in_set(c);
            if(iters == 0) {
                SDL_SetRenderDrawColor(renderer, 0,0,0,255);
            }
            else {
                SDL_SetRenderDrawColor(renderer,
                                       1*iters%255,
                                       4*iters%255,
                                       5*iters%255,
                                       255);
            }
            int x1 = map(x, xmin, xmax, 0, width);
            int y1 = map(y, ymax, ymin, 0, height);
            SDL_RenderDrawPoint(renderer, x1, y1);
        }
    }

    SDL_RenderPresent(renderer);

    int quit = 0;
    while (quit == 0) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
            break;
        }
        else if(event.type == SDL_MOUSEBUTTONDOWN) {
            printf("CLick at %d, %d\n", event.motion.x, event.motion.y);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

Imaginary add(Imaginary a, Imaginary b) {
    Imaginary res = {a.real+b.real, a.i+b.i};
    return res;
}

Imaginary square(Imaginary i) {
    Imaginary res = {i.real*i.real - i.i*i.i, 2*i.real*i.i};
    return res;
}

int is_in_set(Imaginary c) {

    Imaginary z = {0, 0};
    int i;

    for(i = 0; i < MAX_ITER; ++i) {
        z = square(z);
        z = add(z, c);
        int l = z.i*z.i + z.real*z.real;
        if(l > 2) {
            return i;
        }
    }

    return 0;
}


double map(double x, double a, double b, double c, double d) {
    double res = ((x-a)*(d-c))/(b-a);
    return res + c;
}
