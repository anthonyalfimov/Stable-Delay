/*
  ==============================================================================

    RBDParameterKnob.cpp
    Created: 13 Jan 2021 9:29:21pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDParameterKnob.h"

ParameterKnob::ParameterKnob (AudioProcessorValueTreeState& stateToControl,
                              Parameter::Index parameterIndex)
    : Slider (Parameter::Name[parameterIndex])
{
    // Set up common Slider style
    setSliderStyle (SliderStyle::RotaryHorizontalVerticalDrag);
    setTextBoxStyle (Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    auto parameter = stateToControl.getParameter (Parameter::ID[parameterIndex]);
    setTextValueSuffix (parameter->getLabel());
    
    using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
    mAttachment = std::make_unique<SliderAttachment> (stateToControl,
                                                      Parameter::ID[parameterIndex],
                                                      *this);
}

ParameterKnob::~ParameterKnob()
{
    
}
