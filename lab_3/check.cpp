#include <iostream>
#include <Windows.h>
#include <clocale>

using namespace std;

int main(){
    setlocale(LC_CTYPE, "rus");
    SetConsoleCP(1251);// ��������� ������� �������� win-cp 1251 � ����� �����
    SetConsoleOutputCP(1251);
    cout << atof("1.01");
}
