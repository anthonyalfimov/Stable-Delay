/*
  ==============================================================================

    RBDDelayModule.h
    Created: 5 Jan 2021 4:58:02pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDAudioConstants.h"

// TODO: unite this with RBDFxPanelStyle, ComboBox index and ID
// TODO: convert to en enum class
enum RBDDelayType
{
    kRBDDelayType_Delay = 0,
    kRBDDelayType_Chorus
};

class DelayModule
{
public:
    DelayModule();
    ~DelayModule();
    
    void setSampleRate (double inSampleRate);
    void reset();
    void process (const float* inAudio,
                  float inTime,
                  float inFeedback,
                  float inWetDry,
                  float inType,
                  const float* inModulationBuffer,
                  float* outAudio,
                  int inNumSamplesToRender);
    
private:
    float getInterpolatedSample (double inDelayTimeInSamples) const;
    
    // TODO: Should we initialise smoothed values to the parameter default instead?
    //       They quickly settle on the parameter value - whether it is set to default or the
    //       recalled plugin state. Are there any audible artifacts due to this settling process?
    //       Otherwise, there might not be a reason to change this.
    double mTimeSmoothed = 0.0; // Use member variable to maintain consistent smoothing between blocks
    double mSampleRate = -1.0;
    float mFeedbackSample = 0.0f;
    // TODO: Dynamically allocate the buffer with fixed duration independent of sample rate
    float mBuffer[RBD::bufferSize];
    
    // TODO: Change variable names to something more descriptive, e.g. write / read head
    int mDelayIndex = 0;
    
    // TODO: JUCE DSP modules don't use the Leak Detector. Should I?
    // JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RBDDelay)
};

