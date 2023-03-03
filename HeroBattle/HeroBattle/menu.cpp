#include "menu.h"


void CreateMenu()
{
    system("cls");
    cout << "----- Hero Creator -----" << endl;
    cout << "   [1] - Start Create Hero " << endl;
    cout << "   [0] - End Create Hero   " << endl;
    cout << "------------------------" << endl;
}
void magicMenu()
{
    system("cls");
    cout << "----- Choose Magic -----" << endl;
    cout << "  [1] - Block      " << endl;
    cout << "  [2] - Healing    " << endl;
    cout << "  [3] - Attack     " << endl;
    cout << "------------------------" << endl;
}
void punchMenu()
{
    system("cls");
    cout << "----- Choose Punch -----" << endl;
    cout << "  [1] - Range     " << endl;
    cout << "  [2] - Melee     " << endl;
    cout << "------------------------" << endl;
}
void skillMenu()
{
    system("cls");
    cout << "----- Choose Skill -----" << endl;
    cout << "  [1] - Dodge     " << endl;
    cout << "  [2] - ContrHit  " << endl;
    cout << "------------------------" << endl;
}

int RandomNumber(int end_range) {
    srand(time(0)); return rand() % end_range + 1;
}
