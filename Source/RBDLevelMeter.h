/*
  ==============================================================================

    RBDLevelMeter.h
    Created: 29 Jan 2021 6:29:13pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "RBDParameters.h"

class LevelMeter  : public Component,
                    public Timer
{
public:
    LevelMeter (Parameter::Index inParameter, ReallyBasicDelayAudioProcessor* inProcessor);
    ~LevelMeter();
    
    void paint (Graphics& g) override;
    void timerCallback() override;
        
private:
    const Parameter::Index mParameter;
    
    // TODO: Programmatically handle channels to allow for different channel configurations
    //       E.g. mono, mono->stereo
    float mCh0Level = 0.0f;
    float mCh1Level = 0.0f;
  
    ReallyBasicDelayAudioProcessor* mProcessor = nullptr;
};
