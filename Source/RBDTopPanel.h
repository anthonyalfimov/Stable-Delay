/*
  ==============================================================================

    RBDTopPanel.h
    Created: 11 Jan 2021 4:33:47pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "RBDPanelBase.h"

class RBDTopPanel  : public RBDPanelBase,
                     public Button::Listener,
                     public ComboBox::Listener
{
public:
    RBDTopPanel (ReallyBasicDelayAudioProcessor* inProcessor);
    ~RBDTopPanel();
    
    void paint (Graphics&) override;
    
    void buttonClicked (Button* buttonThatWasPressed) override;
    
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    
private:
    void displaySaveAsPopup();
    void updatePresetList();
    
    std::unique_ptr<ComboBox> mPresetList;
    std::unique_ptr<TextButton> mNewPreset, mSavePreset, mSaveAsPreset;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RBDTopPanel)
};
