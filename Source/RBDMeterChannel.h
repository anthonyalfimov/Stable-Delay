/*
  ==============================================================================

    RBDMeterChannel.h
    Created: 24 Mar 2021 3:52:12pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RBDMeterProbe.h"

enum class MeterStyle
{
    Normal = 0,
    Clipping,
    Saturation
};

// TODO: Consider storing the channel levels in the APVTS
//  These would need to be hidden from the host or marked as a special type of
//  parameter that can't be changed ("analysisMeter" tag). But this would solve
//  the question of communicating between the processor and the editor.

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

private:
    MeterStyle mStyle = MeterStyle::Normal;
    MeterProbe* mMeterProbe = nullptr;
    const int mRefreshRate;

    SmoothedValue<float> mPeakLevelInDb;
    SmoothedValue<float> mRmsLevelInDb;
    bool mShowClippingIndicator = false;
    
    // Note: remove "static" if input and output meter have different or
    //  adjustable ranges
    inline static const float meterReleaseTime = 0.2f;
    inline static const float minLevelInDb = -36.0f;
    inline static const float maxLevelInDb = 0.0f;
    inline static const NormalisableRange<float> meterRange
    {
        minLevelInDb, maxLevelInDb
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterChannel);
};

