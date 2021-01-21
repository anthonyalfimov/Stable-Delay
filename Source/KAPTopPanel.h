/*
  ==============================================================================

    KAPTopPanel.h
    Created: 11 Jan 2021 4:33:47pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "KAPPanelBase.h"

class KAPTopPanel  : public KAPPanelBase,
                     public Button::Listener,
                     public ComboBox::Listener
{
public:
    KAPTopPanel (KadenzeAudioPluginAudioProcessor* inProcessor);
    ~KAPTopPanel();
    
    void paint (Graphics&) override;
    
    void buttonClicked (Button* buttonThatWasPressed) override;
    
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    
private:
    void displaySaveAsPopup();
    void updatePresetList();
    
    std::unique_ptr<ComboBox> mPresetList;
    std::unique_ptr<TextButton> mNewPreset, mSavePreset, mSaveAsPreset;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KAPTopPanel)
};
