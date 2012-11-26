#pragma once 


inline void UnixTimeToFileTime( time_t t, LPFILETIME pft ) 
{ 
    LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000; 
    pft->dwLowDateTime = (DWORD) ll; 
    pft->dwHighDateTime = (DWORD) (ll >>32); 
} 

inline void UnixTimeToSystemTime(time_t t, LPSYSTEMTIME pst) 
{ 
    FILETIME ft = { 0 }; 
    
    UnixTimeToFileTime(t, &ft); 
    FileTimeToSystemTime(&ft, pst); 
} 

inline time_t	SystemTimeToUnixTime( LPSYSTEMTIME pst ) 
{ 
    FILETIME		ft = { 0 }; 
    ULONGLONG		ull = 0; 
    time_t			tt = 0; 
    
    SystemTimeToFileTime( pst, &ft ); 
    ull = *( const ULONGLONG * ) &ft; 
    tt = ( time_t ) ( ( ull - 0x19DB1DED53E8000 ) / 10000000 ); 
    
    return tt; 
} 

inline time_t FileTimeToUnixTime( LPFILETIME pft ) 
{ 
    return (time_t)(*((ULONGLONG *)pft)-0x19DB1DED53E8000)/10000000; 
} 

WINBASEAPI BOOL WINAPI FileTimeToSystemTime( IN CONST FILETIME *lpFileTime, OUT LPSYSTEMTIME lpSystemTime); 
WINBASEAPI BOOL WINAPI SystemTimeToFileTime( IN CONST SYSTEMTIME* lpSystemTime, OUT LPFILETIME lpFileTime); 


//
//  FILETIME     <=====>    SYSTEMTIME 
//     /|\                   /|\ 
//      |                     | 
//      +======>  time_t <====+ 
