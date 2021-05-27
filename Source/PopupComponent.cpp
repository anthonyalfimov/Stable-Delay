/*
  ==============================================================================

    PopupComponent.cpp
    Created: 27 May 2021 4:14:54pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "PopupComponent.h"

PopupComponent::PopupComponent (Component* parentComponent,
                                std::unique_ptr<PopupComponent>& owner)
    : mOwner (owner)
{
    if (parentComponent == nullptr)
    {
        jassertfalse;
        return;
    }

    setAlwaysOnTop (true);
    parentComponent->addAndMakeVisible (this);
}

void PopupComponent::updateBounds()
{
    if (getParentComponent() != nullptr)
        setBounds (getParentComponent()->getLocalBounds());
}

void PopupComponent::parentSizeChanged()
{
    updateBounds();
}

void PopupComponent::mouseDown (const MouseEvent& /*event*/)
{
    mOwner.reset();
}
