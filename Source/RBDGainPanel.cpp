/*
  ==============================================================================

    RBDGainPanel.cpp
    Created: 11 Jan 2021 4:33:22pm
    Author:  Anthony

  ==============================================================================
*/

#include "RBDGainPanel.h"
#include "RBDInterfaceUtilities.h"

RBDGainPanel::RBDGainPanel (ReallyBasicDelayAudioProcessor* inProcessor)
    : RBDPanelBase (inProcessor)
{
    setSize (GAIN_PANEL_WIDTH, GAIN_PANEL_HEIGHT);
    setName ("GainPanel");
}

RBDGainPanel::~RBDGainPanel()
{
    
}

void RBDGainPanel::setParameterID (RBDParameter inParameter)
{
//  CREATE SLIDER
    mSlider = std::make_unique<RBDParameterSlider> (mProcessor->parameters,
                                                    RBDParameterID[inParameter],
                                                    RBDParameterLabel[inParameter]);
    
    const int sliderSize = 56;
    
    // TODO: is there a better way to position slider at the center of parent component?
    //   A1: there's `centreWithSize()`, as well as the ability to set size and position separately,
    //       and set position relative to parent size. But I haven't found another way to set this
    //       up with a single line where you can modify the bounds freely.
    //   A2: I think that it's much easier to understand the intent of component positioning when
    //       it's expressed as operations on rectangle objects, rather than math on coordinates and
    //       dimensions.
    mSlider->setBounds (getLocalBounds().withSizeKeepingCentre (sliderSize, sliderSize).withY (25));
    
    addAndMakeVisible (mSlider.get());
    
    // Register as mouse listener for Sliders so we can repaint Slider labels when mouse
    //  enters and exits Slider components
    mSlider->addMouseListener (this, false);
    
//  CREATE METER
    mMeter = std::make_unique<RBDMeter> (inParameter, mProcessor);
    
    // Define meter bounds
    const int meterWidth = sliderSize;
    const int meterGap = 20;
    Rectangle<int> meterBounds = mSlider->getBounds().withSizeKeepingCentre (meterWidth, 0);
    meterBounds.setTop (mSlider->getBottom() + RBD::labelHeight + meterGap);
    meterBounds.setBottom (getHeight() - meterGap);
    
    mMeter->setBounds (meterBounds);
    
    addAndMakeVisible (mMeter.get());
}

void RBDGainPanel::paint (Graphics& g)
{
    RBDPanelBase::paint(g);
    
    if (mSlider != nullptr)
        paintComponentLabel (g, mSlider.get());     // only paint label if slider exists
}

void RBDGainPanel::mouseEnter (const MouseEvent& event)
{
    repaint();
}

void RBDGainPanel::mouseExit (const MouseEvent& event)
{
    repaint();
}
