/*
  ==============================================================================

    PopupComponent.h
    Created: 27 May 2021 4:14:54pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/**
    A component that covers the whole bounds of its parent, always stays on top,
    and self-destructs on mouse click.

    @Discussion
    You must call the updateBounds() method after creating an object of this
    class (or in the constructor of your derived class) to initialise the bounds.
*/
class PopupComponent  : public Component
{
public:
    PopupComponent (Component* parentComponent,
                    std::unique_ptr<PopupComponent>& owner);

    /**
        Update PopupComponent bounds to cover its parent Component.
    */
    void updateBounds();

//==============================================================================
    /** @internal */
    void parentSizeChanged() override;
    /** @internal */
    void mouseDown (const MouseEvent& event) override;

private:
    std::unique_ptr<PopupComponent>& mOwner;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PopupComponent)
};
