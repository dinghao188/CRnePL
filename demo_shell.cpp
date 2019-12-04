#include <iostream>
#include <cstdlib>
#include <cstring>
#include "crnepl.hpp"

using namespace std;

int main()
{
    crnepl::Crnepl rpl;
    rpl.SetPrompt("#>");
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
