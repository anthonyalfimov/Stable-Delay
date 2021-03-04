/*
  ==============================================================================

    RBDSliderLabel.cpp
    Created: 3 Mar 2021 6:25:25pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "RBDSliderLabel.h"
#include "RBDInterfaceConstants.h"

// TODO: Choose whether to handle the case when OwnerSlider is nullptr
//  If we want to handle the case when the passed ptr is null, our paint()
//  method should not use the mOwnerSlider member - access to it should be
//  limited only to Listener callbacks (they get called only if the caller
//  exists).
//  If we are sure that we won't be passing nullptr to the constructor and
//  that Slider ptrs will not be reset to null during their lifetime,
//  we can keep things as they are remove all the precautions from the ctor.

SliderLabel::SliderLabel (Slider* ownerSlider)
{
    mOwnerSlider = ownerSlider;
    attachToComponent (mOwnerSlider, false);

    if (mOwnerSlider != nullptr)
    {
        // Unregister as a listener with the slider in case it was registered already
        ownerSlider->removeMouseListener (this);

        ownerSlider->addMouseListener (this, false);
        setName (mOwnerSlider->getName() + "Label");
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
    // TODO: Use LookAndFeel method to paint the SliderLabel text
    //  Our LookAndFeel should have a method to draw a generic label without
    //  any background. Here, we should first draw the label background as we
    //  do, and then call the LookAndFeel method to draw the text

    const Colour bgColour = mOwnerSlider->isMouseOverOrDragging()
                          ? RBD::controlHoverColour
                          : RBD::controlNormalColour;
    g.setColour (bgColour);
    g.fillRoundedRectangle (getLocalBounds().toFloat(), RBD::defaultCornerSize);

    g.setColour (RBD::textNormalColour);
    g.setFont (RBD::font1);

    const String text = mOwnerSlider->isMouseButtonDown()
                      ? mOwnerSlider->getTextFromValue (mOwnerSlider->getValue())
                      : getText();

    // TODO: Disable text scaling
    g.drawFittedText (text, getLocalBounds(), Justification::centred, 1);
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

void SliderLabel::mouseDown (const MouseEvent& /*event*/)
{
    repaint();
}

void SliderLabel::mouseDrag (const MouseEvent& /*event*/)
{
    repaint();
}

void SliderLabel::mouseUp (const MouseEvent& /*event*/)
{
    repaint();
}

