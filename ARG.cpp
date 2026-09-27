#include "splashkit.h"

using namespace std;

struct car_data
{
    double x;
    double y;
    double speed;
    int score;
    int energy;
};

struct obstacle_data
{
    double x;
    double y;
    bool active;
};

struct coin_data
{
    double x;
    double y;
    bool collected;
};

struct zone_data
{
    double x;
    double y;
    bool active;
};

struct position_data
{
    double x;
    double y;
};

void move_car(car_data &car)
{
    if (key_down(LEFT_KEY))
    {
        car.x = car.x - car.speed;
    }

    if (key_down(RIGHT_KEY))
    {
        car.x = car.x + car.speed;
    }

    if (key_down(UP_KEY))
    {
        car.y = car.y - car.speed;
    }

    if (key_down(DOWN_KEY))
    {
        car.y = car.y + car.speed;
    }

    if (car.x < 175)
    {
        car.x = 175;
    }

    if (car.x > 725)
    {
        car.x = 725;
    }

    if (car.y < 95)
    {
        car.y = 95;
    }

    if (car.y > 520)
    {
        car.y = 520;
    }
}

void draw_track()
{
    clear_screen(COLOR_DARK_GREEN);

    fill_rectangle(COLOR_GRAY, 150, 60, 600, 500);

    fill_rectangle(COLOR_WHITE, 150, 60, 10, 500);
    fill_rectangle(COLOR_WHITE, 740, 60, 10, 500);

    fill_rectangle(COLOR_WHITE, 150, 70, 600, 20);

    for (int x = 150; x < 750; x = x + 40)
    {
        fill_rectangle(COLOR_BLACK, x, 70, 20, 20);
    }

    for (int y = 120; y < 520; y = y + 70)
    {
        fill_rectangle(COLOR_YELLOW, 445, y, 10, 35);
    }
}

void draw_car(car_data car)
{
    fill_rectangle(
        COLOR_RED,
        car.x - 15,
        car.y - 25,
        30,
        50
    );

    fill_rectangle(
        COLOR_BLUE,
        car.x - 10,
        car.y - 15,
        20,
        15
    );

    fill_rectangle(
        COLOR_BLACK,
        car.x - 20,
        car.y - 20,
        5,
        15
    );

    fill_rectangle(
        COLOR_BLACK,
        car.x + 15,
        car.y - 20,
        5,
        15
    );

    fill_rectangle(
        COLOR_BLACK,
        car.x - 20,
        car.y + 10,
        5,
        15
    );

    fill_rectangle(
        COLOR_BLACK,
        car.x + 15,
        car.y + 10,
        5,
        15
    );
}

void draw_ghost(
    position_data ghost[],
    int ghost_count,
    int ghost_index)
{
    if (ghost_count > 0 && ghost_index < ghost_count)
    {
        fill_rectangle(
            COLOR_LIGHT_GRAY,
            ghost[ghost_index].x - 12,
            ghost[ghost_index].y - 22,
            24,
            44
        );

        fill_rectangle(
            COLOR_GRAY,
            ghost[ghost_index].x - 8,
            ghost[ghost_index].y - 12,
            16,
            12
        );
    }
}

void draw_obstacles(
    obstacle_data obstacles[],
    int number_of_obstacles)
{
    for (int i = 0; i < number_of_obstacles; i++)
    {
        if (obstacles[i].active)
        {
            fill_rectangle(
                COLOR_BLACK,
                obstacles[i].x - 15,
                obstacles[i].y - 15,
                30,
                30
            );

            fill_rectangle(
                COLOR_RED,
                obstacles[i].x - 10,
                obstacles[i].y - 5,
                20,
                10
            );
        }
    }
}

void draw_coins(
    coin_data coins[],
    int number_of_coins)
{
    for (int i = 0; i < number_of_coins; i++)
    {
        if (!coins[i].collected)
        {
            fill_circle(
                COLOR_YELLOW,
                coins[i].x,
                coins[i].y,
                10
            );

            fill_circle(
                COLOR_WHITE,
                coins[i].x,
                coins[i].y,
                4
            );
        }
    }
}

