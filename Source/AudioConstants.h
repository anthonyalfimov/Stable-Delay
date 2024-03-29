/*
  ==============================================================================

    AudioConstants.h
    Created: 4 Feb 2021 6:38:27pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

// TODO: Do we need global constants, or can we place them as members somewhere?
// The benefit of global constants is that they are all collected in one place
// Perhaps, they could live in a relevant base class?

namespace RBD
{
    // C++17 "inline" keyword ensures that only one copy of the constant
    //  is created for the whole project

    inline constexpr float maxDelayTimeInSeconds = 1.0f;
    inline constexpr float maxDelayTimeInMs = maxDelayTimeInSeconds * 1000.0f;
}
