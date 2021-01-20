#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdbool.h>
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360
#define SPEED 500
#define BALL_SPEED 300

int main()
{
    // Initialzie SDL2
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("SDL init error: %s\n", SDL_GetError());
        SDL_Quit();

        return 1;
    }

    // Create a SDL window
    SDL_Window *window = SDL_CreateWindow("My game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    // Check if window created or not
    if(!window)
    {
        printf("SDL window error: %s\n", SDL_GetError());
        SDL_Quit();

        return 1;
    }

    // Create a SDL renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // check if renderer created or not
    if(!renderer)
    {
        printf("SDL renderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    // Create rectangle for players
    SDL_Rect player_left_rect;
    SDL_Rect player_right_rect;

    player_left_rect.h = 75;
    player_left_rect.w = 10;
    player_left_rect.x = 10;
    player_left_rect.y = (WINDOW_HEIGHT - player_left_rect.h) / 2;

    player_right_rect.h = 75;
    player_right_rect.w = 10;
    player_right_rect.x = WINDOW_WIDTH - 10 - 10;
    player_right_rect.y = (WINDOW_HEIGHT - player_right_rect.h) / 2;

    // Create rectangle for ball
    SDL_Rect ball_rect;

    ball_rect.h = 10;
    ball_rect.w = 10;
    ball_rect.x = (WINDOW_WIDTH - ball_rect.w) / 2;
    ball_rect.y = (WINDOW_HEIGHT - ball_rect.h) / 2;

    // ball velocity
    int ball_vel_x = BALL_SPEED;
    int ball_vel_y = BALL_SPEED;


    int player_left_up = 0;
    int player_left_down = 0;
    int player_right_up = 0;
    int player_right_down = 0;

    int left_score = 0;
    int right_score = 0;

    int close_requested = false;
    // Animation loop
    while(!close_requested)
    {
        SDL_Event event;
        // SDL events
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    close_requested = true;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_UP:
                            player_right_up = 1;
                            break;
                        case SDL_SCANCODE_DOWN:
                            player_right_down = 1;
                            break;
                        case SDL_SCANCODE_W:
                            player_left_up = 1;
                            break;
                        case SDL_SCANCODE_S:
                            player_left_down = 1;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_UP:
                            player_right_up = 0;
                            break;
                        case SDL_SCANCODE_DOWN:
                            player_right_down = 0;
                            break;
                        case SDL_SCANCODE_W:
                            player_left_up = 0;
                            break;
                        case SDL_SCANCODE_S:
                            player_left_down = 0;
                            break;
                    }
                    break;
            }
        }

        if(player_left_down && !player_left_up) player_left_rect.y += SPEED / 60;
        if(player_left_up && !player_left_down) player_left_rect.y -= SPEED / 60;
        if(player_right_down && !player_right_up) player_right_rect.y += SPEED / 60;
        if(player_right_up && !player_right_down) player_right_rect.y -= SPEED / 60;

        if(ball_rect.x == (player_left_rect.x + 10) && (ball_rect.y >= player_left_rect.y && ball_rect.y <= (player_left_rect.y + player_left_rect.h)))
        {
            ball_vel_x = -ball_vel_x;
        }
        if((ball_rect.x + 10) == player_right_rect.x && (ball_rect.y >= player_right_rect.y && ball_rect.y <= (player_right_rect.y + player_right_rect.h)))
        {
            ball_vel_x = -ball_vel_x;
        }

        if(player_left_rect.y <= 0) player_left_rect.y = 0;
        if(player_left_rect.y >= (WINDOW_HEIGHT - player_left_rect.h)) player_left_rect.y = WINDOW_HEIGHT - player_left_rect.h;
        if(player_right_rect.y <= 0) player_right_rect.y = 0;
        if(player_right_rect.y >= (WINDOW_HEIGHT - player_right_rect.h)) player_right_rect.y = WINDOW_HEIGHT - player_right_rect.h;
        if(ball_rect.y <= 0)
        {
            ball_rect.y = 0;
            ball_vel_y = -ball_vel_y;
        }
        if(ball_rect.y >= (WINDOW_HEIGHT - ball_rect.h))
        {
            ball_rect.y = WINDOW_HEIGHT - ball_rect.h;
            ball_vel_y = -ball_vel_y;
        }
        if(ball_rect.x <= 0)
        {
            right_score++;
            printf("Left Score is %d\n", left_score);
            printf("Right Score is %d\n", right_score);
            ball_rect.x = (WINDOW_WIDTH - ball_rect.w) / 2;
            ball_rect.y = (WINDOW_HEIGHT - ball_rect.h) / 2;

            // ball velocity
            ball_vel_x = -ball_vel_x;
            ball_vel_y = -ball_vel_y;
        }
        if(ball_rect.x >= (WINDOW_WIDTH - ball_rect.w))
        {
            left_score++;
            printf("Left Score is %d\n", left_score);
            printf("Right Score is %d\n", right_score);
            ball_rect.x = (WINDOW_WIDTH - ball_rect.w) / 2;
            ball_rect.y = (WINDOW_HEIGHT - ball_rect.h) / 2;

            // ball velocity
            ball_vel_x = -ball_vel_x;
            ball_vel_y = -ball_vel_y;
        }

        ball_rect.x += ball_vel_x / 60;
        ball_rect.y += ball_vel_y / 60;
        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &player_left_rect);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &player_right_rect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &ball_rect);
        SDL_RenderPresent(renderer);


        SDL_Delay(1000 / 60);
    }



    // Free memory
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
