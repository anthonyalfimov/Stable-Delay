/*
  ==============================================================================

    Meter.h
    Created: 29 Jan 2021 6:29:13pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MeterChannel.h"
#include "MeterLegend.h"
#include "Parameters.h"

class Meter  : public Component,
               private Timer
{
public:
    Meter (Parameter::Index parameterIndex,
           ReallyBasicDelayAudioProcessor& processor,
           int numChannels);
    ~Meter();

//==============================================================================
    void setStyle (MeterStyle newStyle);

//==============================================================================
    /** @internal */
    void resized() override;
    /** @internal */
    void timerCallback() override;
    /** @internal */
    void mouseDown (const MouseEvent& event) override;

//==============================================================================
    inline static constexpr int refreshRate = 15; // Hz

    // Additional padding to Meter component size needed to accomodate the
    //  clipping indicator
    inline static constexpr int padding = MeterChannel::padding;
        
private:
    const int mNumChannels;
    MeterStyle mStyle = MeterStyle::Normal;

    std::unique_ptr<MeterLegend> mMeterLegend;
    OwnedArray<MeterChannel> mMeterChannels;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Meter)
};
