/*
  ==============================================================================

    ParameterToggle.cpp
    Created: 27 Apr 2021 10:26:55pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "ParameterToggle.h"

ParameterToggle::ParameterToggle (AudioProcessorValueTreeState& stateToControl,
                                  Parameter::Index parameterIndex)
    : ToggleButton (Parameter::Name[parameterIndex])
{
    jassert (stateToControl.getParameter (Parameter::ID[parameterIndex]) != nullptr);

    // Set up attachment
    using ButtonAttachment = AudioProcessorValueTreeState::ButtonAttachment;
    mAttachment = std::make_unique<ButtonAttachment> (stateToControl,
                                                      Parameter::ID[parameterIndex],
                                                      *this);
}

