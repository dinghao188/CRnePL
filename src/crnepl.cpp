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

    InitSysActionMap();
    InitUserActionMap();
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

string & Crnepl::GetPrevInput()
{
    if (iHisCurPos != iHisEndPos)
    {
        iHisCurPos = RoundPos(iHisCurPos-1);
    }
    return aHistoryRecord[iHisCurPos];
}

string & Crnepl::GetNextInput()
{
    if (iHisCurPos != iHisBeginPos)
    {
        iHisCurPos = RoundPos(iHisCurPos+1);
    }
    return aHistoryRecord[iHisCurPos];
}

string & Crnepl::GetCurInput()
{
    return aHistoryRecord[iHisCurPos];
}

void Crnepl::InitSysActionMap()
{
    oSysActionMap.insert(ACTION_MAP_ITEM(KEY_CTRL_J, ACT_SUBMIT));
    oSysActionMap.insert(ACTION_MAP_ITEM(KEY_CTRL_H, ACT_BS));
    oSysActionMap.insert(ACTION_MAP_ITEM(KEY_CTRL_D, ACT_DEL));
}

void Crnepl::InitUserActionMap()
{
    oUserActionMap.insert(ACTION_MAP_ITEM(KEY_CTRL_U, ACT_CLEAR_LINE));
    oUserActionMap.insert(ACTION_MAP_ITEM(KEY_CTRL_P, ACT_PRE));
    oUserActionMap.insert(ACTION_MAP_ITEM(KEY_CTRL_N, ACT_NEXT));
    oUserActionMap.insert(ACTION_MAP_ITEM(KEY_CTRL_I, ACT_COMPLETION));
}
