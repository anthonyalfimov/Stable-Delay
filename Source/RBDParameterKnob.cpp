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
    jassert (stateToControl.getParameter (Parameter::ID[parameterIndex]) != nullptr);

    // Set up Slider style: rotary knob
    setSliderStyle (SliderStyle::RotaryHorizontalVerticalDrag);
    setTextBoxStyle (Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    if (auto parameter = stateToControl.getParameter (Parameter::ID[parameterIndex]);
        parameter != nullptr)
    {
        setTextValueSuffix (parameter->getLabel());
    }

    // Set up attachment
    using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
    mAttachment = std::make_unique<SliderAttachment> (stateToControl,
                                                      Parameter::ID[parameterIndex],
                                                      *this);
}

ParameterKnob::~ParameterKnob()
{
    
}
