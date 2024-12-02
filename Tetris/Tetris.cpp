#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <filesystem>

void gotoxy(int x, int y) {
    COORD p = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

void SetColor(int background, int text) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (background << 4) | text);
}

void Intro() {
    std::cout << "     *************      *********      *************      **********      ************      " << std::endl;
    std::cout << "     *************      *********      *************      **********      ************      ****" << std::endl;
    std::cout << "         *****          ****               *****          **         **       ****         * " << std::endl;
    std::cout << "         *****          *********          *****          **         **       ****         * " << std::endl;
    std::cout << "         *****          *********          *****          ***********         ****          ****" << std::endl;
    std::cout << "         *****          ****               *****          **     **           ****              *" << std::endl;
    std::cout << "         *****          *********          *****          **       **     ************          *" << std::endl;
    std::cout << "         *****          *********          *****          **        **    ************      ****" << std::endl;
}

struct Point {
public:
    int x;
    int y;
};

class Figure {
private:
    std::vector<Point> Vec;
    std::vector<Point> Temp;

    const int BORDER = 1;
    const int BLOCK = 2;

    int dfg = 0;

    int kind = 0;

public:

    int getX(int i) {
        return Vec[i].x;
    }

    int getY(int i) {
        return Vec[i].y;
    }

    int getVecSize() {
        return Vec.size();
    }

    void Save() {
        Temp = Vec;
    }

    void Undo() {
        Vec = Temp;
    }

    void Push(int x, int y) {
        Point temp;
        temp.x = x;
        temp.y = y;
        Vec.push_back(temp);
    }

    void Show() {
        if (kind == 0) {
            SetColor(0, 9);
        }
        if (kind == 1) {
            SetColor(0, 10);
        }
        if (kind == 2) {
            SetColor(0, 4);
        }
        if (kind == 3) {
            SetColor(0, 6);
        }
        if (kind == 4) {
            SetColor(0, 7);
        }
        if (kind == 5) {
            SetColor(0, 8);
        }
        if (kind == 6) {
            SetColor(0, 9 );
        }
        for (int i = 0; i < Vec.size(); ++i) {
            gotoxy(Vec[i].x, Vec[i].y);
            std::cout << char(178);
        }
        SetColor(0, 7);
    }

    void Move_Down() {
        for (int i = 0; i < Vec.size(); ++i) {
            ++Vec[i].y;
        }
    }

    void Move_Up() {
        for (int i = 0; i < Vec.size(); ++i) {
            --Vec[i].y;
        }
    }

    void Move_Left() {
        for (int i = 0; i < Vec.size(); ++i) {
            --Vec[i].x;
        }
    }

    void Move_Right() {
        for (int i = 0; i < Vec.size(); ++i) {
            ++Vec[i].x;
        }
    }

    void Hide() {
        for (int i = 0; i < Vec.size(); ++i) {
            gotoxy(Vec[i].x, Vec[i].y);
            std::cout << ' ';
        }
    }

    void spawn() {
        Vec.clear();
        kind = rand() % 7;

        switch (kind) {
        case 0: // O-Tetromino
            Push(4, 0);
            Push(5, 0);
            Push(4, 1);
            Push(5, 1);
            break;
        case 1: // I-Tetromino
            Push(3, 0);
            Push(4, 0);
            Push(5, 0);
            Push(6, 0);
            break;
        case 2: // T-Tetromino
            Push(4, 1);
            Push(3, 0);
            Push(4, 0);
            Push(5, 0);
            break;
        case 3: // S-Tetromino
            Push(4, 1);
            Push(5, 1);
            Push(3, 0);
            Push(4, 0);
            break;
        case 4: // Z-Tetromino
            Push(3, 1);
            Push(4, 1);
            Push(4, 0);
            Push(5, 0);
            break;
        case 5: // J-Tetromino
            Push(3, 1);
            Push(3, 0);
            Push(4, 0);
            Push(5, 0);
            break;
        case 6: // L-Tetromino
            Push(5, 1);
            Push(3, 0);
            Push(4, 0);
            Push(5, 0);
            break;
        }

    }

    void Rotate() {

        for (int i = 0; i < Vec.size(); ++i) {
            int x = -(Vec[i].y - Vec[1].y) + Vec[1].x;
            int y = (Vec[i].x - Vec[1].x) + Vec[1].y;

            Vec[i].x = x;
            Vec[i].y = y;
        }
    }

