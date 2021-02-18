/*
  ==============================================================================

    RBDParameterSlider.cpp
    Created: 13 Jan 2021 9:29:21pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDParameterSlider.h"

ParameterSlider::ParameterSlider (AudioProcessorValueTreeState& stateToControl,
                                  Parameter::Index parameterIndex)
    : Slider (Parameter::Label[parameterIndex])
{
    setSliderStyle (SliderStyle::RotaryHorizontalVerticalDrag);
    setTextBoxStyle (Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    
    setRange (0.0f, 1.0f, 0.001f);
    
    using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
    mAttachment = std::make_unique<SliderAttachment> (stateToControl,
                                                      Parameter::ID[parameterIndex],
                                                      *this);
}

ParameterSlider::~ParameterSlider()
{
    
}
