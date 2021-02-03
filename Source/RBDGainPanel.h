/*
  ==============================================================================

    RBDGainPanel.h
    Created: 11 Jan 2021 4:33:22pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDPanelBase.h"
#include "RBDParameters.h"
#include "RBDParameterSlider.h"
#include "RBDMeter.h"

class RBDGainPanel  : public RBDPanelBase
{
public:
    RBDGainPanel (ReallyBasicDelayAudioProcessor* inProcessor);
    ~RBDGainPanel();
    
    // TODO: Consider setting parameter ID for GainPanel in its constructor
    void setParameterID (RBDParameter inParameter);
    
    void paint (Graphics& g) override;
    
    void mouseEnter (const MouseEvent& event) override;
    void mouseExit (const MouseEvent& event) override;
    
private:
    std::unique_ptr<RBDParameterSlider> mSlider;
    std::unique_ptr<RBDMeter> mMeter;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RBDGainPanel)
};
