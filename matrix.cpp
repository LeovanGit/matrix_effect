#include <iostream>
#include <ncurses.h>

#define lines_count 50 // 50
#define general_slowdown 1 // 1
#define color_count 4

const char symbols[] = "1QW2ER3TY4UI5IO6P[7AS8DF]9FG0HJK/LNCXZ";
const int symbols_len = sizeof(symbols) - 1;
int win_height = -1, win_width = -1;

inline int rand_from_range(int a, int b)
{
    static const double r = 1.0 / RAND_MAX; 
    return int(a + (r * rand()) * (b - a));
}

class Line
{
public: 
    Line() : x(rand_from_range(0, win_width)), 
             y(0), 
             speed(rand_from_range(1, 3)),
             len(rand_from_range(win_height / 4, 3 * win_height / 4)) {}

    void Move()
    {
        if (y >= win_height + len)
        {
            x = rand_from_range(0, win_width);
            y = 0;
            speed = rand_from_range(1, 3);
            len = rand_from_range(win_height / 4, 3 * win_height / 4);
        } else y += speed;
    }

    void Draw()
    {     
        // this func draw symbol only if it less bright than symbol in current cell

        // main (which near white) symbol
        if (get_symbol_brightness(y, x) > 5) Draw_rand_symbol(y, x, 5);

        // other symbols
        int last_y = y - len + 1;

        for (int i = last_y; i != y; ++i)
        {
            // [1; 4] depends on symbol's distance to main symbol
            int color = 1 + (double(i - last_y) / (len - 1)) * color_count;
            if (get_symbol_brightness(i, x) > color) Draw_rand_symbol(i, x, color);
        }

        // remove last (amount of "speed") symbols
        for (int i = last_y, end = last_y + speed; i != end; ++i) mvaddch(i, x, ' ');

        // draw all changes from buffer to real screen
        refresh();
        napms(general_slowdown);
    }

private:
    int x;
    int y; // position of first symbol
    int speed; // how many characters move per iteration
    int len;

    void Draw_rand_symbol(int y, int x, int color)
    {
        attron(COLOR_PAIR(color));
        mvaddch(y, x, symbols[rand_from_range(0, symbols_len)]);
        attroff(COLOR_PAIR(color));
    }       

    int get_symbol_brightness(int y, int x)
    {
        int color = mvinch(y, x) & A_COLOR;
        switch (color)
        {
            case COLOR_PAIR(1): return 1;
            case COLOR_PAIR(2): return 2;
            case COLOR_PAIR(3): return 3;
            case COLOR_PAIR(4): return 4;
            case COLOR_PAIR(5): return 5;
            default: return 6; // 6 - empty cell
        }
    }
};

int main()
{
    initscr();
    noecho();
    curs_set(0);
    getmaxyx(stdscr, win_height, win_width);

    srand(time(nullptr));

    start_color();    
    init_pair(1, 22, COLOR_BLACK);
    init_pair(2, 28, COLOR_BLACK);
    init_pair(3, 34, COLOR_BLACK);
    init_pair(4, 40, COLOR_BLACK);
    init_pair(5, 157, COLOR_BLACK);

    Line lines[lines_count];

    while(true)
    {
        for (int i = 0; i != lines_count; ++i)
        {
                lines[i].Draw();
                lines[i].Move();
        }
    }

    endwin();
    return 0;
}
