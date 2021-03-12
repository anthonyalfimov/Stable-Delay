/*
  ==============================================================================

    RBDMeterProbe.cpp
    Created: 12 Mar 2021 6:26:27pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "RBDMeterProbe.h"
#include "RBDAudioConstants.h"

MeterProbe::MeterProbe()
{

}

MeterProbe::~MeterProbe()
{

}

void MeterProbe::process (const float* inAudio, int /*numSamplesToRender*/)
{
    // TODO: Already smoothing audio level in RBDMeter. Why smooth twice?

    // FIXME: This metering depends on the buffer size, sample rate
    //  Here we are not really averaging, and we can miss peaks
    //  JUCE AudioBuffer class has a built-in RMS method - look into it

    float absSampleValue = fabs (inAudio[0]);
    float levelSmoothed = mMeterLevel.load();
    levelSmoothed = levelSmoothed - RBD::meterSmoothingCoef
                                    * (levelSmoothed - absSampleValue);
    mMeterLevel.store (levelSmoothed);
}

const std::atomic<float>* MeterProbe::getMeterLevel() const
{
    return &mMeterLevel;
}
