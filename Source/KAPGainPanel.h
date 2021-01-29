/*
  ==============================================================================

    KAPGainPanel.h
    Created: 11 Jan 2021 4:33:22pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "KAPPanelBase.h"
#include "KAPParameters.h"
#include "KAPParameterSlider.h"

class KAPGainPanel  : public KAPPanelBase
{
public:
    KAPGainPanel (KadenzeAudioPluginAudioProcessor* inProcessor);
    ~KAPGainPanel();
    
    // TODO: Consider setting parameter ID for GainPanel in its constructor
    void setParameterID (KAPParameter inParameter);
    
    void paint (Graphics& g) override;
    
    void mouseEnter (const MouseEvent& event) override;
    void mouseExit (const MouseEvent& event) override;
    
private:
    std::unique_ptr<KAPParameterSlider> mSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KAPGainPanel)
};
