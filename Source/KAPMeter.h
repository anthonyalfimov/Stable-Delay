/*
  ==============================================================================

    KAPMeter.h
    Created: 29 Jan 2021 6:29:13pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "KAPInterfaceDefines.h"

class KAPMeter  : public Component,
                  public Timer
{
public:
    KAPMeter (int inParameter, KadenzeAudioPluginAudioProcessor* inProcessor);
    ~KAPMeter();
    
    void paint (Graphics& g) override;
    void timerCallback() override;
        
private:
    const int mParameter;
    
    // TODO: Programmatically handle channels to allow for different channel configurations
    //       E.g. mono, mono->stereo
    float mCh0Level = 0.0f;
    float mCh1Level = 0.0f;
  
    KadenzeAudioPluginAudioProcessor* mProcessor = nullptr;
};
