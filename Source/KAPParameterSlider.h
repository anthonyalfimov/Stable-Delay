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
    // TODO: we should be able to get the parameter label knowing its ID, without needing to pass it
    KAPParameterSlider (AudioProcessorValueTreeState& stateToControl,
                        const String& parameterID,
                        const String& parameterLabel);
    ~KAPParameterSlider();
    
private:
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KAPParameterSlider)
};
