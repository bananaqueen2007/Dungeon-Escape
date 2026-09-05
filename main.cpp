#include <iostream>
#include <windows.h>
#include <ctime>
#include "GameManager.h"

int main()
{

    srand((unsigned)time(nullptr));
    GameManager game;
    game.showMainMenu();
    std::cout << "\n程序结束，按回车关闭窗口...";
    std::cin.get();
    return 0;
}