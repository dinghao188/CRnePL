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
        KEY_CTRL_EXT = 27,

        // displayable character code
        DISPLAY_ASCII_START = 32,   // ' '
        DISPLAY_ASCII_END = 126,   // '~'
    };

    /*
     * define all supported actions here
     */
    enum ACTION_CODE
    {
        ACT_BS = 1,     // Remove one character forward
        ACT_DEL,        // Remove one character under current cursor
        ACT_PRE,        // Get previous command
        ACT_NEXT,       // Get next command
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
        void AddHistory(std::string &record);
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
    };
}
#endif
