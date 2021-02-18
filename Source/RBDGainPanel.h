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
#include "RBDParameterSlider.h"
#include "RBDLevelMeter.h"

class GainPanel  : public InterfacePanel
{
public:
    GainPanel (ReallyBasicDelayAudioProcessor* inProcessor);
    ~GainPanel();
    
    // TODO: Consider setting parameter ID for GainPanel in its constructor
    void setParameterID (Parameter::Index parameterIndex);
    
    void paint (Graphics& g) override;
    
    void mouseEnter (const MouseEvent& event) override;
    void mouseExit (const MouseEvent& event) override;
    
private:
    std::unique_ptr<ParameterSlider> mSlider;
    std::unique_ptr<LevelMeter> mMeter;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainPanel)
};
