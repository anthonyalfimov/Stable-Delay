/*
  ==============================================================================

    MeterChannel.h
    Created: 24 Mar 2021 3:52:12pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MeterProbe.h"

enum class MeterStyle
{
    Normal = 0,
    Clipping,
    Saturation
};

// TODO: Consider storing the channel levels in the APVTS
//  Create a parameter with parameterCategory set to
//  AudioProcessorParameter::inputMeter or AudioProcessorParameter::outputMeter.
//  These categories tell the host that this parameter is a meter level value
//  and therefore read-only. Most hosts will display these type of parameters
//  as a meter in the generic view of the plug-in.
//
// However, what will happen if we add both peak and RMS levels with these
//  categories? Will the hosts be able to handle this?

class MeterChannel  : public Component
{
public:
    MeterChannel (MeterProbe* meterProbe, int refreshRate);
    ~MeterChannel();

    void setStyle (MeterStyle newStyle);
    void resetClippingIndicator();

    /** Update channel meter reading.

        @Discussion
        Timer callback in the meter object should call this function at the
        refresh rate supplied to the MeterChannel constructor.
    */
    void update();

//==============================================================================
    /** @internal */
    void paint (Graphics& g) override;

    // Additional padding to MeterChannel component size needed to accomodate
    //  the clipping indicator
    inline static constexpr int padding = 2;

    // Reduction amount for the bounds of the meter bars inside the meter channel
    inline static constexpr int inset = 2;

//==============================================================================
    inline static const float minLevelInDb = -36.0f;
    inline static const float maxLevelInDb = 4.0f;

    inline static const NormalisableRange<float> range
    {
        minLevelInDb, maxLevelInDb
    };

private:
    MeterStyle mStyle = MeterStyle::Normal;
    MeterProbe* mMeterProbe = nullptr;
    const int mRefreshRate;

    SmoothedValue<float> mPeakLevelInDb;
    SmoothedValue<float> mRmsLevelInDb;
    bool mShowClippingIndicator = false;
    
    // Note: remove "static" if input and output meter have different or
    //  adjustable ranges
    inline static const float meterReleaseTime = 0.2f; // seconds

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterChannel)
};

