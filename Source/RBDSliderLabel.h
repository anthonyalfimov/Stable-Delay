/*
  ==============================================================================

    RBDSliderLabel.h
    Created: 3 Mar 2021 6:25:25pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RBDInterfaceConstants.h"

/**
    A component that displays Slider name or Slider value, if the value is
    being changed. Label background becomes highlighted when the mouse is over
    the parent Slider, but not the Label itself.

    @Discussion
    Make sure that the SliderLabel object is destroyed before the Slider it is
    attached to.
*/
class SliderLabel  : public Label,
                     private Timer
{
public:
    explicit SliderLabel (Slider* ownerSlider, bool onLeft = false);
    ~SliderLabel();

//==============================================================================
    /** @internal */
    void componentMovedOrResized (Component& component,
                                  bool wasMoved,
                                  bool wasResized) override;

    /** @internal */
    void mouseEnter (const MouseEvent& event) override;
    /** @internal */
    void mouseExit (const MouseEvent& event) override;
    /** @internal */
    void mouseDown (const MouseEvent& event) override;
    /** @internal */
    void mouseDrag (const MouseEvent& event) override;
    /** @internal */
    void mouseUp (const MouseEvent& event) override;
    /** @internal */
    void mouseDoubleClick (const MouseEvent& event) override;

    /** @internal */
    void timerCallback() override;

private:
    Slider* mOwnerSlider;

    String mSliderName;
    const bool isLeftOfOwner;

    // This time should cover double clicking
    // TODO: Consider deriving the delay time from current double click speed
    
    // Time before slider value display switches back to slider name
    inline static const int switchDelayTimeInMs = 1000 /* ms */;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderLabel)
};
