

#ifndef __X18_WINERROR_H
#define __X18_WINERROR_H


#include "nsisapi.h"

void PushWinError(DWORD);
void PushLastError(void);


#endif