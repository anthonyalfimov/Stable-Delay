/*
  ==============================================================================

    RBDParameterKnob.h
    Created: 13 Jan 2021 9:29:21pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RBDParameters.h"

class ParameterKnob  : public Slider
{
public:
    ParameterKnob (AudioProcessorValueTreeState& stateToControl,
                   Parameter::Index parameterIndex);
    ~ParameterKnob();

private:
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mAttachment;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterKnob)
};
