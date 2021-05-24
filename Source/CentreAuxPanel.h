/*
  ==============================================================================

    CentreAuxPanel.h
    Created: 11 Jan 2021 4:32:37pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "InterfacePanel.h"
#include "ParameterSlider.h"
#include "SliderLabel.h"

class CentreAuxPanel  : public InterfacePanel
{
public:
    explicit CentreAuxPanel (ReallyBasicDelayAudioProcessor& processor);
    
private:
    std::unique_ptr<ParameterSlider> mSpreadSlider;
    std::unique_ptr<SliderLabel> mSpreadLabel;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CentreAuxPanel)
};
