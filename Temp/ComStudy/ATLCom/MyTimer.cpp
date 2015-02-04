// MyTimer.cpp : Implementation of CMyTimer

#include "stdafx.h"
#include "MyTimer.h"
#include <time.h>

// CMyTimer

//连接点
static CMyTimer * g_pMyTimer;
void static CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime )
{
    /********************* VT_DATE 表示时间的方法 *********************
    DATE 是一个 8 字节的浮点数，即 double 类型
    它的整数部分表示：从1899/12/30 零点开始到现在的总天数
    它的小数部分表示：当天从零点算起，已经度过了几分之几天
    *******************************************************************/
    time_t lt;
    ::time( &lt );	// 取得自 1970/1/1 0:0:0 以来，到当前的总秒数

    lt += 8*60*60;	// 北京时间比格林尼制早8个小时

    long day=(long)(lt/(60*60*24)) + 25569;	// 1899/12/30 到现在的天数
    // 25569 是1899/12/30 到 1970/1/1 的总天数
    // 因为 VARIANT(VT_DATE) 的时间是按照 1899/12/30 开始计算的

    DATE dtSrc = day + (lt % 86400) / 86400.;	// 86400 是一天的总秒数

    VARIANT v;
    v.vt = VT_DATE;
    v.date = dtSrc;
    g_pMyTimer->Fire_Timer( v );
}

STDMETHODIMP CMyTimer::SetTimer(LONG uElapse)
{
    // uElapse 参数的单位是毫秒(1/1000 秒)

    KillTimer();	// 如果已经设置了时钟，则先删除
    m_idTimer = ::SetTimer( NULL, 0, uElapse, TimerProc );
    g_pMyTimer = this;

    return m_idTimer? S_OK : E_FAIL;	// 非0表示成功
}

STDMETHODIMP CMyTimer::KillTimer(void)
{
    if( m_idTimer )
    {
        ::KillTimer( NULL, m_idTimer );
        m_idTimer = 0;
        g_pMyTimer = NULL;
    }

    return S_OK;
}
