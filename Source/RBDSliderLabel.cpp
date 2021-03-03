/*
  ==============================================================================

    RBDSliderLabel.cpp
    Created: 3 Mar 2021 6:25:25pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "RBDSliderLabel.h"
#include "RBDInterfaceConstants.h"

SliderLabel::SliderLabel (Slider* ownerSlider)
    : Label (ownerSlider->getName() + "Label", ownerSlider->getName())
{
    mOwnerSlider = ownerSlider;
    attachToComponent (mOwnerSlider, false);

    if (mOwnerSlider != nullptr)
    {
        // Unregister as a listener with the slider
        ownerSlider->removeMouseListener (this);
        ownerSlider->addMouseListener (this, false);
        setText (mOwnerSlider->getName(), dontSendNotification);
    }
}

SliderLabel::~SliderLabel()
{
    if (mOwnerSlider != nullptr)
        mOwnerSlider->removeMouseListener (this);
}

void SliderLabel::paint (Graphics& g)
{
    const Colour bgColour = mOwnerSlider->isMouseOver() ? RBD::controlHoverColour
                                                        : RBD::controlNormalColour;
    g.setColour (bgColour);
    g.fillRoundedRectangle (getLocalBounds().toFloat(), RBD::defaultCornerSize);

    g.setColour (RBD::textNormalColour);
    g.setFont (RBD::font1);
    g.drawFittedText (getText(), getLocalBounds(), Justification::centred, 1);
}

void SliderLabel::componentMovedOrResized (Component& component,
                                           bool /*wasMoved*/,
                                           bool /*wasResized*/)
{
    const int width = component.getWidth();
    const int height = RBD::labelHeight;
    const int yPosition = component.getBottom();
    auto bounds = component.getBounds().withSizeKeepingCentre (width, height)
                                       .withY (yPosition);
    setBounds (bounds);
}

void SliderLabel::mouseEnter (const MouseEvent& /*event*/)
{
    repaint();
}

void SliderLabel::mouseExit (const MouseEvent& /*event*/)
{
    repaint();
}
