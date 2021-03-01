/*
  ==============================================================================

    RBDParameters.h
    Created: 13 Jan 2021 4:03:34pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

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

    inline const String Label[NumParameters]
    {
        "Input Gain",
        "Time",
        "Feedback",
        "Dry Wet",
        "Type",
        "Output Gain",
        "Mod Rate",
        "Mod Depth"
    };

    inline const NormalisableRange<float> Range[NumParameters]
    {
        {0.0f, 1.0f},       // Input Gain
        {0.0f, 1.0f},       // Time
        {0.0f, 1.0f},       // Feedback
        {0.0f, 1.0f},       // Dry Wet
        {1.0f, 2.0f, 1.0f}, // Type
        {0.0f, 1.0f},       // Output Gain
        {0.0f, 1.0f},       // Modulation Rate
        {0.0f, 1.0f}        // Modulation Depth
    };

    inline const float DefaultValue[NumParameters]
    {
        0.5f,   // Input Gain
        0.5f,   // Time
        0.35f,  // Feedback
        0.5f,   // Dry Wet
        1.0f,   // Type
        0.5f,   // Output Gain
        0.2f,   // Modulation Rate
        0.35f   // Modulation Depth
    };
} // end namespace Parameter

// We will use this enum as comboBox IDs, so the items must start from 1
enum class FxTypeID
{
    Delay = 1,
    Chorus
};

