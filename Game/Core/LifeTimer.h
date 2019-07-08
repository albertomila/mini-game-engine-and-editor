#pragma once
#include "Game/Core/GameTimer.h"

/////////////////////////////////////////////////////////////////////////////////
template< bool PAUSE_AFFECTED = false, bool SLOW_MOTION_AFFECTED = false>
struct CLifeTimerGetTimer
{
    static float GetGameTime() { return CGameTimer::Get()->GetGameTime(); }
};

/////////////////////////////////////////////////////////////////////////////////
template<>
struct CLifeTimerGetTimer< false, true >
{
    static float GetGameTime() { return CGameTimer::Get()->GetGameTimeSlowMotion(); }
};

/////////////////////////////////////////////////////////////////////////////////
template<>
struct CLifeTimerGetTimer< true, false >
{
    static float GetGameTime() { return CGameTimer::Get()->GetGameTimePause(); }
};

/////////////////////////////////////////////////////////////////////////////////
template<>
struct CLifeTimerGetTimer< true, true >
{
    static float GetGameTime() { return CGameTimer::Get()->GetGameTimeSlowMotionPause(); }
};

/////////////////////////////////////////////////////////////////////////////////
//CLifeTimerBase
/////////////////////////////////////////////////////////////////////////////////
template< bool PAUSE_AFFECTED = false, bool SLOW_MOTION_AFFECTED = false >
class CLifeTimerBase
{
    static const S8 INVALID_TIME = -1;
public:
    CLifeTimerBase()
        : m_lifeTime( INVALID_TIME )
    {
    }

    CLifeTimerBase( float lifeTimer )
    {
        SetLifeTime( lifeTimer );
    }

    ~CLifeTimerBase(){}

    void Invalidate()
    {
        SetLifeTime( INVALID_TIME );
    }

    void SetExpired()
    {
        SetLifeTime(0.f);
    }

    void SetLifeTime(float lifetime)
    {
        m_lifeTime = lifetime;

        Reset();
    }

    void Reset()
    {
        m_startTime = GetTime();
    }

    virtual float GetTime() const
    {
        return CLifeTimerGetTimer<PAUSE_AFFECTED, SLOW_MOTION_AFFECTED>::GetGameTime();
    }

    float GetLifeTime() const
    {
        return m_lifeTime;
    }

    float GetCurrentTime() const
    {
        return GetTime() - m_startTime;
    }

    float GetRemaining() const
    {
        return Utils::Max(0.f, m_lifeTime - GetCurrentTime() );
    }

    bool HasExpired() const
    {
        if( m_lifeTime == INVALID_TIME )
        {
            return false;
        }

        return (m_lifeTime == 0.f || GetCurrentTime() >= m_lifeTime);
    }

private:
    float m_lifeTime;
    float m_startTime;
};

      //CLifeTimerBase<PAUSE_AFFECTED,SLOW_MOTION_AFFECTED >
typedef CLifeTimerBase<false, false>   CLifeTimer;
typedef CLifeTimerBase<false, true>    CLifeTimerSlowMotion;
typedef CLifeTimerBase<true, false>    CLifeTimerPause;
typedef CLifeTimerBase<true, true>     CLifeTimerPauseSlowMotion;

/*

std::cout   << "Timer = " << m_timer.GetLifeTime() 
<< ", remaining =" << m_timer.GetRemaining() 
<< ", current =" << m_timer.GetCurrentTime() 
<< ", HasExpired =" << m_timer.HasExpired() << "\n";

*/