    void RotateRevers() {
        for (int i = 0; i < Vec.size(); ++i) {
            int x = (Vec[i].y - Vec[1].y) + Vec[1].x;
            int y = -(Vec[i].x - Vec[1].x) + Vec[1].y;

            Vec[i].x = x;
            Vec[i].y = y;
        }
    }

    bool allowMode() {
        if (dfg > 10) {
            dfg = 0;
            return true;
        }
        else {
            ++dfg;
            return false;
        }

    }
};

class Map {
private:

    const int EMPTY = 0;
    const int BORDER = 1;
    const int BLOCK = 2;

    const int BLOCK_IN_LINE = 10;

    std::vector<std::vector<int>> Vec1;
public:

    void init(int width, int height) {
        std::vector<int> temp;
        temp.push_back(BORDER);
        for (int i = 1; i < width - 1; ++i)
            temp.push_back(EMPTY);
        temp.push_back(BORDER);

        for (int j = 0; j < height - 1; ++j)
            Vec1.push_back(temp);

        temp.clear();
        for (int v = 0; v < width; ++v)
            temp.push_back(BORDER);

        Vec1.push_back(temp);
    }

    void ShowField() {

        gotoxy(0, 0);
        for (int i = 0; i < Vec1.size(); ++i) {
            for (int j = 0; j < Vec1[i].size(); ++j) {
                if (Vec1[i][j] == BORDER) {
                    std::cout << "#";
                }

                if (Vec1[i][j] == BLOCK) {
                    std::cout << "*";
                }

                if (Vec1[i][j] == EMPTY) {
                    std::cout << " ";
                }
            }
            std::cout << std::endl;
        }
    }

    void AnimationDeleteLines(std::vector<int> lines) {
        if (lines.empty()) return;

        for (int i = 1; i <= 10; ++i) {
            for (int j = 0; j < lines.size(); ++j) {
                gotoxy(i, lines[j]);
                std::cout << " ";
            }
            Sleep(100);
        }
    }

    int DeleteLines(int width) {
        std::vector<int> lines;

        for (int i = Vec1.size() - 2; i >= 0; --i) {
            int d = 0;
            for (int j = 1; j < Vec1[i].size() - 1; ++j) {
                if (Vec1[i][j] == BLOCK) {
                    ++d;
                }
                else {
                    break;
                }
            }
            if (d == BLOCK_IN_LINE) {
                lines.push_back(i);
            }
        }

        AnimationDeleteLines(lines);
        for (int i = 0; i < lines.size(); ++i) {
            Vec1.erase(Vec1.begin() + lines[i]);
        }



        std::vector<int> temp;
        temp.push_back(BORDER);
        for (int i = 1; i < width - 1; ++i)
            temp.push_back(EMPTY);
        temp.push_back(BORDER);

        for (int j = 0; j < lines.size(); ++j)
            Vec1.insert(Vec1.begin(), temp);

        return lines.size();
    }


    bool isCollision(Figure& fig) {
        for (int i = 0; i < fig.getVecSize(); ++i) {

            if (fig.getY(i) < 0) {
                return true;
            }

            if (fig.getX(i) <= 0) {
                return true;
            }

            if (fig.getX(i) >= (10 + 2) - 1) {
                return true;
            }

            if (Vec1[fig.getY(i)][fig.getX(i)] == BORDER) {
                return true;
            }

            if (Vec1[fig.getY(i)][fig.getX(i)] == BLOCK) {
                return true;
            }
        }
        return false;
    }

    bool ShiftLeftFig(Figure& fig) {
        if (isCollision(fig)) {
            fig.Move_Left();
            if (isCollision(fig)) {
                fig.Move_Left();
                if (isCollision(fig)) {
                    fig.Undo();
                    return false;
                }
            }
        }
        return true;
    }

    bool ShiftRightFig(Figure& fig) {
        if (isCollision(fig)) {
            fig.Move_Right();
            if (isCollision(fig)) {
                fig.Move_Right();
                if (isCollision(fig)) {
                    fig.Undo();
                    return false;
                }
            }
        }
        return true;
    }

    void Fixed(Figure& fig) {
        for (int i = 0; i < fig.getVecSize(); ++i) {
            Vec1[fig.getY(i)][fig.getX(i)] = 2;
        }
    }
};

class Game {
private:
    const int HEIGHT = 20 + 1;
    const int WIDTH = 10 + 2;

