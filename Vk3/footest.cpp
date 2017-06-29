#include "Source\Headers\ManagerIF.hpp"

#include <iostream>
#include <stdio.h>
#include <climits>
#include <string>
#include <iomanip>

int main() {

    ManagerIF test;
    Manager t;
    Filter f;
    User alice(0, "Alice Error");
    User bob(1, "Bob Coding");
    User charlie(2, "Charlie Warning");
    ManagerAC::AddTargetUser(t, alice);
    ManagerAC::AddTargetUser(t, bob);
    ManagerAC::AddTargetUser(t, charlie);
    t.ROOT_USER_NAME = "BUGAGA";
    t.CONNECTION = "Brreak";
    test.Update_n_Show(t, f);

    system("pause");
    return 0;
}
