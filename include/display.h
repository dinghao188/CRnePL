#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <stdio.h>

/*
 * @filename display.h
 * @description provide methods to control the display of terminal
 * @note Don't include this file in *.h
 */

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

#endif
