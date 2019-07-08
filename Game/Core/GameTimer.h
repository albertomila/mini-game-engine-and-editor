#pragma once
#include "Game\Core\Singleton.h"
#include "Game\Core\BasicTypes.h"

/////////////////////////////////////////////////////////////////////////////////
//CGameTimer
/////////////////////////////////////////////////////////////////////////////////
class GAME_DLL CGameTimer : public CSingleton<CGameTimer>
{
    static const U32 MAX_FPS = 60;
    static const U32 MIN_FPS = 10;

public:
    CGameTimer();
    ~CGameTimer();
    
    float GetFPS() const{ return m_fps; }
    float GetFrameTime() const{ return m_currentDt; }
    float GetGameTime() const{ return m_currentTime; }

    bool IsGamePaused() const { return m_pause; }
    void Pause( bool pause = true) { m_pause = pause; }
    float GetFrameTimePause() const{ return m_pause ? 0 : m_currentDt; }
    float GetFrameTimeSlowMotionPause() const{ return m_pause ? 0 : m_currentDtSlowMotion; }
    float GetGameTimePause() const{ return m_pause ? 0 : m_currentTime; }
    float GetGameTimeSlowMotionPause() const{ return m_pause ? 0 : m_currentTimeSlowMotion; }

    void PreUpdate();
    void PostUpdate();

    void    SetSlowmotionFactor(float factor) { m_slowmotionFactor = factor; }
    double  GetSlowmotionFactor() const { return m_slowmotionFactor; }
    float GetFrameTimeSlowMotion() const{ return m_currentDtSlowMotion; }
    float GetGameTimeSlowMotion() const{ return m_currentTimeSlowMotion; }
    
private:
    double GetClockTime() const;

private:
    float  m_fps;
    U64    m_currentFrameCount;
    double m_minDt;
    double m_maxDt;

    double m_currentSystemTime;
    double m_lastSystemTime;

    double m_currentDt;
    double m_currentTime;
    double m_lastTime;

    double m_currentDtSlowMotion;
    double m_currentTimeSlowMotion;
    double m_slowmotionFactor;

    bool   m_pause;
};