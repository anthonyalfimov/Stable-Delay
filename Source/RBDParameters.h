/*
  ==============================================================================

    RBDParameters.h
    Created: 13 Jan 2021 4:03:34pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDUtilities.h"
#include "RBDAudioConstants.h"

// One of the main uses for these enums is to serve as named array indices.
//  Therefore, I don't want to have to cast it to int every time.
//  Current solution is to put a plain enum in a namespace to have name scope
//  but keep automatic casting to int.

namespace FxType
{
    enum Index : int
    {
        Delay = 0,
        Chorus,
        Flanger
    };

    inline const StringArray Label
    {
        "DELAY",
        "CHORUS",
        "FLANGER"
    };
} // end namespace FxType

namespace Parameter
{
    enum Index : int
    {
        InputGain = 0,
        DelayTime,
        DelayFeedback,
        DryWet,
        FxType,
        OutputGain,
        ModulationRate,
        ModulationDepth,
        StereoSpread,
        NumParameters
    };

    // TODO: Consider switching to StringArray objects

    inline const String ID[NumParameters]
    {
        "InputGain",
        "Time",
        "Feedback",
        "Mix",
        "Type",
        "OutputGain",
        "ModulationRate",
        "ModulationDepth",
        "StereoSpread"
    };

    inline const String Name[NumParameters]
    {
        "Input Gain",
        "Time",
        "Feedback",
        "Mix",
        "Type",
        "Output Gain",
        "Rate",
        "Depth",
        "Spread"
    };

    // TODO: DRY generation of ranges and ticks from min, max and mid values
    //  Ranges and ticks are pretty WET, we can generate these based on
    //  min and max value and the mid-point value. It should be possible to
    //  not specify the mid value, in which case it's calculated from min and
    //  max

    inline const NormalisableRange<float> Range[NumParameters]
    {
        // Input Gain:
        {-24.0f, 24.0f, 0.1f, 0.55f, true},
        // Time:
        createSkewedNormalisableRange (1.0f, RBD::maxDelayTimeInMs, 0.0f, 200.0f),
        // Feedback:
        createSkewedNormalisableRange (0.0f, 120.0f, 0.0f, 50.0f),
        // Dry Wet Mix:
        {0.0f, 100.0f},
        // Type:
        {0.0f, 2.0f, 1.0f},
        // Output Gain:
        {-24.0f, 24.0f, 0.1f, 0.55f, true},
        // Modulation Rate:
        createSkewedNormalisableRange (0.01f, 10.0f, 0.0f, 2.0f),
        // Modulation Depth:
        {0.0f, 100.0f},
        // Stereo Spread:
        createSkewedNormalisableRange (0.0f, 100.0f, 0.0f, 30.0f)
    };

    inline const float DefaultValue[NumParameters]
    {
        0.0f,   // Input Gain
        100.0f, // Time
        50.0f,  // Feedback
        50.0f,  // Dry Wet
        0.0f,   // Type
        0.0f,   // Output Gain
        1.0f,   // Modulation Rate
        50.0f,  // Modulation Depth
        30.0f   // Stereo Spread
    };

    inline const String Label[NumParameters]
    {
        " dB",  // Input Gain
        " ms",  // Time
        " %",   // Feedback
        " %",   // Dry Wet
        "",     // Type
        " dB",  // Output Gain
        " Hz",  // Modulation Rate
        " %",   // Modulation Depth
        " %"    // Stereo Spread
    };

    inline const auto stringFromFxTypeValue = [] (float value, int /*maxStringLength*/)
    {
        const auto fxTypeIndex = static_cast<FxType::Index> (value);

        return FxType::Label[fxTypeIndex];
    };

    using stringFromValueFunction = std::function<String(float value,
                                                         int maxStringLength)>;
    
    inline const stringFromValueFunction stringFromValue[NumParameters]
    {
        nullptr,                    // Input Gain
        showDecimalPlaceBelow<100>, // Time
        showDecimalPlaces<0>,       // Feedback
        showDecimalPlaces<0>,       // Dry Wet
        stringFromFxTypeValue,      // Type
        nullptr,                    // Output Gain
        showDecimalPlaces<2>,       // Modulation Rate
        showDecimalPlaces<0>,       // Modulation Depth
        showDecimalPlaceBelow<10>,  // Stereo Spread
    };

    inline const std::vector<float> majorTicks[NumParameters]
    {
        // Input Gain:
        {-24.0f, 24.0f, 0.0f},
        // Time:
        {1.0f, RBD::maxDelayTimeInMs, 10.0f, 100.0f},
        // Feedback:
        {0.0f, 120.0f, 50.0f},
        // Dry Wet:
        {0.0f, 100.0f, 50.0f},
        // Type:
        {},
        // Output Gain:
        {-24.0f, 24.0f, 0.0f},
        // Modulation Rate:
        {0.01f, 10.0f, 0.1f, 1.0f},
        // Modulation Depth:
        {0.0f, 100.0f, 50.0f},
        // Stereo Spread:
        {}
    };

    // MARK: THIS IS REALLY WET. SHAME

    inline const std::vector<float> minorTicks[NumParameters]
    {
        // Input Gain:
        {1, 4, 8, 12, 16, 20, -1, -4, -8, -12, -16, -20},
        // Time:
        {3, 30, 60, 200, 300, 400, 500, 600, 700, 800, 900},
        // Feedback:
        {10, 20, 30, 40, 60, 70, 80, 90, 100, 110},
        // Dry Wet:
        {10, 20, 30, 40, 60, 70, 80, 90},
        // Type:
        {},
        // Output Gain:
        {1, 4, 8, 12, 16, 20, -1, -4, -8, -12, -16, -20},
        // Modulation Rate:
        {0.03f, 0.3f, 0.6f, 2, 3, 4, 5, 6, 7, 8, 9},
        // Modulation Depth:
        {10, 20, 30, 40, 60, 70, 80, 90},
        // Stereo Spread:
        {}
    };
} // end namespace Parameter
