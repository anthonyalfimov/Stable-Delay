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

namespace Parameter
{
    // TODO: Consider using and enum class with overriden operator[]
    //  If all containers that we need to index with this enum are not
    //  using a plain array, we can add an overrid for the operator[]
    //  to allow these enumerators to act as an index.

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

    // TODO: Consider using StringArray objects

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

    // TODO: Update Type range to follow the FxTypeID values

    const static NormalisableRange<float> Range[NumParameters]
    {
        {0.0f, 1.0f},       // Input Gain
        {0.0f, 1.0f},       // Time
        {0.0f, 1.0f},       // Feedback
        {0.0f, 1.0f},       // Dry Wet
        {0.0f, 1.0f, 1.0f}, // Type
        {0.0f, 1.0f},       // Output Gain
        {0.0f, 1.0f},       // Modulation Rate
        {0.0f, 1.0f}        // Modulation Depth
    };

    const static float DefaultValue[NumParameters]
    {
        0.5f,   // Input Gain
        0.5f,   // Time
        0.35f,  // Feedback
        0.5f,   // Dry Wet
        0.0f,   // Type
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

// TODO: Consider creating a StringArray with Fx Types listed
//  Is is also useful to add an override for operator[] to allow FxTypeID
//  to be used as an index directly - with the necessary conversion.
//  It also can be used in converting FxType parameter value to string.

// TODO: With updated Type range, replace this function with a simple cast
/**
Helper function that converts a float [0, 1] value to the FxTypeID to accomodate
 float representation of the FX TYPE parameter
@param value float [0, 1] representation of the parameter to be interpreted as FxTypeID
*/
inline FxTypeID floatToFxTypeID (float value)
{
    return (value < 0.5f) ? FxTypeID::Delay : FxTypeID::Chorus;
}
