#ifndef _CRNEPL_H
#define _CRNEPL_H

#include <istream>
#include <ostream>
#include <string>
#include <vector>

/*
 * class Crnepl
 * The most important to implement RPL function
 */
class Crnepl
{
public:
    Crnepl();
    
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

public:
    const int HISTORY_SIZE = 100; //max size of historical input
private:
    std::string prompt; // prompt string
    std::istream *pInput; //input stream
    std::ostream *pOutput; //output stream

    std::string aHistoryRecord[HISTORY_SIZE]; //record the history input, this is a loop queue
    int iHisBeginPos; // begin pos of his queue;
    int iHisEndPos; // end pos of his queue;
    int iHisCurPos; // cur pos of his queue;
};

#endif
