/*
  ==============================================================================

    ParameterDropdown.h
    Created: 13 Jan 2021 9:29:35pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include "PopupPanel.h"
#include "MouseEventInvoker.h"
#include "InterfaceConstants.h"
#include "Parameters.h"

// TODO: Extract a dropdown base class to be also used for the preset browser

class ParameterDropdown;

class DropdownMenu  : public PopupPanel
{
public:
    explicit DropdownMenu (ParameterDropdown* dropdown);

//==============================================================================
    /** @internal */
    void dismiss() override;
    /** @internal */
    void resized() override;
    /** @internal */
    void paint (Graphics& g) override;

protected:
    // TODO: Place these members as public/protected in the Popup base class
    Rectangle<int> panelBounds;
    Colour panelColour = RBD::popupBgColour;
    int panelBorderSize = 1;
    float panelCornerSize = RBD::defaultCornerSize;

//==============================================================================
    SafePointer<ParameterDropdown> mDropdown;

private:
    OwnedArray<TextButton> mItems;
    int numItems = 0;

    // MARK: Temporary Radio Group ID
    int radioGroupId = 1001;

//==============================================================================
    void buildMenu();

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DropdownMenu)
};

class ParameterDropdown  : public ComboBox
{
public:
    /**
        Create a ComboBox that will be attached to the parameter in
        stateToControl that is identified by the parameterIndex.

        @Discussion
        Initial ComboBox update happens when attachment is created, so all
        ComboBox items must be supplied to the constructor. The constructor
        adds the supplied items to the ComboBox, and then creates the parameter
        attachment.
    */
    ParameterDropdown (AudioProcessorValueTreeState& stateToControl,
                       Parameter::Index parameterIndex,
                       const StringArray& itemList,
                       int firstItemIdOffset = 1);

    void showPopup() override;

    std::unique_ptr<PopupPanel>& getDropdownMenu() { return mMenu; }

private:
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> mAttachment;

//==============================================================================
    std::unique_ptr<PopupPanel> mMenu;
    
//==============================================================================
    MouseEventInvoker mMouseEventInvoker;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterDropdown)
};
