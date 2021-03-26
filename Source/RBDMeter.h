/*
  ==============================================================================

    RBDMeter.h
    Created: 29 Jan 2021 6:29:13pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RBDMeterChannel.h"
#include "PluginProcessor.h"
#include "RBDParameters.h"

class Meter  : public Component,
               private Timer
{
public:
    Meter (Parameter::Index parameterIndex,
           ReallyBasicDelayAudioProcessor& processor);
    ~Meter();

    void setStyle (MeterStyle newStyle);

//==============================================================================
    /** @internal */
    void resized() override;
    /** @internal */
    void timerCallback() override;
    /** @internal */
    void mouseDown (const MouseEvent& event) override;

    inline static constexpr int refreshRate = 15; // Hz

    // Additional padding to Meter component size needed to accomodate the
    //  clipping indicator
    inline static constexpr int padding = MeterChannel::padding;
        
private:
    const Parameter::Index mParameterIndex;
    int mNumChannels = 0;
    MeterStyle mStyle = MeterStyle::Normal;

    OwnedArray<MeterChannel> mMeterChannels;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Meter)
};
