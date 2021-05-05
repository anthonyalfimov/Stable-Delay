/*
  ==============================================================================

    TopPanel.h
    Created: 11 Jan 2021 4:33:47pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "InterfacePanel.h"
#include "HighlightableComboBox.h"

class TopPanel  : public InterfacePanel,
                  public Button::Listener,
                  public ComboBox::Listener
{
public:
    explicit TopPanel (ReallyBasicDelayAudioProcessor& processor);

//==============================================================================
    /** @internal */
    void buttonClicked (Button* buttonThatWasPressed) override;
    /** @internal */
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
//==============================================================================
    /** @internal
        Registers a listener to be told when mouse events occur in the Title
        Label. Returns the address of the Title Label you registered with.
    */
    Component* addTitleMouseListener (MouseListener* listener);
    /** @internal */
    void removeTitleMouseListener (MouseListener* listener);
    
private:
    void displaySaveAsPopup();
    void updatePresetList();
    
    std::unique_ptr<HighlightableComboBox> mPresetList;
    std::unique_ptr<TextButton> mNewPreset, mSavePreset, mSaveAsPreset;

    Label mTitleLabel { "title", "Really Basic Delay" };

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopPanel)
};
