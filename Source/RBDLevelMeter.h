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

// TODO: Consider storing the channel levels in the APVTS
//  These would need to be hidden from the host or marked as a special type of
//  parameter that can't be changed ("analysisMeter" tag). But this would solve
//  the question of communicating between the processor and the editor.

class LevelMeter  : public Component,
                    private Timer
{
public:
    LevelMeter (Parameter::Index parameterIndex,
                ReallyBasicDelayAudioProcessor& processor);
    ~LevelMeter();

    //==============================================================================
    /** @internal */
    void paint (Graphics& g) override;
    /** @internal */
    void timerCallback() override;
        
private:
    const Parameter::Index mParameterIndex;
    
    // TODO: Programmatically handle audio channels in the meter
    //       E.g. mono, mono->stereo
    float mCh0Level = 0.0f;
    float mCh1Level = 0.0f;
    
    ReallyBasicDelayAudioProcessor& mProcessor;
};
