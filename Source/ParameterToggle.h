/*
  ==============================================================================

    ParameterToggle.h
    Created: 27 Apr 2021 10:26:55pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Parameters.h"

class ParameterToggle  : public Button
{
public:
    ParameterToggle (AudioProcessorValueTreeState& stateToControl,
                     Parameter::Index parameterIndex);

private:
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> mAttachment;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterToggle)
};
