/*
  ==============================================================================

    RBDFxPanel.h
    Created: 11 Jan 2021 4:33:08pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDInterfacePanel.h"
#include "RBDParameterSlider.h"

// TODO: Clean up usage of RBDFxPanelStyle, ComboBox index and ID

enum RBDFxPanelStyle
{
    kRBDFxPanelStyle_Delay = 0,
    kRBDFxPanelStyle_Chorus,
    kRBDFxPanelStyle_TotalNumStyles
};

class FxPanel  : public InterfacePanel,
                    public ComboBox::Listener
{
public:
    FxPanel (ReallyBasicDelayAudioProcessor* inProcessor);
    ~FxPanel();
    
    void paint (Graphics& g) override;
    
    void setFxPanelStyle (RBDFxPanelStyle inStyle);
    
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    
    void mouseEnter (const MouseEvent& event) override;
    void mouseExit (const MouseEvent& event) override;
    
private:
    RBDFxPanelStyle mStyle;
    
    // TODO: Why use pointers here? Why not create an array of Slider objects?
    //       Objects of this class are dynamically allocated anyway. Perhaps, polymorphism?
    //       Note: some JUCE examples show Sliders, etc, contained as objects, not ptrs.
    OwnedArray<ParameterSlider> mSliders;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FxPanel)
};
