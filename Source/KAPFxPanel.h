/*
  ==============================================================================

    KAPFxPanel.h
    Created: 11 Jan 2021 4:33:08pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "KAPPanelBase.h"
#include "KAPParameterSlider.h"

// TODO: switch to an enum class, put the enum inside the class

enum KAPFxPanelStyle
{
    kKAPFxPanelStyle_Delay = 0,
    kKAPFxPanelStyle_Chorus,
    kKAPFxPanelStyle_TotalNumStyles
};

class KAPFxPanel  : public KAPPanelBase
{
public:
    KAPFxPanel (KadenzeAudioPluginAudioProcessor* inProcessor);
    ~KAPFxPanel();
    
    void paint (Graphics& g) override;
    
    void setFxPanelStyle (KAPFxPanelStyle inStyle);
    
private:
    KAPFxPanelStyle mStyle;
    
    // TODO: Why use pointers here? Why not create an array of Slider objects?
    //       Objects of this class are dynamically allocated anyway. Perhaps, polymorphism?
    //       Note: some JUCE examples show Sliders, etc, contained as objects, not ptrs.
    OwnedArray<KAPParameterSlider> mSliders;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KAPFxPanel)
};