void draw_zones(
    zone_data turbo_zones[],
    zone_data slow_zones[],
    int number_of_zones)
{
    for (int i = 0; i < number_of_zones; i++)
    {
        if (turbo_zones[i].active)
        {
            fill_rectangle(
                COLOR_CYAN,
                turbo_zones[i].x - 25,
                turbo_zones[i].y - 15,
                50,
                30
            );
        }

        if (slow_zones[i].active)
        {
            fill_rectangle(
                COLOR_YELLOW,
                slow_zones[i].x - 25,
                slow_zones[i].y - 15,
                50,
                30
            );
        }
    }
}

bool rectangles_collide(
    double x1,
    double y1,
    double width1,
    double height1,
    double x2,
    double y2,
    double width2,
    double height2)
{
    if (x1 < x2 + width2 &&
        x1 + width1 > x2 &&
        y1 < y2 + height2 &&
        y1 + height1 > y2)
    {
        return true;
    }

    return false;
}

bool check_obstacle_collision(
    car_data car,
    obstacle_data obstacles[],
    int number_of_obstacles)
{
    for (int i = 0; i < number_of_obstacles; i++)
    {
        if (obstacles[i].active)
        {
            bool collision = rectangles_collide(
                car.x - 15,
                car.y - 25,
                30,
                50,
                obstacles[i].x - 15,
                obstacles[i].y - 15,
                30,
                30
            );

            if (collision)
            {
                return true;
            }
        }
    }

    return false;
}

void check_coin_collision(
    car_data &car,
    coin_data coins[],
    int number_of_coins)
{
    for (int i = 0; i < number_of_coins; i++)
    {
        if (!coins[i].collected)
        {
            bool collision = rectangles_collide(
                car.x - 15,
                car.y - 25,
                30,
                50,
                coins[i].x - 10,
                coins[i].y - 10,
                20,
                20
            );

            if (collision)
            {
                coins[i].collected = true;
                car.score = car.score + 10;

                if (car.energy < 100)
                {
                    car.energy = car.energy + 10;
                }

                if (car.energy > 100)
                {
                    car.energy = 100;
                }
            }
        }
    }
}

bool check_zone_collision(
    car_data car,
    zone_data zones[],
    int number_of_zones)
{
    for (int i = 0; i < number_of_zones; i++)
    {
        if (zones[i].active)
        {
            bool collision = rectangles_collide(
                car.x - 15,
                car.y - 25,
                30,
                50,
                zones[i].x - 25,
                zones[i].y - 15,
                50,
                30
            );

            if (collision)
            {
                return true;
            }
        }
    }

    return false;
}

void damage_car(car_data &car)
{
    car.energy = car.energy - 25;

    if (car.energy < 0)
    {
        car.energy = 0;
    }

    car.x = 450;
    car.y = 500;
}

void shift_track(
    obstacle_data obstacles[],
    int number_of_obstacles,
    int lap)
{
    if (lap == 2)
    {
        obstacles[0].x = 600;
        obstacles[0].y = 430;

        obstacles[1].x = 280;
        obstacles[1].y = 350;

        obstacles[2].x = 620;
        obstacles[2].y = 250;

        obstacles[3].x = 350;
        obstacles[3].y = 160;
    }

    if (lap == 3)
    {
        obstacles[0].x = 300;
        obstacles[0].y = 420;

        obstacles[1].x = 650;
        obstacles[1].y = 380;

        obstacles[2].x = 400;
        obstacles[2].y = 230;

        obstacles[3].x = 600;
        obstacles[3].y = 140;
    }

    for (int i = 0; i < number_of_obstacles; i++)
    {
        obstacles[i].active = true;
    }
}