    int score;

    bool gameOver = false;

    std::vector<std::pair<std::string, int>> Gamer;


public:
    Game() {
        
    }

    void SaveRecord() {
        std::ifstream fin("Save.bin", std::ios_base::binary);
        if (!fin) {
            std::cerr << "Eroare la deschiderea fișierului pentru citire!" << std::endl;
            return;
        }

        Gamer.clear(); // Curățăm vectorul pentru a evita duplicarea datelor

        // Citim perechile de date din fișier
        while (fin.peek() != EOF) {
            int length;
            fin.read(reinterpret_cast<char*>(&length), sizeof(length)); // Citim lungimea numelui

            if (length <= 0 || length > 1000) { // Verificăm dacă lungimea este validă
                std::cerr << "Eroare: Lungime invalidă a șirului!" << std::endl;
                break;
            }

            std::string name(length, ' '); // Creăm șirul cu lungimea specificată
            fin.read(&name[0], length);    // Citim conținutul numelui

            int score;
            fin.read(reinterpret_cast<char*>(&score), sizeof(score)); // Citim scorul

            Gamer.emplace_back(name, score); // Adăugăm perechea în vectorul Gamer
        }

        fin.close();

        // Sortăm vectorul după scoruri, în ordine descrescătoare
        std::sort(Gamer.begin(), Gamer.end(), [](const auto& left, const auto& right) {
            return left.second > right.second;
            });

        // Afișăm tabela de lideri
        gotoxy(30, 1);
        std::cout << "Leaderboard: \n";
        for (size_t i = 0; i < Gamer.size(); ++i) {
            gotoxy(30, 2+i);
            std::cout << i + 1 << ". " << Gamer[i].first << " " << Gamer[i].second << std::endl;
        }
    }


    bool fileExists(const std::string& filePath) {
        std::ifstream file(filePath);
        return file.good();
    }

    void SaveRecord(std::string nickname, int score) {
        gotoxy(60, 4);
        std::cout << "Symbols limit 10";
        /*gotoxy(60, 5);
        std::cout << "Please enter your nickname: ";
        std::cin >> nickname;*/

        bool limit = true;
        while (limit) {
            gotoxy(60, 5);
            std::cout << "Please enter your nickname: ";
            std::cin >> nickname;

            gotoxy(60, 6);
            if (nickname.size() <= 10) limit = false;
            if (nickname.size() > 10) {
                std::cout << "LIMITS";

                for (int i = 0; i < nickname.size(); ++i) {
                    gotoxy(88 + i, 5);
                    std::cout << ' ';
                }
            }
        }

        for (int i = 0; i < 6; ++i) {
            gotoxy(60 + i, 6);
            std::cout << ' ';
        }

        for (int i = 0; i < 16; ++i) {
            gotoxy(60 + i, 4);
            std::cout << ' ';
        }

        bool status = true;

        for (int i = 0; i < Gamer.size(); ++i) {
            if (Gamer[i].first == nickname) {
                if (score > Gamer[i].second) {
                    Gamer[i].second = 0;
                    Gamer[i].second = score;
               }              
                status = false;
            }
        }

        if (status) {
            Gamer.push_back(std::make_pair(nickname, score));

            
       }
        // Sortare vector Gamer după scor în ordine descrescătoare
        std::sort(Gamer.begin(), Gamer.end(), [](auto& left, auto& right) {
            return left.second > right.second;
            });


        gotoxy(30, 1);
        std::cout << "Leaderboard: " << std::endl;

        // Afișarea liderilor
        for (int i = 0; i < Gamer.size(); ++i) {
            gotoxy(30, i + 2);
            std::cout << i + 1 << ". " << Gamer[i].first << " " << Gamer[i].second << std::endl;
        }

        // Deschidem fișierul pentru scriere în mod binar
        std::ofstream fout("Save.bin", std::ios_base::binary);

        // Scrierea datelor în fișier
        for (const auto& gamer : Gamer) {

            int length = gamer.first.size();
            fout.write(reinterpret_cast<const char*>(&length), sizeof(length)); // Scriem lungimea numelui
            fout.write(gamer.first.c_str(), length);                           // Scriem numele
            fout.write(reinterpret_cast<const char*>(&gamer.second), sizeof(gamer.second)); // Scriem scorul
        }

        fout.close();

        for (int i = 0; i < 27 + nickname.size() + 1; ++i) {
            gotoxy(60 + i, 5);
            std::cout << ' ';
        }
       
    }

