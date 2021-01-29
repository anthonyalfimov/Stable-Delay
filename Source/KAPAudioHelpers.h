/*
  ==============================================================================

    KAPAudioHelpers.h
    Created: 5 Jan 2021 4:58:30pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

namespace KAP
{
//  Constants
    const static int bufferSize = 192000;  // global const is static by default, reinforce intent
    
    const static double paramSmoothingCoefGeneric = 0.04;
    const static double paramSmoothingCoefFine = 0.004;
    const static float meterSmoothingCoef = 0.2f;
    
// Helper functions
    inline float linearInterp (float value0, float value1, double fractionalPosition)
    {
        return (1.0 - fractionalPosition) * value0 + fractionalPosition * value1;
    }
    
    // TODO: There must be a better way to disable denormals if this is what we want to achieve
    inline float denormalise (float inValue)
    {
        return std::fabs (inValue) < 1e-15 ? 0.0f : inValue;
    }
}
