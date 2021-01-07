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
    const static int bufferSize = 192000;  // global const is static by default, reinforce intent
    
    inline float linearInterp (float value0, float value1, double fractionalPosition)
    {
        return (1.0 - fractionalPosition) * value0 + fractionalPosition * value1;
    }
}
