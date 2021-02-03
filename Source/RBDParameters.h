/*
  ==============================================================================

    RBDParameters.h
    Created: 13 Jan 2021 4:03:34pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

// TODO: switch to an enum class in a namespace

enum RBDParameter
{
    kParameter_InputGain = 0,
    kParameter_DelayTime,
    kParameter_DelayFeedback,
    kParameter_DelayWetDry,
    kParameter_DelayType,
    kParameter_OutputGain,
    kParameter_ModulationRate,
    kParameter_ModulationDepth,
    kParameter_TotalNumParameters
};

// TODO: put inside a namespace

const static String RBDParameterID[kParameter_TotalNumParameters]
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

const static String RBDParameterLabel[kParameter_TotalNumParameters]
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

const static float RBDParameterDefaultValue[kParameter_TotalNumParameters]
{
    0.5f,   // Input Gain
    0.5f,   // Time
    0.35f,  // Feedback
    0.35f,  // Dry Wet
    0.0f,   // Type
    0.5f,   // Output Gain
    0.2f,   // Modulation Rate
    0.35f   // Modulation Depth
};
