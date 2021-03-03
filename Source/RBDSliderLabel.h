/*
  ==============================================================================

    RBDSliderLabel.h
    Created: 3 Mar 2021 6:25:25pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SliderLabel  : public Label
{
public:
    SliderLabel (Slider* ownerSlider);
    ~SliderLabel();

    /** @internal */
    void paint (Graphics& g) override;
    /** @internal */
    void componentMovedOrResized (Component& component,
                                  bool wasMoved,
                                  bool wasResized) override;

    void mouseEnter (const MouseEvent& event) override;
    void mouseExit (const MouseEvent& event) override;

private:
    Slider* mOwnerSlider;
};
