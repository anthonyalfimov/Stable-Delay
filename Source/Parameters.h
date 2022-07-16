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

namespace Toggle
{
    enum Index : int
    {
        Off = 0,
        On  = 1
    };

    inline const StringArray Label
    {
        "OFF",
        "ON"
    };

} // end namespace Toggle

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
        DelayTime,
        Feedback,
        InvertFeedback,
        DryWet,
        FxType,
        OutputGain,
        ModulationRate,
        ModulationDepth,
        StereoSpread,
        
        DClipDynamic,
        DLimRise,
        DLimConstFall,
        DLimFallRange,
        DClipOutputDetector,
        DClipPostCutFactor,

        DFbMaxHeadroom,
        DFbMinHeadroom,
        DFbOverdrive,
        
        NumParameters
    };

    // TODO: Consider switching to StringArray objects

    inline const String ID[NumParameters]
    {
        "InputDrive",
        "Time",
        "Feedback",
        "InvertFeedback",
        "Mix",
        "Type",
        "OutputGain",
        "ModulationRate",
        "ModulationDepth",
        "StereoSpread",
        
        "DClipDynamic",
        "DLimRise",
        "DLimConstFall",
        "DLimFallRange",
        "DClipOutputDetector",
        "DClipPostCutFactor",

        "DFbMaxHeadroom",
        "DFbMinHeadroom",
        "DFbOverdrive",
    };

    inline const String Name[NumParameters]
    {
        "Drive",
        "Time",
        "Feedback",
        "Invert",
        "Mix",
        "Type",
        "Output Gain",
        "Rate",
        "Depth",
        "Spread",
        
        "Clip: Dynamic",
        "Lim: Rise",
        "Lim: Fall Const",
        "Lim: Fall Range",
        "Clip: Output Detector",
        "Clip: Post Cut Factor",

        "Fb: Max Headroom",
        "Fb: Min Headroom",
        "Fb: Overdrive",
    };

    // TODO: DRY generation of ranges and ticks from min, max and mid values
    //  Ranges and ticks are pretty WET, we can generate these based on
    //  min and max value and the mid-point value. It should be possible to
    //  not specify the mid value, in which case it's calculated from min and
    //  max

    inline const PiecewiseRange<float, 3> timeRange
    {
        { { 1.0f, 10.0f, 0.0f, 0.8f }, 0.15f },
        { { 10.0f, 100.0f }, 0.4f },
        { { 100.0f, RBD::maxDelayTimeInMs }, 1.0f }
    };

    inline const PiecewiseRange<float, 3> rateRange
    {
        { { 0.01f, 0.1f, 0.0f, 0.8f }, 0.15f },
        { { 0.1f, 1.0f }, 0.4f },
        { { 1.0f, 10.0f }, 1.0f }
    };

    inline const PiecewiseRange<float, 3> gainRange
    {
        { { -10.0f, -2.0f, 0.1f }, 0.33f },
        { { -2.0f, 2.0f, 0.1f }, 0.67f },
        { { 2.0f, 10.0f, 0.1f }, 1.0f }
    };
    
    inline const PiecewiseRange<float, 3> riseRange
    {
        { { 0.2f, 1.0f, 0.1f }, 0.15f },
        { { 1.0f, 20.0f, 1.0f }, 0.4f },
        { { 20.0f, 600.0f, 1.0f }, 1.0f }
    };
    
    inline const PiecewiseRange<float, 3> fallRange
    {
        { { 0.0f, 100.0f, 1.0f }, 0.2f },
        { { 100.0f, 1000.0f, 1.0f }, 0.6f },
        { { 1000.0f, 8000.0f, 10.0f }, 1.0f }
    };

    inline const NormalisableRange<float> Range[NumParameters]
    {
        // Input Drive:
        { 0.0f, 32.0f, 0.1f },
        // Time:
        timeRange.getNormalisableRange(),
        // Feedback:
        createSkewedNormalisableRange (0.0f, 120.0f, 0.0f, 50.0f),
        // Invert Feedback:
        { 0.0f, 1.0f, 1.0f },
        // Dry Wet Mix:
        { 0.0f, 100.0f },
        // Type:
        { 0.0f, 2.0f, 1.0f },
        // Output Gain:
        gainRange.getNormalisableRange(),
        // Modulation Rate:
        rateRange.getNormalisableRange(),
        // Modulation Depth:
        { 0.0f, 100.0f },
        // Stereo Spread:
        createSkewedNormalisableRange (0.0f, 100.0f, 0.0f, 30.0f),
        
        // DClipDynamic:
        { 0.0f, 1.0f, 1.0f },
        // DLimRise:
        riseRange.getNormalisableRange(),
        // DLimConstFall:
        fallRange.getNormalisableRange(),
        // DLimFallRange:
        fallRange.getNormalisableRange(),
        // DClipOutputDetector:
        { 0.0f, 1.0f, 1.0f },
        // DClipPostCutFactor:
        { 0.5f, 1.0f, 0.01f },

        // DFbMaxHeadroom:
        { -4.0f, 8.0f, 0.1f },
        // DFbMinHeadroom:
        { -4.0f, 8.0f, 0.1f },
        // DFbOverdrive:
        { 1.0f, 5.0f, 0.1f },
    };

    inline const float DefaultValue[NumParameters]
    {
        0.0f,   // Input Drive
        100.0f, // Time
        50.0f,  // Feedback
        0.0f,   // Invert Feedback
        50.0f,  // Dry Wet
        0.0f,   // Type
        0.0f,   // Output Gain
        1.0f,   // Modulation Rate
        50.0f,  // Modulation Depth
        30.0f,  // Stereo Spread
        
        1.0f,   // DClipDynamic
        20.0f,  // DLimRise
        1200.0f,// DLimConstFall
        2400.0f,// DLimFallRange
        0.0f,   // DClipOutputDetector
        0.65f,  // DClipPostCutFactor

        8.0f,   // DFbMaxHeadroom
        2.0f,   // DFbMinHeadroom
        1.0f,   // DFbOverdrive
    };

    inline const String Label[NumParameters]
    {
        " dB",  // Input Drive
        " ms",  // Time
        " %",   // Feedback
        "",     // Invert Feedback
        " %",   // Dry Wet
        "",     // Type
        " dB",  // Output Gain
        " Hz",  // Modulation Rate
        " %",   // Modulation Depth
        " %",   // Stereo Spread
        
        "",     // DClipDynamic
        " ms",  // DLimRise
        " ms",  // DLimConstFall
        " ms",  // DLimFallRange
        "",     // DClipOutputDetector
        "x",    // DClipPostCutFactor

        " dB",  // DFbMaxHeadroom
        " dB",  // DFbMinHeadroom
        "x",    // DFbOverdrive
    };

    inline const auto stringFromFxTypeValue = [] (float value, int /*maxStringLength*/)
    {
        const auto fxTypeIndex = static_cast<FxType::Index> (value);

        return FxType::Label[fxTypeIndex];
    };

    inline const auto stringFromToggleValue = [] (float value, int /*maxStringLength*/)
    {
        const auto toggleIndex = static_cast<Toggle::Index> (value);

        return Toggle::Label[toggleIndex];
    };

    using stringFromValueFunction = std::function<String(float value,
                                                         int maxStringLength)>;
    
    inline const stringFromValueFunction stringFromValue[NumParameters]
    {
        nullptr,                    // Input Drive
        showDecimalPlaceBelow<100>, // Time
        showDecimalPlaces<0>,       // Feedback
        stringFromToggleValue,      // Invert Feedback
        showDecimalPlaces<0>,       // Dry Wet
        stringFromFxTypeValue,      // Type
        nullptr,                    // Output Gain
        showDecimalPlaces<2>,       // Modulation Rate
        showDecimalPlaces<0>,       // Modulation Depth
        showDecimalPlaceBelow<10>,  // Stereo Spread
        
        stringFromToggleValue,      // DClipDynamic
        showDecimalPlaces<2>,       // DDetRise
        showDecimalPlaces<0>,       // DDetFall
        showDecimalPlaces<0>,       // DClipFall
        stringFromToggleValue,      // DClipOutputDetector
        showDecimalPlaces<2>,       // DClipPostCutFactor

        showDecimalPlaces<1>,       // DFbMaxHeadroom
        showDecimalPlaces<1>,       // DFbMinHeadroom
        showDecimalPlaces<1>,       // DFbOverdrive
    };

    inline const std::initializer_list<float> majorTicks[NumParameters]
    {
        // Input Drive:
        { 0.0f, 12.0f, 24.0f },
        // Time:
        { 1.0f, RBD::maxDelayTimeInMs, 10.0f, 100.0f },
        // Feedback:
        { 0.0f, 120.0f, 50.0f },
        // Invert Feedback:
        {},
        // Dry Wet:
        { 0.0f, 100.0f, 50.0f },
        // Type:
        {},
        // Output Gain:
        { -10.0f, 10.0f, 0.0f },
        // Modulation Rate:
        { 0.01f, 10.0f, 0.1f, 1.0f },
        // Modulation Depth:
        { 0.0f, 100.0f, 50.0f },
        // Stereo Spread:
        {},
        
        {}, {}, {}, {}, {}, {}, {}, {}, {}
    };

    inline const std::initializer_list<float> minorTicks[NumParameters]
    {
        // Input Drive:
        { 2, 4, 6, 8, 10, 14, 16, 18, 20, 22 },
        // Time:
        { 5, 40, 70, 200, 300, 400, 500, 600, 700, 800, 900 },
        // Feedback:
        { 10, 20, 30, 40, 60, 70, 80, 90, 100, 110 },
        // Invert Feedback:
        {},
        // Dry Wet:
        { 10, 20, 30, 40, 60, 70, 80, 90 },
        // Type:
        {},
        // Output Gain:
        { 1, 2, 4, 6, 8, -1, -2, -4, -6, -8 },
        // Modulation Rate:
        { 0.05f, 0.4f, 0.7f, 2, 3, 4, 5, 6, 7, 8, 9 },
        // Modulation Depth:
        { 10, 20, 30, 40, 60, 70, 80, 90 },
        // Stereo Spread:
        {},
        
        {}, {}, {}, {}, {}, {}, {}, {}, {}
    };
} // end namespace Parameter
