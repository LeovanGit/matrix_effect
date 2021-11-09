#include <iostream>
#include <ncurses.h>
#include <vector>

const std::string symbols = "1QW2ER3TY4UI5IO6P[7AS8DF]9FG0HJK/LNCXZ";
const int symbols_len = symbols.size();
int height = -1, width = -1;

class Line
{
public:
    int x;
    int y = 0;
    int speed = 75;
    
    void Move()
    {
        y = ((y >= height + height/2) ? 0 : y+1);
    }

    void Draw()
    {
        // remove old symbol, max length of line = height/2
        if (y >= height/2) mvaddch(y - height/2, x, ' ');
        
        // recolor to green previous symbol
        if (y > 0 && y <= height)
        {
            attron(COLOR_PAIR(1));
            mvaddch(y-1, x, mvinch(y-1, x));
            attroff(COLOR_PAIR(1));
        }

        // draw main, white symbol
        if (y < height) mvaddch(y, x, symbols[rand() % symbols_len]);

        // draw all changes from buffer to real screen
        refresh();
        napms(speed);
    }
};

int main()
{
//    std::vector <Line> lines(10);

    initscr();
    curs_set(0);

    Line line;

    getmaxyx(stdscr, height, width);

    line.x = rand() % width; // [0; width]
    
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    while(true)
    {

        line.Draw();
        line.Move();

    }
    getch();

    endwin();

    return 0;
}