void draw_information(
    car_data car,
    int lap,
    int time_left,
    double current_speed)
{
    draw_text(
        "ADAPTIVE RACING GAME",
        COLOR_WHITE,
        20,
        15
    );

    draw_text(
        "Score: " + std::to_string(car.score),
        COLOR_WHITE,
        20,
        590
    );

    draw_text(
        "Lap: " + std::to_string(lap) + " / 3",
        COLOR_WHITE,
        180,
        590
    );

    draw_text(
        "Time: " + std::to_string(time_left),
        COLOR_WHITE,
        310,
        590
    );

    draw_text(
        "Energy: " + std::to_string(car.energy),
        COLOR_WHITE,
        420,
        590
    );

    draw_text(
        "Speed: " + std::to_string((int)current_speed),
        COLOR_WHITE,
        560,
        590
    );
}

void draw_start_screen()
{
    clear_screen(COLOR_DARK_GREEN);

    draw_text(
        "ADAPTIVE RACING GAME",
        COLOR_CYAN,
        260,
        120
    );

    draw_text(
        "A RACING GAME THAT CHANGES AS YOU PLAY",
        COLOR_WHITE,
        225,
        165
    );

    draw_text(
        "Complete 3 laps before time runs out.",
        COLOR_WHITE,
        250,
        230
    );

    draw_text(
        "Collect energy to keep your car alive.",
        COLOR_WHITE,
        245,
        265
    );

    draw_text(
        "Avoid obstacles and use the turbo zones.",
        COLOR_WHITE,
        225,
        300
    );

    draw_text(
        "The track changes after every lap.",
        COLOR_YELLOW,
        255,
        335
    );

    draw_text(
        "Beat your previous lap with the ghost racer.",
        COLOR_YELLOW,
        230,
        370
    );

    draw_text(
        "Arrow Keys = Move",
        COLOR_WHITE,
        330,
        430
    );

    draw_text(
        "Press SPACE to start",
        COLOR_CYAN,
        315,
        480
    );
}

void draw_game_over_screen(car_data car)
{
    clear_screen(COLOR_BLACK);

    draw_text(
        "RACE OVER",
        COLOR_RED,
        330,
        180
    );

    if (car.energy <= 0)
    {
        draw_text(
            "Your car ran out of energy.",
            COLOR_WHITE,
            280,
            240
        );
    }
    else
    {
        draw_text(
            "You ran out of time.",
            COLOR_WHITE,
            300,
            240
        );
    }

    draw_text(
        "Final Score: " + std::to_string(car.score),
        COLOR_WHITE,
        300,
        290
    );

    draw_text(
        "Press SPACE to race again",
        COLOR_YELLOW,
        275,
        370
    );
}

void draw_winner_screen(
    car_data car,
    int best_lap)
{
    clear_screen(COLOR_DARK_GREEN);

    draw_text(
        "RACE COMPLETE!",
        COLOR_YELLOW,
        290,
        150
    );

    draw_text(
        "You completed all 3 laps.",
        COLOR_WHITE,
        280,
        210
    );

    draw_text(
        "Final Score: " + std::to_string(car.score),
        COLOR_WHITE,
        300,
        260
    );

    draw_text(
        "Energy Remaining: " + std::to_string(car.energy),
        COLOR_WHITE,
        285,
        300
    );

    draw_text(
        "Best Lap: " + std::to_string(best_lap) + " seconds",
        COLOR_CYAN,
        290,
        340
    );

    draw_text(
        "Press SPACE to race again",
        COLOR_YELLOW,
        275,
        410
    );
}

