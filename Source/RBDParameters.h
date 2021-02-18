/*
  ==============================================================================

    RBDParameters.h
    Created: 13 Jan 2021 4:03:34pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

// One of the main uses for this enum is to serve as named array indices.
// Therefore, I don't want to have to cast it to int every time.
// Current solution is to put a plain enum in a namespace to have name scope, but
// keep automatic casting to int.

// TODO: Find a solution to use enum class as an array index efficiently
// e.g. use an array class to store items and add an overloaded operator[] ?

namespace Parameter
{
    enum Index : int
    {
        InputGain = 0,
        DelayTime,
        DelayFeedback,
        DelayWetDry,
        DelayType,
        OutputGain,
        ModulationRate,
        ModulationDepth,
        NumParameters
    };

    const static String ID[NumParameters]
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

    const static String Label[NumParameters]
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

    const static float DefaultValue[NumParameters]
    {
        0.5f,   // Input Gain
        0.5f,   // Time
        0.35f,  // Feedback
        0.5f,  // Dry Wet
        0.0f,   // Type
        0.5f,   // Output Gain
        0.2f,   // Modulation Rate
        0.35f   // Modulation Depth
    };
} // end namespace Parameter

//enum RBDDelayType
//{
//    kRBDDelayType_Delay = 0,
//    kRBDDelayType_Chorus
//};
