/*
                  _        _      
  _ __ ___   __ _| |_ _ __(_)_  __
 | '_ ` _ \ / _` | __| '__| \ \/ /
 | | | | | | (_| | |_| |  | |>  < 
 |_| |_| |_|\__,_|\__|_|  |_/_/\_\
                                  
 by Leovan
*/

#include <iostream>
#include <ncurses.h>
#include <vector>

const std::string symbols = "1QW2ER3TY4UI5IO6P[7AS8DF]9FG0HJK/LNCXZ";
const int symbols_len = symbols.size();
const int frequency = 50;
int height = -1, width = -1;

class Line
{
private:
    int x;
    int y = 0;
    int speed = 1; 
    int len;

public: 
    void Set_random_x()
    {
        x = rand() % width; // [0; width]
    }

    void Set_random_len()
    {
        len = height/4 + (rand() % height/2); // [height/4; 3*height/4]
    }

    void Move()
    {
        if (y >= height + len)
        {
            y = 0;
            Set_random_x();
            Set_random_len();
        }
        else y++;
    }

    void Draw()
    {
        // remove old symbol, max length of line = height/2
        if (y >= len) mvaddch(y - len, x, ' ');
        
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
    initscr();
    curs_set(0);

    getmaxyx(stdscr, height, width);
    
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    attron(COLOR_PAIR(1));
    mvprintw(height/2-3, width/2-16,"                  _        _      ");
    mvprintw(height/2-2, width/2-16,"  _ __ ___   __ _| |_ _ __(_)_  __");
    mvprintw(height/2-1, width/2-16," | '_ ` _ \\ / _` | __| '__| \\ \\/ /");
    mvprintw(height/2,   width/2-16," | | | | | | (_| | |_| |  | |>  < ");
    mvprintw(height/2+1, width/2-16," |_| |_| |_|\\__,_|\\__|_|  |_/_/\\_\\");
    attroff(COLOR_PAIR(1));
    refresh();
    napms(500);

    std::vector <Line> lines(frequency);
    for (int i = 0, l = lines.size(); i < l; i++)
    {
        lines[i].Set_random_x();
        lines[i].Set_random_len();
    }

    while(true)
    {
        for (int i = 0, l = lines.size(); i < l; i++)
        {
            lines[i].Draw();
            lines[i].Move();
        }
    }

    endwin();

    return 0;
}
