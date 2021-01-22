/*
  ==============================================================================

    KAPParameterSlider.cpp
    Created: 13 Jan 2021 9:29:21pm
    Author:  Anthony

  ==============================================================================
*/

#include "KAPParameterSlider.h"

KAPParameterSlider::KAPParameterSlider (AudioProcessorValueTreeState& stateToControl,
                                        const String& parameterID,
                                        const String& parameterLabel)
    : Slider (parameterLabel)
{
    setSliderStyle (SliderStyle::RotaryHorizontalVerticalDrag);
    setTextBoxStyle (Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    
    setRange (0.0f, 1.0f, 0.001f);
    
    mAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (stateToControl,
                                                                                    parameterID,
                                                                                    *this);
}

KAPParameterSlider::~KAPParameterSlider()
{
    
}