void reset_game(
    car_data &car,
    obstacle_data obstacles[],
    int number_of_obstacles,
    coin_data coins[],
    int number_of_coins,
    zone_data turbo_zones[],
    zone_data slow_zones[],
    int number_of_zones)
{
    car.x = 450;
    car.y = 500;
    car.speed = 4;
    car.score = 0;
    car.energy = 100;

    obstacles[0].x = 250;
    obstacles[0].y = 400;
    obstacles[0].active = true;

    obstacles[1].x = 650;
    obstacles[1].y = 350;
    obstacles[1].active = true;

    obstacles[2].x = 350;
    obstacles[2].y = 250;
    obstacles[2].active = true;

    obstacles[3].x = 600;
    obstacles[3].y = 180;
    obstacles[3].active = true;

    for (int i = 0; i < number_of_obstacles; i++)
    {
        obstacles[i].active = true;
    }

    coins[0].x = 220;
    coins[0].y = 450;

    coins[1].x = 500;
    coins[1].y = 400;

    coins[2].x = 680;
    coins[2].y = 280;

    coins[3].x = 300;
    coins[3].y = 170;

    coins[4].x = 550;
    coins[4].y = 120;

    for (int i = 0; i < number_of_coins; i++)
    {
        coins[i].collected = false;
    }

    turbo_zones[0].x = 250;
    turbo_zones[0].y = 300;
    turbo_zones[0].active = true;

    turbo_zones[1].x = 650;
    turbo_zones[1].y = 200;
    turbo_zones[1].active = true;

    slow_zones[0].x = 350;
    slow_zones[0].y = 430;
    slow_zones[0].active = true;

    slow_zones[1].x = 600;
    slow_zones[1].y = 300;
    slow_zones[1].active = true;

    for (int i = 0; i < number_of_zones; i++)
    {
        turbo_zones[i].active = true;
        slow_zones[i].active = true;
    }
}