    void ResetGame() {
        gameOver = false;
        score -= score;

        for (int i = 0; i < 8 + std::to_string(score).size(); ++i) {
            gotoxy(15 + i, 1);
            std::cout << ' ';
        }
    }

    int GameOver() {
        gotoxy(60, 5);
        SetColor(0, 4);
        std::cout << "GAME OVER" << std::endl;
        gotoxy(50, 6);
        SetColor(0, 7);
        std::cout << "Click \" Enter \" for restart";

        gameOver = true;

        return 0;
    }

    void HideCursor() {
        void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO structCursorInfo;
        GetConsoleCursorInfo(handle, &structCursorInfo);
        structCursorInfo.bVisible = false;
        SetConsoleCursorInfo(handle, &structCursorInfo);
    }

    void incrimentScore(int lines) {

        switch (lines) {
        case 1: score += 1;
            break;
        case 2: score += 3;
            break;
        case 3: score += 6;
            break;
        case 4: score += 12;
            break;
        }

    }

    void ShowScore() {

        gotoxy(15, 1);
        std::cout << "SCORE: " << score << std::endl;
    }

    int Run() {

        Map map;

        map.init(WIDTH, HEIGHT);
        map.ShowField();

        ShowScore();

        Figure fig;
        fig.spawn();

        int ch = 0;
        int i = 0;

        int X = 5, Y = 5;

        //SaveRecord();
        while (true) {

            while (!_kbhit()) {
                if (gameOver) continue;

                fig.Hide();
                if (fig.allowMode()) fig.Move_Down();

                if (map.isCollision(fig)) {
                    fig.Move_Up();
                    map.Fixed(fig);
                    fig.Show();

                    ShowScore();
                    int lines = map.DeleteLines(WIDTH);
                    incrimentScore(lines);
                    ShowScore();


                    map.DeleteLines(WIDTH);
                    map.ShowField();
                    fig.spawn();

                    if (map.isCollision(fig)) {
                        fig.Show();
                        map.Fixed(fig);
                        this->GameOver();

                        break;
                    }
                }
                fig.Show();
                Sleep(20);
            }

            int ch = _getch();
            if (gameOver) {
                std::string NickName;
                if (ch == 13) {
                    for (int i = 0; i < 9; ++i) {
                        gotoxy(60 + i, 5);
                        std::cout << ' ';
                    }

                    for (int i = 0; i < 27; ++i) {
                        gotoxy(50 + i, 6);
                        std::cout << ' ';
                    }


                    SaveRecord(NickName, score);
                    
                    this->ResetGame();
                    this->Run();


                }
            }
            if (!gameOver) {
                switch (ch) {
                case 75:
                    fig.Hide();
                    fig.Move_Left();

                    if (map.isCollision(fig)) {
                        fig.Move_Right();
                    }
                    break;
                case 77:
                    fig.Hide();
                    fig.Move_Right();

                    if (map.isCollision(fig)) {
                        fig.Move_Left();
                    }
                    break;
                case 80:
                    fig.Hide();
                    fig.Move_Down();

                    if (map.isCollision(fig)) {
                        fig.Move_Up();
                    }
                    break;
                case 72:
                    fig.Hide();
                    fig.Save();
                    fig.Rotate();

                    if (!map.ShiftLeftFig(fig)) {
                        fig.Undo();
                        fig.Rotate();
                        if (!map.ShiftRightFig(fig)) {
                            fig.Undo();
                        }
                    }
                    break;
                case 32:
                    fig.Hide();
                    fig.Save();
                    fig.RotateRevers();

                    if (!map.ShiftRightFig(fig)) {
                        fig.Undo();
                        fig.RotateRevers();
                        if (!map.ShiftLeftFig(fig)) {
                            fig.Undo();
                        }
                    }
                    break;
                }
            }


        }
        return 0;
    }
};

int main()
{
    Game game;

    HWND hWindowConsole = GetConsoleWindow();
    RECT r;
    GetWindowRect(hWindowConsole, &r);
    MoveWindow(hWindowConsole, r.left, r.top, 1000, 600, TRUE);

    Intro();
    //gotoxy(20, 5);

    Sleep(5000);
    system("cls");
    gotoxy(0, 0);

    game.SaveRecord();
    game.HideCursor();
    game.Run();




}