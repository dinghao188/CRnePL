#include "crnepl.h"
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace crnepl;

Crnepl::Crnepl()
{
    m_sPrompt = ">";

    m_iHisBeginPos = 0;
    m_iHisEndPos = HISTORY_SIZE-1;
    m_iHisCurPos = 0;

    InitSysActionMap();
    InitUserActionMap();

#ifdef __linux
    system("stty -echo; stty -icanon");
#endif
}

Crnepl::~Crnepl()
{
#ifdef __linux
    system("stty echo; stty icanon");
#endif
}

void Crnepl::LoopOnce(char *buf)
{
    char ch;
    int bufPos = 0;
    buf[0] = 0;
    cout << m_sPrompt << ' ';

    while (cin.get(ch) && cin.good())
    {
        if (ch >= crnepl::DISPLAY_ASCII_START && ch <= DISPLAY_ASCII_END)
        {
            cout << ch;
            buf[bufPos++] = ch;
        }
        else if (ch == KEY_CTRL_J)
        {
            buf[bufPos] = 0;
            cout << endl;
            break;
        }
        else
        {
        }
    }
    
}

void Crnepl::AddHistory(string &record)
{
    m_aHistoryRecord[m_iHisBeginPos] = record;
    m_iHisBeginPos = RoundPos(m_iHisBeginPos+1);
    m_aHistoryRecord[m_iHisBeginPos] = "";

    if (m_iHisBeginPos == m_iHisEndPos)
    {
        m_iHisEndPos = RoundPos(m_iHisEndPos+1);
    }
    
    m_iHisCurPos = m_iHisBeginPos;
}

string & Crnepl::GetPrevInput()
{
    if (m_iHisCurPos != m_iHisEndPos)
    {
        m_iHisCurPos = RoundPos(m_iHisCurPos-1);
    }
    return m_aHistoryRecord[m_iHisCurPos];
}

string & Crnepl::GetNextInput()
{
    if (m_iHisCurPos != m_iHisBeginPos)
    {
        m_iHisCurPos = RoundPos(m_iHisCurPos+1);
    }
    return m_aHistoryRecord[m_iHisCurPos];
}

string & Crnepl::GetCurInput()
{
    return m_aHistoryRecord[m_iHisCurPos];
}

void Crnepl::InitSysActionMap()
{
    m_oSysActionMap.insert(ACTION_MAP_ITEM(KEY_CTRL_J, ACT_SUBMIT));
    m_oSysActionMap.insert(ACTION_MAP_ITEM(KEY_CTRL_H, ACT_BS));
    m_oSysActionMap.insert(ACTION_MAP_ITEM(KEY_CTRL_D, ACT_DEL));
}

void Crnepl::InitUserActionMap()
{
    m_oUserActionMap.insert(ACTION_MAP_ITEM(KEY_CTRL_U, ACT_CLEAR_LINE));
    m_oUserActionMap.insert(ACTION_MAP_ITEM(KEY_CTRL_P, ACT_PRE));
    m_oUserActionMap.insert(ACTION_MAP_ITEM(KEY_CTRL_N, ACT_NEXT));
    m_oUserActionMap.insert(ACTION_MAP_ITEM(KEY_CTRL_I, ACT_COMPLETION));
}
