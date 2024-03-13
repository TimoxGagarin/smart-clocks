#include <stdlib.h>
#include <signal.h>
#include <ncurses.h>
#include <math.h>
#include <time.h>

#define PI 3.141526535897

volatile sig_atomic_t quit_flag = 0;

void handle_sigint(int signum)
{
    quit_flag = 1;
}

void handle_sigsegv(int signum)
{
    refresh();
    endwin();
    puts("Ошибка сегментирования (образ памяти сброшен на диск)");
    exit(EXIT_FAILURE);
}

/**
 * @brief Инициализирует обработчики сигналов.
 */
void initSignalHandlers()
{
    signal(SIGINT, &handle_sigint);
    signal(SIGSEGV, &handle_sigsegv);
}

void init_clocks()
{
    initscr();
    noecho();

    // Отображение буфера на экране
    while (!quit_flag)
    {
        keypad(stdscr, TRUE);
        int rows, cols;
        getmaxyx(stdscr, rows, cols);
        char buffer[rows][cols];

        clear_clocks(cols, rows, buffer);
        draw_circle(cols, rows, buffer);
        show_clocks(cols, rows, buffer);
        write_clocks(cols, rows, buffer);
        refresh();
        sleep(1);
    }

    endwin();
}

void clear_clocks(int cols, int rows, char buffer[rows][cols])
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            buffer[i][j] = ' ';
}

void write_clocks(int cols, int rows, char buffer[rows][cols])
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            mvprintw(i, j, "%c", buffer[i][j]);
    refresh();
}

void draw_circle(int cols, int rows, char buffer[rows][cols])
{
    int rad = (cols < rows ? cols : rows) / 2;
    for (int i = 0; i < 360; i++)
    {
        int mod_x = 2 * (rad * cos(i)) + cols / 2;
        int mod_y = rad * sin(i) + rows / 2;
        buffer[mod_y][mod_x] = '*';
    }
}

void draw_line(int cols, int rows, char buffer[rows][cols], double angle, int rad, char num[2])
{
    double delta_x = 2 * (rad * cos(angle));
    double delta_y = rad * sin(angle);

    double min = abs(delta_x) > abs(delta_y) ? delta_x : delta_y;

    delta_x /= abs(min);
    delta_y /= abs(min);

    for (int i = 0; i < (int)abs(min); i++)
    {
        int x = cols / 2 + delta_x * i;
        int y = rows / 2 + delta_y * i;
        if (buffer[y][x] == ' ' && buffer[y][x + 1] == ' ' && buffer[y][x + 2] == ' ')
        {
            buffer[y][x] = num[0];
            buffer[y][x + 1] = num[1];
        }
    }
}

void show_clocks(int cols, int rows, char buffer[rows][cols])
{
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    int rad = (cols < rows ? cols : rows) / 2;

    int hours = localTime->tm_hour;
    int minutes = localTime->tm_min;
    int seconds = localTime->tm_sec;

    double seconds_angle = (double)seconds / 30 * PI;
    double minutes_angle = (double)minutes / 30 * PI + (double)seconds_angle / 60;
    double hours_angle = ((double)hours - 12) / 6 * PI + (double)minutes_angle / 60;

    char hours_str[2];
    char minutes_str[2];
    char seconds_str[2];

    sprintf(hours_str, "%d", hours);
    sprintf(minutes_str, "%d", minutes);
    sprintf(seconds_str, "%d", seconds);

    draw_line(cols, rows, buffer, seconds_angle + PI * 3 / 2, rad * 0.9, seconds_str);
    draw_line(cols, rows, buffer, minutes_angle + PI * 3 / 2, rad * 0.7, minutes_str);
    draw_line(cols, rows, buffer, hours_angle + PI * 3 / 2, rad * 0.5, hours_str);
}