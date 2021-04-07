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
    jassert (stateToControl.getParameter (Parameter::ID[parameterIndex]) != nullptr);

    // Set up Slider style: horizontal slider
    setSliderStyle (SliderStyle::LinearHorizontal);
    setTextBoxStyle (Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    setSliderSnapsToMousePosition (false);

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

ParameterSlider::~ParameterSlider()
{

}
