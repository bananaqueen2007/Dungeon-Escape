#include <iostream>
#include <windows.h>
#include <ctime>
#include "GameManager.h"
using namespace std;

int main()
{
    srand((unsigned)time(nullptr));
    GameManager game;
    game.showMainMenu();
    cout << "\n程序结束，按回车关闭窗口...";
    cin.get();
    return 0;
}