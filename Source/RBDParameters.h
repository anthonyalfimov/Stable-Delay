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

// TODO: Consider using switching to an enum classes
//  Together with the namespace, this would make using the enum more bulky
//  and will require extra considerations for using it as an array index,
//  but will eliminate potentially unsafe implicit cast to int

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
        StereoWidth,
        NumParameters
    };

    // TODO: Consider switching to StringArray objects

    inline const String ID[NumParameters]
    {
        "InputGain",
        "Time",
        "Feedback",
        "WetDry",
        "Type",
        "OutputGain",
        "ModulationRate",
        "ModulationDepth",
        "StereoWidth"
    };

    inline const String Name[NumParameters]
    {
        "Input Gain",
        "Time",
        "Feedback",
        "Dry Wet",
        "Type",
        "Output Gain",
        "Rate",
        "Depth",
        "Width"
    };

    inline const NormalisableRange<float> Range[NumParameters]
    {
        {-24.0f, 24.0f, 0.1f, 0.6f, true}, // Input Gain
        createSkewedNormalisableRange (1.0f, RBD::maxDelayTimeInMs, 0.0f, 200.0f), // Time
        createSkewedNormalisableRange (0.0f, 120.0f, 0.0f, 50.0f),// Feedback
        {0.0f, 100.0f}, // Dry Wet
        {0.0f, 2.0f, 1.0f}, // Type
        {-24.0f, 24.0f, 0.1f, 0.6f, true}, // Output Gain
        createSkewedNormalisableRange (0.01f, 10.0f, 0.0f, 2.0f), // Modulation Rate
        {0.0f, 100.0f}, // Modulation Depth
        createSkewedNormalisableRange (0.0f, 100.0f, 0.0f, 30.0f) // Stereo Width
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
        30.0f   // Stereo Width
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
        " %"    // Stereo Width
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
        showDecimalPlaceBelow<10>,  // Stereo Width
    };
} // end namespace Parameter
