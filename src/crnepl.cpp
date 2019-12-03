#include "crnepl.h"
#include "display.h"
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
    m_sBuf = buf;
    m_iBufPos = 0;
    m_iBufLen = 0;

    char ch;
    ACTION_CODE iActionCode = ACT_NOACT;
    m_sBuf[0] = 0;
    cout << m_sPrompt << ' ';

    while (cin.get(ch) && cin.good())
    {
        if (ch >= DISPLAY_ASCII_START && ch <= DISPLAY_ASCII_END)
        {
            InsertChar(ch);
            continue;
        }
        
        iActionCode = GetActionCode((KEY_CODE)ch);
        if (ExecuteAction(iActionCode))
        {
            break;
        }
    }
    
}

void Crnepl::AddHistory(string &record)
{
    m_aHistoryRecord[m_iHisBeginPos] = record;
    m_iHisBeginPos = RoundPos(m_iHisBeginPos+1);
    m_aHistoryRecord[m_iHisBeginPos] = "";

    if (m_iHisBeginPos == m_iHisEndPos){
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
    m_oSysActionMap.insert(ACTION_MAP_ITEM(KEY_CTRL_M, ACT_SUBMIT));
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

ACTION_CODE Crnepl::GetActionCode(KEY_CODE key)
{
    ACTION_CODE iActionCode = ACT_NOACT;
    map<KEY_CODE, ACTION_CODE>::iterator it;

    it = m_oUserActionMap.find(key);
    if (it != m_oUserActionMap.end())
    {
        return (it->second);
    }

    it = m_oSysActionMap.find(key);
    if (it != m_oSysActionMap.end())
    {
        return (it->second);
    }

    return iActionCode;
}

bool Crnepl::ExecuteAction(ACTION_CODE iActionCode)
{
    switch (iActionCode)
    {
    case ACT_NOACT:
        break;
    case ACT_SUBMIT:
        DoActionSubmit();
        return true;
        break;
    case ACT_BS:
        DoActionBackspace();
        break;
    default:
        break;
    }
    return false;
}

/************** ALL ACTION FUNCTION *********************************/
void Crnepl::InsertChar(char ch)
{
    if (m_iBufPos == m_iBufLen)
    {
        cout << ch;

        m_sBuf[m_iBufPos++] = ch;
        ++m_iBufLen;
    }
    else
    {
        DelRight(-1);

        ++m_iBufLen;
        for (int i = m_iBufLen-1; i > m_iBufPos; --i)
        {
            m_sBuf[i] = m_sBuf[i-1];
        }
        m_sBuf[m_iBufPos] = ch;
    }
}

void Crnepl::DoActionSubmit()
{
    cout << endl;
    m_sBuf[m_iBufLen] = 0;
}

void Crnepl::DoActionBackspace()
{
    if (m_iBufPos == 0)
    {
        return;
    }
    
    if (m_iBufPos == m_iBufLen)
    {
        MoveLeft(1);
        DelRight(1);

        m_sBuf[--m_iBufPos] = 0;
        --m_iBufLen;
    }
    else 
    {
        m_sBuf[m_iBufPos] = 0;
    }
}
/********************************************************************/
