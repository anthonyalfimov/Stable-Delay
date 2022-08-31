/*
  ==============================================================================

    Utilities.h

    Convinience templates and functions

    Created: 5 Mar 2021 11:23:16pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// TODO: Move all utilities into `weir` namespace

//==============================================================================
// NormalisableRange Utilities

/**
    This function returns a NormalisableRange with a given range, interval and
    skew factor based on a value that should be the centre of the range.

    @param centrePointValue this must be greater than the start of the range and less than the end.
*/
template <typename Type>
NormalisableRange<Type> createSkewedNormalisableRange (Type rangeStart,
                                                       Type rangeEnd,
                                                       Type intervalValue,
                                                       Type centrePointValue)
{
    NormalisableRange<Type> range (rangeStart, rangeEnd, intervalValue);
    range.setSkewForCentre (centrePointValue);
    return range;
}

//==============================================================================
// Value-to-String conversion utilities
// TODO: comply with passed `maxStringLength`

// C++14 template variable
template <int places>
const auto showDecimalPlaces = [] (float value, int /*maxStringLength*/)
{
    if (places == 0)
        return String (roundToInt (value));
    else
        return String (value, places);
};

template <int threshold>
const auto showDecimalPlaceBelow = [] (float value, int /*maxStringLength*/)
{
    if (value < (threshold - 0.05f))    // account for the String ctor rounding
        return String (value, 1);
    else
        return String (roundToInt (value));
};

//==============================================================================
// Constexpr Utilities

namespace weir
{
    /**
        constexpr utility function that returns the number of its arguments
    */
    template <typename... Args>
    constexpr auto countArguments (const Args&... args)
    {
        return sizeof... (args);
    }
}
