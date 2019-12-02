#include <iostream>
#include <cstdlib>
#include "crnepl.h"
#include <cstring>

using namespace std;

int main()
{
    crnepl::Crnepl rpl;
    char buf[1024];
    while (1)
    {
        rpl.LoopOnce(buf);
        if (strcmp(buf,"quit")==0)
        {
            break;
        }
        else
        {
            system(buf);
        }
    }
}
