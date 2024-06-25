#include "Header.h"

void gotoxy(int x, int y) {
    // Get the console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Create a COORD structure and set the coordinates
    COORD pos;
    pos.X = x;
    pos.Y = y;

    // Set the console cursor position
    SetConsoleCursorPosition(hConsole, pos);
}

void ShowConsoleCursor(bool showFlag) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
struct segment {
    int x, y;
    int shape = 111;
};

class player {
private:
	int length;
    vector<segment> s;
    int facing; 
    // 0: left
    // 1: right
    // 2: up
    // 3: down
public:
    player() {
        length = 3;
        s.resize(3);
        s[0] = { 50, 20 };
        s[1] = { 51, 20 };
        s[2] = { 52, 20 };
        facing = 0;
    }

    int getX() {
        return s[0].x;
    }
    int getY() {
        return s[0].y;
    }
    int getFacing() {
        return facing;
    }
    void print() {
        gotoxy(s[0].x, s[0].y);
        cout << char(s[0].shape);
    }
    void updatePos() {
        gotoxy(s[length - 1].x, s[length - 1].y);
        cout << " ";
        for (int i = length-1; i >=1; i--) {
            s[i] = s[i - 1];
        }
    }
    void move() {
        if (_kbhit()) {
            // Lấy phím được nhấn
            char ch = _getch();
            // Kiểm tra nếu phím 'A' được nhấn
            if (ch == 'A' || ch == 'a') {
                if (facing == 2 || facing == 3)
                {
                    facing = 0;
                    updatePos();
                    s[0].x -= 1;
                }
            }
            else if (ch == 'D' || ch == 'd') {
                if (facing == 2 || facing == 3)
                {
                    facing = 1;
                    updatePos();
                    s[0].x += 1;
                }
            }
            else if (ch == 'W' || ch == 'w') {
                if (facing == 0 || facing == 1)
                {
                    facing = 2;
                    updatePos();
                    s[0].y--;
                }
            }
            else if (ch == 'S' || ch == 's') {
                if (facing == 0 || facing == 1)
                {
                    facing = 3;
                    updatePos();
                    s[0].y++;
                }
            }
        }
        else {
            if (facing == 0) 
            {
                updatePos();
                s[0].x -= 1;
            }
            if (facing == 1) 
            {
                updatePos();
                s[0].x += 1;
            }
            if (facing == 2) 
            {
                updatePos();
                s[0].y--;
            }
            if (facing == 3) 
            {
                updatePos();
                s[0].y++;
            }
        }
    }
    bool collide() {
        if(s[0].x >= WIDTH || s[0].x <= 0)
            return true;
        if (s[0].y >= HEIGHT || s[0].y <= 0)
            return true;
        return false;
    }
    bool selfCollide() {
        for (int i = 4; i < length; i++) {
            if (s[i].x == s[0].x)
                if (s[i].y == s[0].y)
                    return true;
        }
        return false;
    }
    void eatFood() {
        length++;
        segment new_head{ s[0].x,s[0].y,111 };
        if (facing == 0)
            new_head.x--;
        else if (facing == 1)
            new_head.x++;
        else if (facing == 2)
            new_head.y--;
        else if (facing == 3)
            new_head.y++;
        s.insert(s.begin(), new_head);
    }
};

class food {
private: 
	int x, y;
    int shape = 4;
public:
    int getX() { return x; }
    int getY() { return y; }
    food() {
        x = 40;
        y = 10;
    }
	void getEaten() {
        x = y = 0;
        shape = 0;
        thread([&]() {
            this_thread::sleep_for(chrono::seconds(2));
            shape = 4;
            x = 40, y = 10;
        }).detach();
	}
	void print() {
        gotoxy(x, y);
        cout << char(shape);
	}
};

class score_board {
private:
    int x, y;
    int score;
public:
    score_board() {
        x = 10;
        y = 2;
        score = 0;
    }
    void print() {
        gotoxy(x, y);
        cout << "Score: " << score;
    }
    void plus() {
        score++;
    }
    int getScore() {
        return score;
    }
};

class main_game {
private:
    player p;
    food f;
    score_board s;
public:
    bool checkEatFood() {
        if (p.getX() == f.getX() && p.getY() == f.getY())
        {
            return true;
        }
        return false;
    }
    void startGame() {
        p.print();
        f.print();
        s.print();
    }
    void gameOver() {
        system("cls");
        gotoxy(40, 10);
        cout << "Game Over";
    }
    void playing() {
        int delay = 100;
        int facing = p.getFacing();
        while (true) {
            p.move();
            f.print();
            p.print();
            s.print();
            
            facing = p.getFacing();
            if (facing == 0 || facing == 1)
                delay = 50;
            else
                delay = 100;
            if (p.collide() || p.selfCollide()) {
                gameOver();
                return;
            }
            if (checkEatFood()) {
                s.plus();
                f.getEaten();
                p.eatFood();
            }
            delay -= s.getScore();
            Sleep(delay);
        }
    }
    
};

static BOOL SetConsoleSize(int cols, int rows) {
    HWND hWnd;
    HANDLE hConOut;
    CONSOLE_FONT_INFO fi;
    CONSOLE_SCREEN_BUFFER_INFO bi;
    int w, h, bw, bh;
    RECT rect = { 0, 0, 0, 0 };
    COORD coord = { 0, 0 };
    hWnd = GetConsoleWindow();
    if (hWnd) {
        hConOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConOut && hConOut != (HANDLE)-1) {
            if (GetCurrentConsoleFont(hConOut, FALSE, &fi)) {
                if (GetClientRect(hWnd, &rect)) {
                    w = rect.right - rect.left;
                    h = rect.bottom - rect.top;
                    if (GetWindowRect(hWnd, &rect)) {
                        bw = rect.right - rect.left - w;
                        bh = rect.bottom - rect.top - h;
                        if (GetConsoleScreenBufferInfo(hConOut, &bi)) {
                            coord.X = bi.dwSize.X;
                            coord.Y = bi.dwSize.Y;
                            if (coord.X < cols || coord.Y < rows) {
                                if (coord.X < cols) {
                                    coord.X = cols;
                                }
                                if (coord.Y < rows) {
                                    coord.Y = rows;
                                }
                                if (!SetConsoleScreenBufferSize(hConOut, coord)) {
                                    return FALSE;
                                }
                            }
                            return SetWindowPos(hWnd, NULL, rect.left, rect.top, cols * fi.dwFontSize.X + bw, rows * fi.dwFontSize.Y + bh, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
                        }
                    }
                }
            }
        }
    }
    return FALSE;
}

int main() {
    ShowConsoleCursor(0);
	SetConsoleSize(WIDTH, HEIGHT);
    main_game game;
    game.startGame();
    game.playing();
	return 0;
}