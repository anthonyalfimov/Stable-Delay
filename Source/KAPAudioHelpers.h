/*
  ==============================================================================

    KAPAudioHelpers.h
    Created: 5 Jan 2021 4:58:30pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace KAP
{
//  Constants
    const static int bufferSize = 192000;  // global const is static by default, reinforce intent
    
    const static double paramSmoothingCoefGeneric = 0.04;
    const static double paramSmoothingCoefFine = 0.004;
    const static float meterSmoothingCoef = 0.2f;
    
    const static float minMeterDbLevel = 96.0f;
    
// Helper functions
    inline float linearInterp (float value0, float value1, double fractionalPosition)
    {
        return (1.0 - fractionalPosition) * value0 + fractionalPosition * value1;
    }
    
    // TODO: There must be a better way to disable denormals if this is what we want to achieve
    inline float denormalise (float inValue)
    {
        return fabs (inValue) < 1e-15 ? 0.0f : inValue;
    }
    
    // TODO: This function just remaps [0, 1] -> [0, 1] with a skew. Should this be done better?
    //       Technically, it's not [0, 1]: sample at 1.0f is 0 dBFS, but it could exceed that (?)
    //       However, our meters don't go above 0 dBFS
    
    // This function takes sample value from [0, 1] and remaps it back to [0, 1], but now
    //  with logarithmic scale (linear on dB level)
    inline float remappedMeterLevel (float inSampleLevel)
    {
        // Convert [0, 1] level into dBFS scale - result is negative for all non-clipping levels
        // Arbritrary "floor" constant added - to avoid singularity when converting 0 to log scale
        // TODO: does this "floor" constant need to relate to some other values? minMeterDbLevel?
        float inDbLevel = Decibels::gainToDecibels (inSampleLevel + 0.00001f);
        
        // `minMeterDbLevel` defines the minimum level displayed on the meter.
        //  If it is 96.0f, then meter starts from -96 dBFS. Levels below that will result in a
        //  negative value that should be caugth by the meter painting routine.
        return (inDbLevel + minMeterDbLevel) / minMeterDbLevel;
    }
}
