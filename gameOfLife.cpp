#include<cstdlib>
#include<ctime>
#include<iostream>
#include<windows.h>

class game{
    public:
    game(int _height = 10, int _width = 10){
        stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_CURSOR_INFO cursor;
        GetConsoleCursorInfo(stdHandle, &cursor);
        cursor.bVisible = false;
        SetConsoleCursorInfo(stdHandle, &cursor);

        if (_height < 10) _height = 10;
        if (_width < 10)  _width  = 10;

        height = _height;
        width  = _width;

        beings      = new std::string[height];
        beingsCopy  = new std::string[height];

        fill();

        copy(beingsCopy, beings);
    }
    ~game(){
        delete[] beings;
        delete[] beingsCopy;
    }

    void loop(){
        for (int i = 0; i < height; ++i){
            for (int j = 0; j < width; ++j){
                beingsCopy[i][j] = evaluate(i, j) ? '1' : ' ';
            }
        }

        copy(beings, beingsCopy);

        print();
    }

    private:
    std::string* beings;
    std::string* beingsCopy;
    int height, width;
    HANDLE stdHandle;

    void fill(){
        short random;

        for (int i = 0; i < height; ++i){
            for (int j = 0; j < width; ++j){
                random = rand() % 2;
                beings[i] += (random) ? '1' : ' ';
            }
        }

        for (int i = 0; i < height; ++i){
            SetConsoleCursorPosition(stdHandle, {(short)width, (short)i});
            std::cout.put('|');
        }
        std::cout<<std::endl;

        for (int i = 0; i < width; ++i) std::cout.put('-');
    }

    void print(){
        SetConsoleCursorPosition(stdHandle, {0, 0});
        
        for (int i = 0; i < height; ++i) std::cout<<beings[i]<<std::endl;
    }

    bool evaluate(int _i, int _j){
        int alive = 0;

        int i = _i - 1, j = _j - 1;

        for (int k = 0; k < 2; ++k){
            if (i >= 0 && j + k >= 0){
                if (beings[i][j + k] == '1') ++alive;
            }
            if (j + 2 < width && i + k >= 0){
                if (beings[i + k][j + 2] == '1') ++alive;
            }
            if (i + 2 < height && j + 2 - k < width){
                if (beings[i + 2][j + 2 - k] == '1') ++alive;
            }
            if (i + 2 - k < height && j >= 0){
                if (beings[i + 2 - k][j] == '1') ++alive;
            }
        }

        if (beings[_i][_j] == '1')
            return alive >= 2 && alive <= 3;

        return alive == 3;
    }

    void copy(std::string beings[], std::string beingsCopy[]){
        for (int i = 0; i < height; ++i) beings[i] = beingsCopy[i];
    }
};

int main(){
    srand(time(0));

    int n1, n2;


    std::cout<<"Sim. height: "; std::cin>>n1;
    std::cout<<"Sim. width: "; std::cin>>n2;

    system("cls");

    game g(n1, n2);

    do{
        Sleep(10);
        g.loop();
    }while(!GetAsyncKeyState(VK_ESCAPE));

    return 0;
}