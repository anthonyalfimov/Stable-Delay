/*
  ==============================================================================

    RBDAudioUtilities.h
    Created: 5 Jan 2021 4:58:30pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace RBD
{
    inline float linearInterp (float value0, float value1, double fractionalPosition)
    {
        return (1.0 - fractionalPosition) * value0 + fractionalPosition * value1;
    }

    inline float denormalise (float inValue)
    {
        return fabs (inValue) < 1e-15 ? 0.0f : inValue;
    }
    
    // TODO: Consider using an unclipped version of rational tanh approximation
    /**
    Clipped version of fast rational tanh(x) approximation
    */
    inline float fastTanh (float x)
    {
        return jlimit (-1.0f, 1.0f, x * (27 + x * x) / (27 + 9 * x * x));
    }
    
    // TODO: This function just remaps [0, 1] -> [0, 1] with a skew. Should this be done better?
    //  Technically, it's not [0, 1]: sample at 1.0f is 0 dBFS, but it could
    //  exceed that (?)
    //  However, our meters don't go above 0 dBFS
    
    /**
    This function takes sample value from [0, 1] and remaps it back to [0, 1],
    but now with logarithmic scale (linear on dB level)
    */
    inline float remappedMeterLevel (float inSampleLevel)
    {
        // Convert [0, 1] level into dBFS scale - result is negative for all
        // non-clipping levels.
        // Arbritrary "floor" constant added - to avoid singularity when
        // converting 0 to log scale
        // TODO: Does this "floor" constant need to relate to some other values? minMeterDbLevel?
        float inDbLevel = Decibels::gainToDecibels (inSampleLevel + 0.00001f);
        
        // `minMeterDbLevel` defines the minimum level displayed on the meter.
        //  If it is 96.0f, then meter starts from -96 dBFS. Levels below that
        //  will result in a negative value that should be caugth by the meter
        //  painting routine.
        return (inDbLevel + minMeterDbLevel) / minMeterDbLevel;
    }
}
