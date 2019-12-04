#ifndef _CRNEPL_H
#define _CRNEPL_H


#include <map>
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>


namespace crnepl
{

/*--------------Cursor Control-----------------------------------------*/
static inline void MoveUp(int n)
{
    printf("\033[%dA", n);
}
static inline void MoveDown(int n)
{
    printf("\033[%dB", n);
}
static inline void MoveRight(int n)
{
    printf("\033[%dC", n);
}
static inline void MoveLeft(int n)
{
    printf("\033[%dD", n);
}
static inline void MoveTo(int x,int y)
{
    printf("\033[%d;%dH", x, y);
}
static inline void CursorStore()
{
    printf("\033[s");
}
static inline void CursorRestore()
{
    printf("\033[u");
}
/*---------------------------------------------------------------------*/

/*------------------Character Delete-----------------------------------*/
static inline void DelRight(int n)
{
    if (n == -1)
    {
        printf("\033[K");
    }
    else
    {
        printf("\033[%dP", n);
    }
}
static inline void DelLeft()
{
    printf("\033[1K");
}
static inline void ClearLine()
{
    printf("\033[2K");
}
/*---------------------------------------------------------------------*/

/*
 * define all code of key here.
 * I get it by using bash on linux,
 * maybe it's different in different operating systems
 */
enum KEY_CODE
{
    // Control key
    KEY_CTRL_A = 1,
    KEY_CTRL_B, KEY_CTRL_C, KEY_CTRL_D, KEY_CTRL_E, KEY_CTRL_F,
    KEY_CTRL_G, KEY_CTRL_H, KEY_CTRL_I, KEY_CTRL_J, KEY_CTRL_K,
    KEY_CTRL_L, KEY_CTRL_M, KEY_CTRL_N, KEY_CTRL_O, KEY_CTRL_P,
    KEY_CTRL_Q, KEY_CTRL_R, KEY_CTRL_S, KEY_CTRL_T, KEY_CTRL_U,
    KEY_CTRL_V, KEY_CTRL_W, KEY_CTRL_X, KEY_CTRL_Y, KEY_CTRL_Z,

    // Extend control key start with 27
    KEY_EXT = 27, // Esc

    // displayable character code
    DISPLAY_ASCII_START = 32,   // ' '
    DISPLAY_ASCII_END = 126,   // '~'

    KEY_BS = 127 // maybe Backspace or Ctrl+Backspace
};

/*
 * define all supported actions here
 */
enum ACTION_CODE
{
    ACT_NOACT = 0,  // No Action
    ACT_BS,         // Remove one character forward
    ACT_DEL,        // Remove one character under current cursor
    ACT_PRE_HIS,        // Get previous command
    ACT_NEXT_HIS,       // Get next command
    ACT_CLEAR_LINE, // Clear current line input
    ACT_COMPLETION, // complete current input
    ACT_SUBMIT      // submit current  input
};

/* simplify the definition of key-action map*/
typedef std::pair<KEY_CODE, ACTION_CODE> ACTION_MAP_ITEM;

/*
 * class Crnepl
 * The most important to implement RPL function
 */
class Crnepl
{
public:
    Crnepl();
    ~Crnepl();
    
    /*
     * @description The entry of getting an input
     * @param buf The buffer to store input,user provided
     * @return void
     */
    void LoopOnce(char *buf);

    /* @description Add a record to the history queue
     * @return void
     */
    void AddHistory(const std::string &record);
    /*
     * @description Get previous input history
     * @return the reference of previous record
     */
    std::string & GetPrevInput();
    /*
     * Get current input history
     * @return the reference of current record
     */
    std::string & GetCurInput();
    /*
     * @description Get next input history
     * @return the reference of next record
     */
    std::string & GetNextInput();

protected:
    int RoundPos(int iPos) { return (iPos+HISTORY_SIZE) % HISTORY_SIZE; }
    void InsertChar(char ch);
    void DoActionSubmit();
    void DoActionBackspace();
    void DoActionClearline();
    void DoActionPreHis();
    void DoActionNextHis();
private:
    /*
     * @description Initialize sytem action map, cannot be modified
     * @return void
     */
    void InitSysActionMap();
    /*
     * @description Initialize user action map, can be customed
     * @return void
     */
    void InitUserActionMap();
    /*
     * @description Get an action code according by keycode
     * @return ACTION_CODE
     */
    ACTION_CODE GetActionCode(KEY_CODE key);
    /*
     * @description Execute an action
     * @return bool true:end this input loop,false:continue this input loop
     */
    bool ExecuteAction(ACTION_CODE iActionCode);

public:
    static const int HISTORY_SIZE = 100; //max size of historical input
private:
    std::string m_sPrompt; // prompt string

    std::map<KEY_CODE,ACTION_CODE> m_oSysActionMap;
    std::map<KEY_CODE,ACTION_CODE> m_oUserActionMap;

    std::string m_aHistoryRecord[HISTORY_SIZE]; //record the history input, this is a loop queue
    int m_iHisBeginPos; // begin pos of his queue;
    int m_iHisEndPos; // end pos of his queue;
    int m_iHisCurPos; // cur pos of his queue;

    char *m_sBuf;
    int m_iBufPos;
    int m_iBufLen;
};

}


namespace crnepl
{

using namespace std;

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
    CursorStore();
    

    while (cin.get(ch) && cin.good())
    {
        if (ch >= DISPLAY_ASCII_START && ch <= DISPLAY_ASCII_END)
        {
            InsertChar(ch);
            continue;
        }
        
        iActionCode = GetActionCode(KEY_CODE(ch));
        if (ExecuteAction(iActionCode))
        {
            break;
        }
    }
}

void Crnepl::AddHistory(const string &record)
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
    m_oSysActionMap.insert(ACTION_MAP_ITEM(KEY_BS,ACT_BS));
}

void Crnepl::InitUserActionMap()
{
    m_oUserActionMap.insert(ACTION_MAP_ITEM(KEY_CTRL_U, ACT_CLEAR_LINE));
    m_oUserActionMap.insert(ACTION_MAP_ITEM(KEY_CTRL_P, ACT_PRE_HIS));
    m_oUserActionMap.insert(ACTION_MAP_ITEM(KEY_CTRL_N, ACT_NEXT_HIS));
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
    case ACT_CLEAR_LINE:
        DoActionClearline();
        break;
    case ACT_PRE_HIS:
        DoActionPreHis();
        break;
    case ACT_NEXT_HIS:
        DoActionNextHis();
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

    string sTmp(m_sBuf);
    if (!sTmp.empty())
    {
        AddHistory(string(m_sBuf));    
    }
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

void Crnepl::DoActionClearline()
{
    CursorRestore();
    DelRight(-1);

    m_iBufPos = m_iBufLen = 0;
    m_sBuf[m_iBufPos] = 0;

    CursorStore();
}

void Crnepl::DoActionPreHis()
{
    DoActionClearline();
    
    string &preHis = GetPrevInput();
    for (string::size_type i = 0;i < preHis.length();++i)
    {
        InsertChar(preHis[i]);
    }
}

void Crnepl::DoActionNextHis()
{
    DoActionClearline();

    string &nextHis = GetNextInput();
    for (string::size_type i = 0;i < nextHis.length();++i)
    {
        InsertChar(nextHis[i]);
    }
}
/********************************************************************/

}
#endif
