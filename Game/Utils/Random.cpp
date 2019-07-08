#include "Game_PCH.h"
#include "Game/Utils/Random.h"

namespace Utils
{
    ///////////////////////////////////////////////////////////////////////////
    float RandomFloat(float min, float max)
    {
        // this  function assumes max > min, you may want 
        // more robust error checking for a non-debug build
        //assert(max > min); 
        float random = ((float) rand()) / (float) RAND_MAX;

        // generate (in your case) a float between 0 and (4.5-.78)
        // then add .78, giving you a float between .78 and 4.5
        float range = max - min;  
        return (random*range) + min;
    }

    ///////////////////////////////////////////////////////////////////////////
    float RandomFloat(float max)
    {
        float random = ((float) rand()) / (float) RAND_MAX;
        return (random*max);
    }

	///////////////////////////////////////////////////////////////////////////
    void Clamp( float& input, float min, float max)
    {
        if( input < min ) input = min;
        else if( input > max ) input = max;
    }
}