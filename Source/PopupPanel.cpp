/*
  ==============================================================================

    PopupPanel.cpp
    Created: 27 May 2021 4:14:54pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "PopupPanel.h"

PopupPanel::PopupPanel (std::unique_ptr<PopupPanel>& owner)
    : mOwner (owner)
{
    setAlwaysOnTop (true);
}

void PopupPanel::updateBounds()
{
    if (getParentComponent() != nullptr)
        setBounds (getParentComponent()->getLocalBounds());
}

void PopupPanel::dismiss()
{
    mOwner.reset();
}

void PopupPanel::parentSizeChanged()
{
    updateBounds();
}

void PopupPanel::mouseDown (const MouseEvent& /*event*/)
{
    dismiss();
}
