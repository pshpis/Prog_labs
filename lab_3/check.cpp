#include <iostream>
#include <Windows.h>
#include <clocale>

using namespace std;

int main(){
    setlocale(LC_CTYPE, "rus");
    SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
    SetConsoleOutputCP(1251);
    cout << atof("1.01");
}
