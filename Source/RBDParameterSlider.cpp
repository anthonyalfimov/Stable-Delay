/*
  ==============================================================================

    RBDParameterSlider.cpp
    Created: 17 Mar 2021 3:31:24pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "RBDParameterSlider.h"

ParameterSlider::ParameterSlider (AudioProcessorValueTreeState& stateToControl,
                                  Parameter::Index parameterIndex)
    : Slider (Parameter::Name[parameterIndex])
{
    // Set up Slider style: horizontal slider
    setSliderStyle (SliderStyle::LinearHorizontal);
    setTextBoxStyle (Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    setSliderSnapsToMousePosition (false);
    auto parameter = stateToControl.getParameter (Parameter::ID[parameterIndex]);
    jassert (parameter != nullptr);

    if (parameter != nullptr)
        setTextValueSuffix (parameter->getLabel());

    // Set up attachment
    using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
    mAttachment = std::make_unique<SliderAttachment> (stateToControl,
                                                      Parameter::ID[parameterIndex],
                                                      *this);
}

ParameterSlider::~ParameterSlider()
{

}
