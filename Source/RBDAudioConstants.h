/*
  ==============================================================================

    RBDAudioConstants.h
    Created: 4 Feb 2021 6:38:27pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

// TODO: Do we need global constants, or can we place them as members somewhere?
// The benefit of global constants is that they are all collected in one place
// Perhaps, they could live in a relevant base class?

// TODO: Do we need to namespace Audio constants? Use better namespace name? Template instead?

namespace RBD
{
    // Global const is static by default, just reinforce the intent
    constexpr static int bufferSize = 192000;
    
    constexpr static double paramSmoothingCoefGeneric = 0.04;
    constexpr static double paramSmoothingCoefFine = 0.004;
    constexpr static float meterSmoothingCoef = 0.2f;
    
    constexpr static float minMeterDbLevel = 96.0f;
}
