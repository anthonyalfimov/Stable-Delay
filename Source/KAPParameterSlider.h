/*
  ==============================================================================

    KAPParameterSlider.h
    Created: 13 Jan 2021 9:29:21pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class KAPParameterSlider  : public Slider
{
public:
    KAPParameterSlider (AudioProcessorValueTreeState& stateToControl, const String& parameterID);
    ~KAPParameterSlider();
    
private:
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mAttachment;
    
};
