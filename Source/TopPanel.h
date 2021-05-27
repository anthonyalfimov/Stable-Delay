/*
  ==============================================================================

    TopPanel.h
    Created: 11 Jan 2021 4:33:47pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "InterfacePanel.h"
#include "TitleComponent.h"
#include "MouseEventInvoker.h"

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
    
private:
    void displaySaveAsPopup();
    void updatePresetList();

    MouseEventInvoker mPresetListMouseEventInvoker;
    std::unique_ptr<ComboBox> mPresetList;

    std::unique_ptr<TextButton> mNewPreset, mSavePreset, mSaveAsPreset;

    std::unique_ptr<TitleComponent> mTitleComponent;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopPanel)
};
