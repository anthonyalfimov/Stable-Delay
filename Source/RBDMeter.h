/*
  ==============================================================================

    RBDMeter.h
    Created: 29 Jan 2021 6:29:13pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "RBDParameters.h"

// TODO: Consider storing the channel levels in the APVTS
//  These would need to be hidden from the host or marked as a special type of
//  parameter that can't be changed ("analysisMeter" tag). But this would solve
//  the question of communicating between the processor and the editor.

class Meter  : public Component,
               private Timer
{
public:
    Meter (Parameter::Index parameterIndex,
           ReallyBasicDelayAudioProcessor& processor);
    ~Meter();

    enum class Style
    {
        Normal = 0,
        Clipping,
        Saturation
    };

    void setStyle (Style newStyle);

    //==============================================================================
    /** @internal */
    void paint (Graphics& g) override;
    /** @internal */
    void timerCallback() override;
    /** @internal */
    void mouseDown (const MouseEvent& event) override;

    inline static constexpr int refreshRate = 15; // Hz

    // Additional padding to Meter component size needed to accomodate the
    //  clipping indicator
    inline static constexpr int padding = 4;
        
private:
    const Parameter::Index mParameterIndex;
    Style mStyle = Style::Normal;
    int mNumChannels = 0;

    std::vector<MeterProbe*> mMeterProbes;
    std::vector<SmoothedValue<float>> mPeakLevelsInDb;
    std::vector<SmoothedValue<float>> mRmsLevelsInDb;
    std::vector<bool> mShowClippingIndicator;

    // Note: remove "static" if input and output meter have different or
    //  adjustable ranges
    inline static const float meterReleaseTime = 0.2f;
    inline static const float minLevelInDb = -36.0f;
    inline static const float maxLevelInDb = 0.0f;
    inline static const NormalisableRange<float> meterRange
    {
        minLevelInDb, maxLevelInDb
    };
};
