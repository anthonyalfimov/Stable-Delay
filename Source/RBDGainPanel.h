/*
  ==============================================================================

    RBDGainPanel.h
    Created: 11 Jan 2021 4:33:22pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDInterfacePanel.h"
#include "RBDParameters.h"
#include "RBDParameterKnob.h"
#include "RBDSliderLabel.h"
#include "RBDLevelMeter.h"

class GainPanel  : public InterfacePanel
{
public:
    GainPanel (ReallyBasicDelayAudioProcessor& processor,
               Parameter::Index parameterIndex);
    ~GainPanel();
    
private:
    std::unique_ptr<ParameterKnob> mKnob;
    std::unique_ptr<SliderLabel> mLabel;
    std::unique_ptr<LevelMeter> mMeter;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainPanel)
};
