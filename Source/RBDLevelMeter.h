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

// TODO: Consider redesigning the meter as a two-component system
//  A "probe" in the processing thread that analyses the audio buffer and
//  a UI component (basically, this class) that draws the meters.
//  Two components should communicate in a real-time thread-safe manner,
//  i.e. via atomics.
//  NB: pay attention to lifetime - the editor component of this system will
//  get destroyed when the editor is closed, while the processor component
//  will always be running.

#warning Communication with the processor thread is not safe!

class LevelMeter  : public Component,
                    public Timer
{
public:
    LevelMeter (Parameter::Index parameterIndex,
                ReallyBasicDelayAudioProcessor& processor);
    ~LevelMeter();
    
    void paint (Graphics& g) override;
    void timerCallback() override;
        
private:
    const Parameter::Index mParameterIndex;
    
    // TODO: Programmatically handle audio channels in the meter
    //       E.g. mono, mono->stereo
    float mCh0Level = 0.0f;
    float mCh1Level = 0.0f;
    
    ReallyBasicDelayAudioProcessor& mProcessor;
};
