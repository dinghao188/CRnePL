#include "crnepl.h"

using namespace std;

Crnepl::Crnepl()
{
    prompt = ">";
    istream = NULL;
    ostream = NULL;

    iHisBeginPos = 0;
    iHisEndPos = HISTORY_SIZE-1;
    iHisCurPos = 0;
}

void Crnepl::AddHistory(string &record)
{
    aHistoryRecord[iHisBeginPos] = record;
    iHisBeginPos = RoundPos(iHisBeginPos+1);
    aHistoryRecord[iHisBeginPos] = "";

    if (iHisBeginPos == iHisEndPos)
    {
        iHisEndPos = RoundPos(iHisEndPos+1);
    }
    
    iHisCurPos = iHisBeginPos;
}

string & GetPrevInput()
{
    if (iHisCurPos != iHisEndPos)
    {
        iHisCurPos = RoundPos(iHisCurPos-1);
    }
    return aHistoryRecord[iHisCurPos];
}

string & GetNextInput()
{
    if (iHisCurPos != iHisBeginPos)
    {
        iHisCurPos = RoundPos(iHisCurPos+1);
    }
    return aHistoryRecord[iHisCurPos];
}

string & GetCurInput()
{
    return aHistoryRecord[iHisCurPos];
}

