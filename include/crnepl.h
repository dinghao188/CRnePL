#ifndef _CRNEPL_H
#define _CRNEPL_H

#include <map>
#include <string>
#include <vector>

namespace crnepl
{

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

    /*
     * @description Set the prompt string
     * @param sPrompt The Prompt string
     * @return void
     */
    void SetPrompt(const char *buf);

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
#endif
