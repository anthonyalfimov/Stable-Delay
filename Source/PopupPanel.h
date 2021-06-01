/*
  ==============================================================================

    PopupPanel.h
    Created: 27 May 2021 4:14:54pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "InterfaceConstants.h"

/**
    Abstract base class for a component that covers the whole bounds of its parent,
    always stays on top, and self-destructs on mouse click.

    @Discussion
    You must call the `updateBounds()` method after adding an object of this
    class to a parent.

    Override `resized()` to set `panelBounds` to desired size and position.
*/
class PopupPanel  : public Component
{
public:
    PopupPanel (std::unique_ptr<PopupPanel>& owner);

    /**
        Update PopupPanel bounds to cover its parent Component.
    */
    void updateBounds();

    /**
        Dismiss the PopupPanel by triggering its owner to delete it.
    */
    virtual void dismiss();

//==============================================================================
    /**
        @internal
        Override `resized()` to set `panelBounds` to desired size and position.
    */
    virtual void resized() override = 0;
    
    /** @internal */
    void parentSizeChanged() override;
    /** @internal */
    void paint (Graphics& g) override;
    /** @internal */
    void mouseDown (const MouseEvent& event) override;

protected:
    Rectangle<int> panelBounds;
    Colour panelColour = RBD::popupBgColour;
    int panelBorderSize = 0;
    float panelCornerSize = RBD::defaultCornerSize;
    DropShadow panelShadow { RBD::shadowColour, 8, { 3, 3 } };

private:
    std::unique_ptr<PopupPanel>& mOwner;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PopupPanel)
};
