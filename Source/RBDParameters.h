/*
  ==============================================================================

    RBDParameters.h
    Created: 13 Jan 2021 4:03:34pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDAudioConstants.h"

// One of the main uses for this enum is to serve as named array indices.
//  Therefore, I don't want to have to cast it to int every time.
//  Current solution is to put a plain enum in a namespace to have name scope
//  but keep automatic casting to int.

namespace Parameter
{
    // TODO: Consider using switching to an enum class
    //  Together with the namespace, this would make using the enum more bulky
    //  and will require extra considerations for using it as an array index,
    //  but will eliminate potentially unsafe implicit cast to int

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
        "ModulationDepth"
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
        "Depth"
    };

    // TODO: Variable parameter value increment depending on the value
    //  It is often common to see different parameter resolution based on its
    //  value. For gain controls, you often get fine step around 0, e.g. 0.1 dB,
    //  and coarser controls at high values, e.g. 0.5 dB step around 10 dB.
    //  This makes sense for logarithmic / skewed parameters.
    //  This might be achieve by supplying custom ValueToText functions...

    // FIXME: Parameters like Time, Modulation Rate should remain continuous!
    //  Snapping the value with parameters who's scale is exponential is
    //  undesirable and leads to audible stepping.
    //  We should either reduce the decimal places shown without snapping the
    //  parameter itself - by passing a custom ValueToText function;
    //  or have an internal parameter proportional to the logarithm of the value

    // NOTE: Gain parameters actually make sense snapped!

    template<typename Type>
    NormalisableRange<Type> createSkewedNormalisableRange (Type rangeStart,
                                                           Type rangeEnd,
                                                           Type intervalValue,
                                                           Type centrePointValue)
    {
        NormalisableRange<Type> range (rangeStart, rangeEnd, intervalValue);
        range.setSkewForCentre (centrePointValue);
        return range;
    }

    inline const NormalisableRange<float> Range[NumParameters]
    {
        {-24.0f, 24.0f, 0.1f, 0.6f, true},                  // Input Gain
        createSkewedNormalisableRange (1.0f, RBD::maxDelayTimeInMs,
                                       0.1f, 200.0f),       // Time
        {0.0f, 120.0f, 1.0f},                               // Feedback
        {0.0f, 100.0f, 1.0f},                               // Dry Wet
        {0.0f, 2.0f, 1.0f},                                 // Type
        {-24.0f, 24.0f, 0.1f, 0.6f, true},                  // Output Gain
        createSkewedNormalisableRange (0.01f, 10.0f,
                                       0.01f, 2.0f),        // Modulation Rate
        {0.0f, 100.0f, 1.0f}                                // Modulation Depth
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
        35.0f   // Modulation Depth
    };

    inline const String Label[NumParameters]
    {
        " dB",  // Input Gain
        " ms",   // Time
        " %",   // Feedback
        " %",   // Dry Wet
        "",     // Type
        " dB",  // Output Gain
        " Hz",  // Modulation Rate
        " %"    // Modulation Depth
    };
} // end namespace Parameter

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


