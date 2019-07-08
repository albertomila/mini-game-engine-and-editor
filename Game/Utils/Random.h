#pragma once

namespace Utils
{
    GAME_DLL float RandomFloat(float min, float max);
    GAME_DLL float RandomFloat(float max);
    GAME_DLL void Clamp( float& input, float min, float max);

    template<class TIN, class TMIN, class TMAX >
    void ClampRef( TIN& input, TMIN min, TMAX max);

    template<class TIN, class TMIN, class TMAX >
    TIN Clamp( TIN input, TMIN min, TMAX max);

    template<class T >
    T Max( T value1, T value2);

    template<class T >
    T Min( T value1, T value2);
}

///////////////////////////////////////////////////////////////////////////
template<class TIN, class TMIN, class TMAX >
void Utils::ClampRef(TIN& input, TMIN min, TMAX max)
{
	if (input < min) input = min;
	else if (input > max) input = max;
}

///////////////////////////////////////////////////////////////////////////
template<class TIN, class TMIN, class TMAX >
TIN Utils::Clamp(TIN input, TMIN min, TMAX max)
{
	if (input < min) return min;
	else if (input > max) return max;
	else return input;
}

///////////////////////////////////////////////////////////////////////////
template<class T >
T Utils::Max(T value1, T value2)
{
	return (value1 >= value2)
		? value1
		: value2;
}

///////////////////////////////////////////////////////////////////////////
template<class T >
T Utils::Min(T value1, T value2)
{
	return (value1 <= value2)
		? value1
		: value2;
}