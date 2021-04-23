/*
  ==============================================================================

    ParameterSlider.h
    Created: 17 Mar 2021 3:31:24pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Parameters.h"

// TODO: Add custom graphics for the Linear Slider

class ParameterSlider  : public Slider
{
public:
    ParameterSlider (AudioProcessorValueTreeState& stateToControl,
                     Parameter::Index parameterIndex);

private:
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mAttachment;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterSlider)
};
