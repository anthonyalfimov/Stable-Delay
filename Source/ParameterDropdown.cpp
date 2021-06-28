/*
  ==============================================================================

 ParameterDropdown.cpp
    Created: 13 Jan 2021 9:29:35pm
    Author:  Anthony

  ==============================================================================
*/

#include "ParameterDropdown.h"
#include "MainPanel.h"
#include "PluginLookAndFeel.h"

//==============================================================================
//  DropdownMenu
//==============================================================================

// TODO: Should we check whether the owner-dropdown pointer is null?
//  The intended use of this class is for the owner-dropdown to own the
//  DropdownMenu instance. So it can't be null without the DropdownMenu itself
//  being destroyed.
//  However, if DropdownMenu class is not private, it could be used differently.

DropdownMenu::DropdownMenu (ParameterDropdown* dropdown, int radioGroupId)
    : PopupPanel (dropdown->getDropdownMenu()),
      mDropdown (dropdown),
      mRadioGroupId (radioGroupId)
{
    // Set up PopupPanel parameters
    panelBorderSize = 1;

    // Set up keyboard focus
    setWantsKeyboardFocus (true);

    // Add DropdownMenu on top of the MainPanel component containing our owner-dropdown
    auto* parent = dropdown->findParentComponentOfClass<MainPanel>();

    if (parent == nullptr)
    {
        dropdown->hidePopup();
        jassertfalse;
        return;
    }

    parent->addAndMakeVisible (this);

    updateBounds();
    buildMenu();
    resized();
    grabKeyboardFocus();
}

void DropdownMenu::dismiss()
{
    mDropdown->hidePopup();
    mDropdown->grabKeyboardFocus(); // return keyboard focus to parent Dropdown
    PopupPanel::dismiss();
}

void DropdownMenu::resized()
{
    // Calculate the Menu position
    if (getParentComponent() == nullptr)
    {
        jassertfalse;
        return;
    }

    auto bounds
    = getParentComponent()->getLocalArea (mDropdown, mDropdown->getLocalBounds());

    const auto padding = PluginLookAndFeel::comboBoxButtonWidth;
    bounds.removeFromLeft (padding);
    bounds.removeFromRight (padding);

    const int itemHeight = bounds.getHeight();
    const int panelHeight = numItems * itemHeight + (numItems - 1) * panelBorderSize;
    bounds.setHeight (panelHeight);
    panelBounds = bounds;

    // Set bounds of the items
    for (auto item : mItems)
    {
        item->setBounds (bounds.removeFromTop (itemHeight));
        bounds.removeFromTop (panelBorderSize);
    }
}

void DropdownMenu::buildMenu()
{
    mItems.clear();
    numItems = mDropdown->getNumItems();

    for (int i = 0; i < numItems; ++i)
    {
        auto* item
        = mItems.add (std::make_unique<TextButton> (mDropdown->getItemText (i)));

        // Set up as part of toggle radio button group
        item->setClickingTogglesState (true);
        item->setRadioGroupId (mRadioGroupId, dontSendNotification);

        // Set up button colours
        item->setColour (TextButton::buttonColourId, RBD::popupItemColour);
        item->setColour (TextButton::buttonOnColourId, RBD::popupItemHoverColour);
        item->setColour (TextButton::textColourOffId, RBD::textFxTypeColour);
        item->setColour (TextButton::textColourOnId, RBD::textNormalColour);

        // Set up on-click callback
        item->onClick = [i, item, this] ()
        {
            if (item->getToggleState())
            {
                mDropdown->setSelectedItemIndex (i);
                this->dismiss();
            }
        };

        // Set connected flags
        bool isConnectedOnTop = (i != 0);
        bool isConnectedOnBottom = (i != (numItems - 1));

        int connectedEdgeFlags = (isConnectedOnTop ? Button::ConnectedOnTop : 0)
                               | (isConnectedOnBottom ? Button::ConnectedOnBottom : 0);

        item->setConnectedEdges (connectedEdgeFlags);

        // Add to parent
        addAndMakeVisible (item);
    }

    // Toggle selected item
    const auto selectedIndex = mDropdown->getSelectedItemIndex();
    mItems[selectedIndex]->setToggleState (true, dontSendNotification);
}

//==============================================================================
//  ParameterDropdown
//==============================================================================

ParameterDropdown::ParameterDropdown (AudioProcessorValueTreeState& stateToControl,
                                      Parameter::Index parameterIndex,
                                      int radioGroupId,
                                      const StringArray& itemList,
                                      int firstItemIdOffset)
    : ComboBox (Parameter::Name[parameterIndex]), mRadioGroupId (radioGroupId)
{
    // Set up style
    setEditableText (false);
    setColour (ComboBox::textColourId, RBD::textFxTypeColour);

    // Set up item list
    addItemList (itemList, firstItemIdOffset);

    // Set up parameter attachment
    jassert (stateToControl.getParameter (Parameter::ID[parameterIndex]) != nullptr);

    using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;
    mAttachment = std::make_unique<ComboBoxAttachment> (stateToControl,
                                                        Parameter::ID[parameterIndex],
                                                        *this);
}

void ParameterDropdown::showPopup()
{
    mMenu = std::make_unique<DropdownMenu> (this, mRadioGroupId);
}

void ParameterDropdown::mouseEnter (const MouseEvent& event)
{
    ComboBox::mouseEnter (event);
    repaint();
}

void ParameterDropdown::mouseExit (const MouseEvent& event)
{
    ComboBox::mouseExit (event);
    repaint();
}