int main()
{
    open_window("Adaptive Racing Game", 900, 650);

    create_timer("GameTimer");
    start_timer("GameTimer");

    car_data car;

    const int NUMBER_OF_OBSTACLES = 4;
    const int NUMBER_OF_COINS = 5;
    const int NUMBER_OF_ZONES = 2;
    const int MAX_GHOST_POSITIONS = 1000;

    obstacle_data obstacles[NUMBER_OF_OBSTACLES];
    coin_data coins[NUMBER_OF_COINS];

    zone_data turbo_zones[NUMBER_OF_ZONES];
    zone_data slow_zones[NUMBER_OF_ZONES];

    position_data ghost_positions[MAX_GHOST_POSITIONS];
    position_data current_lap_positions[MAX_GHOST_POSITIONS];

    int ghost_count = 0;
    int ghost_index = 0;
    int current_lap_count = 0;

    int game_state = 0;
    int lap = 1;
    int time_left = 60;
    int best_lap = 0;

    bool turbo_active = false;
    bool slow_active = false;

    reset_game(
        car,
        obstacles,
        NUMBER_OF_OBSTACLES,
        coins,
        NUMBER_OF_COINS,
        turbo_zones,
        slow_zones,
        NUMBER_OF_ZONES
    );

    while (!window_close_requested("Adaptive Racing Game"))
    {
        process_events();

        if (game_state == 0)
        {
            draw_start_screen();

            if (key_typed(SPACE_KEY))
            {
                reset_game(
                    car,
                    obstacles,
                    NUMBER_OF_OBSTACLES,
                    coins,
                    NUMBER_OF_COINS,
                    turbo_zones,
                    slow_zones,
                    NUMBER_OF_ZONES
                );

                lap = 1;
                time_left = 60;
                best_lap = 0;
                ghost_count = 0;
                ghost_index = 0;
                current_lap_count = 0;
                turbo_active = false;
                slow_active = false;

                reset_timer("GameTimer");

                game_state = 1;
            }
        }
        else if (game_state == 1)
        {
            if (ghost_count > 0)
            {
                ghost_index = ghost_index + 1;

                if (ghost_index >= ghost_count)
                {
                    ghost_index = ghost_count - 1;
                }
            }

            double current_speed = 4;

            if (lap == 2)
            {
                current_speed = 5;
            }

            if (lap == 3)
            {
                current_speed = 6;
            }

            turbo_active = check_zone_collision(
                car,
                turbo_zones,
                NUMBER_OF_ZONES
            );

            slow_active = check_zone_collision(
                car,
                slow_zones,
                NUMBER_OF_ZONES
            );

            if (turbo_active)
            {
                current_speed = current_speed + 3;
            }

            if (slow_active)
            {
                current_speed = current_speed - 2;
            }

            if (current_speed < 2)
            {
                current_speed = 2;
            }

            car.speed = current_speed;

            move_car(car);

            check_coin_collision(
                car,
                coins,
                NUMBER_OF_COINS
            );

            if (check_obstacle_collision(
                    car,
                    obstacles,
                    NUMBER_OF_OBSTACLES))
            {
                damage_car(car);

                car.score = car.score - 5;

                if (car.score < 0)
                {
                    car.score = 0;
                }
            }

            if (current_lap_count < MAX_GHOST_POSITIONS)
            {
                current_lap_positions[current_lap_count].x = car.x;
                current_lap_positions[current_lap_count].y = car.y;
                current_lap_count++;
            }

            if (car.y < 100)
            {
                if (current_lap_count > 0)
                {
                    ghost_count = current_lap_count;

                    if (ghost_count > MAX_GHOST_POSITIONS)
                    {
                        ghost_count = MAX_GHOST_POSITIONS;
                    }

                    for (int i = 0; i < ghost_count; i++)
                    {
                        ghost_positions[i] =
                            current_lap_positions[i];
                    }

                    ghost_index = 0;
                }

                if (lap == 1)
                {
                    best_lap = 60 - time_left;
                }

                lap = lap + 1;

                car.y = 500;

                current_lap_count = 0;

                car.score = car.score + 25;

                shift_track(
                    obstacles,
                    NUMBER_OF_OBSTACLES,
                    lap
                );

                if (lap > 3)
                {
                    game_state = 3;
                }
            }

            if (timer_ticks("GameTimer") >= 1000)
            {
                time_left = time_left - 1;

                reset_timer("GameTimer");

                if (time_left <= 0)
                {
                    game_state = 2;
                }
            }

            if (car.energy <= 0)
            {
                game_state = 2;
            }

            draw_track();

            draw_zones(
                turbo_zones,
                slow_zones,
                NUMBER_OF_ZONES
            );

            draw_obstacles(
                obstacles,
                NUMBER_OF_OBSTACLES
            );

            draw_coins(
                coins,
                NUMBER_OF_COINS
            );

            draw_ghost(
                ghost_positions,
                ghost_count,
                ghost_index
            );

            draw_car(car);

            draw_information(
                car,
                lap,
                time_left,
                current_speed
            );

            if (turbo_active)
            {
                draw_text(
                    "TURBO!",
                    COLOR_CYAN,
                    420,
                    45
                );
            }

            if (slow_active)
            {
                draw_text(
                    "SLOW!",
                    COLOR_YELLOW,
                    420,
                    45
                );
            }
        }
        else if (game_state == 2)
        {
            draw_game_over_screen(car);

            if (key_typed(SPACE_KEY))
            {
                reset_game(
                    car,
                    obstacles,
                    NUMBER_OF_OBSTACLES,
                    coins,
                    NUMBER_OF_COINS,
                    turbo_zones,
                    slow_zones,
                    NUMBER_OF_ZONES
                );

                lap = 1;
                time_left = 60;
                ghost_count = 0;
                ghost_index = 0;
                current_lap_count = 0;
                turbo_active = false;
                slow_active = false;

                reset_timer("GameTimer");

                game_state = 1;
            }
        }
        else if (game_state == 3)
        {
            draw_winner_screen(
                car,
                best_lap
            );

            if (key_typed(SPACE_KEY))
            {
                reset_game(
                    car,
                    obstacles,
                    NUMBER_OF_OBSTACLES,
                    coins,
                    NUMBER_OF_COINS,
                    turbo_zones,
                    slow_zones,
                    NUMBER_OF_ZONES
                );

                lap = 1;
                time_left = 60;
                ghost_count = 0;
                ghost_index = 0;
                current_lap_count = 0;
                turbo_active = false;
                slow_active = false;

                reset_timer("GameTimer");

                game_state = 1;
            }
        }

        refresh_screen(60);
    }

    close_window("Adaptive Racing Game");

    return 0;
}