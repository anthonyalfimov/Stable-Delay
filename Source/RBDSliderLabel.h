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
    A component that displays Slider name or Slider value, is the value is
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
    SliderLabel (Slider* ownerSlider);
    ~SliderLabel();

    //==============================================================================
    /** @internal */
    void paint (Graphics& g) override;
    /** @internal */
    void componentMovedOrResized (Component& component,
                                  bool wasMoved,
                                  bool wasResized) override;

    /** @internal */
    void timerCallback() override;

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

private:
    Slider* mOwnerSlider;

    String mSliderName;
    Colour mBgColour = RBD::controlNormalColour;

    // Time before slider value display switches back to slider name
    inline static const int switchDelayTime = 500 /* ms */;
};
