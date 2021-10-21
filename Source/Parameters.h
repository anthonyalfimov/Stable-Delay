/*
  ==============================================================================

    Parameters.h
    Created: 13 Jan 2021 4:03:34pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "Utilities.h"
#include "AudioConstants.h"
#include "PiecewiseRange.h"

// One of the main uses for these enums is to serve as named array indices.
//  Therefore, I don't want to have to cast it to int every time.
//  Current solution is to put a plain enum in a namespace to have name scope
//  but keep automatic casting to int.

// TODO: Consider turning the Parameter namespace into a class
//  This will allow to have direct control over the lifetime of the contained
//  objects, which is becoming more important as these objects become more
//  complex.
//  This class would contain the ParameterLayout generating method, and would be
//  instantiated in the PluginProcessor.

namespace Parameter
{
    enum Index : int
    {
        InputDrive = 0,
        DryWet,
        OutputGain,

        DClipRise,
        DClipFall,

        NumParameters
    };

    // TODO: Consider switching to StringArray objects

    inline const String ID[NumParameters]
    {
        "InputDrive",
        "Mix",
        "OutputGain",

        "DClipRise",
        "DClipFall",
    };

    inline const String Name[NumParameters]
    {
        "Drive",
        "Mix",
        "Output Gain",

        "Rise Time",
        "Fall Time",
    };

    // TODO: DRY generation of ranges and ticks from min, max and mid values
    //  Ranges and ticks are pretty WET, we can generate these based on
    //  min and max value and the mid-point value. It should be possible to
    //  not specify the mid value, in which case it's calculated from min and
    //  max

    inline const PiecewiseRange<float, 3> gainRange
    {
        { { -10.0f, -2.0f, 0.1f }, 0.33f },
        { { -2.0f, 2.0f, 0.1f }, 0.67f },
        { { 2.0f, 10.0f, 0.1f }, 1.0f }
    };
    
    inline const PiecewiseRange<float, 3> riseRange
    {
        { { 0.04f, 0.1f, 0.01f }, 0.2f },
        { { 0.1f, 1.0f, 0.1f }, 0.75f },
        { { 1.0f, 10.0f, 1.0f }, 1.0f }
    };
    
    inline const PiecewiseRange<float, 3> fallRange
    {
        { { 10.0f, 100.0f, 1.0f }, 0.4f },
        { { 100.0f, 1000.0f, 1.0f }, 0.8f },
        { { 1000.0f, 5000.0f, 10.0f }, 1.0f }
    };

    inline const NormalisableRange<float> Range[NumParameters]
    {
        // Input Drive:
        { 0.0f, 32.0f, 0.1f },
        // Dry Wet Mix:
        { 0.0f, 100.0f },
        // Output Gain:
        gainRange.getNormalisableRange(),

        // DClipRise:
        riseRange.getNormalisableRange(),
        // DClipFall:
        fallRange.getNormalisableRange(),
    };

    inline const float DefaultValue[NumParameters]
    {
        0.0f,   // Input Drive
        50.0f,  // Dry Wet
        0.0f,   // Output Gain

        0.2f,   // DClipRise
        1200.0f,// DClipFall
    };

    inline const String Label[NumParameters]
    {
        " dB",  // Input Drive
        " %",   // Dry Wet
        " dB",  // Output Gain

        " ms",  // DClipRise
        " ms",  // DClipFall
    };

    using stringFromValueFunction = std::function<String(float value,
                                                         int maxStringLength)>;
    
    inline const stringFromValueFunction stringFromValue[NumParameters]
    {
        nullptr,                    // Input Drive
        showDecimalPlaces<0>,       // Dry Wet
        nullptr,                    // Output Gain

        showDecimalPlaces<2>,       // DClipRise
        showDecimalPlaces<0>,       // DClipFall
    };

    inline const std::initializer_list<float> majorTicks[NumParameters]
    {
        // Input Drive:
        { 0.0f, 12.0f, 24.0f },
        // Dry Wet:
        { 0.0f, 100.0f, 50.0f },
        // Output Gain:
        { -10.0f, 10.0f, 0.0f },

        {}, {}
    };

    inline const std::initializer_list<float> minorTicks[NumParameters]
    {
        // Input Drive:
        { 2, 4, 6, 8, 10, 14, 16, 18, 20, 22 },
        // Dry Wet:
        { 10, 20, 30, 40, 60, 70, 80, 90 },
        // Output Gain:
        { 1, 2, 4, 6, 8, -1, -2, -4, -6, -8 },

        {}, {}
    };
} // end namespace Parameter
