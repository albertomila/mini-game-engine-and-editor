#include "Game_PCH.h"
#include "Game/Core/GameTimer.h"
#include <time.h>
#include <iostream>

/////////////////////////////////////////////////////////////////////////////////
CGameTimer::CGameTimer()
    : m_currentDt( 1.f / MAX_FPS)
    , m_maxDt( 1.f / MAX_FPS )
    , m_minDt( 1.f / MIN_FPS )
    , m_currentTime( 0.f )
    , m_lastTime( 0.f )
    , m_currentFrameCount( 0 )
    , m_currentSystemTime( 0.f )
    , m_lastSystemTime( 0.f )
    , m_pause( false )
    , m_currentDtSlowMotion( 1.f / MAX_FPS)
    , m_slowmotionFactor( 0.5f )
    , m_currentTimeSlowMotion( 0.f )
{
}

/////////////////////////////////////////////////////////////////////////////////
CGameTimer::~CGameTimer()
{
}

/////////////////////////////////////////////////////////////////////////////////
void CGameTimer::PreUpdate()
{
    //internal values
    m_currentSystemTime = GetClockTime();
    double dt = m_currentSystemTime - m_lastSystemTime;  

    //correction because the breakpoints
    if( dt > m_minDt )
    {
        //std::cout << "old DT = " << dt << "\n";
        dt = m_minDt;
        //std::cout << "new DT = " << dt << "\n";
    }

    //final values
    ++m_currentFrameCount;
    m_currentDt     = dt;
    m_currentTime   += m_currentDt;
    m_lastTime      = m_currentTime;
    m_lastSystemTime = m_currentSystemTime;
    m_fps           = 1.f / m_currentDt;
    m_currentDtSlowMotion =  m_currentDt * m_slowmotionFactor;
    m_currentTimeSlowMotion += m_currentDtSlowMotion;

    //debug
    //std::cout << m_currentFrameCount << ", " << m_fps << ", " << m_currentDt << ", " << m_currentDtSlowMotion << ", " << m_currentTime << ", " << m_lastTime << ", "  << m_currentSystemTime << ", " << m_lastSystemTime << "\n";
}

/////////////////////////////////////////////////////////////////////////////////
void CGameTimer::PostUpdate()
{
    //sleep if necessary
    while( ( GetClockTime() - m_currentSystemTime ) < m_maxDt )
    {
    }
}

/////////////////////////////////////////////////////////////////////////////////
double CGameTimer::GetClockTime() const
{
    return (double)clock() / 1000.f;
}