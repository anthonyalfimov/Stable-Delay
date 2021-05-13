/*
  ==============================================================================

    CenterPanelMenuBar.cpp
    Created: 11 Jan 2021 4:32:37pm
    Author:  Anthony

  ==============================================================================
*/

#include "CentrePanelMenuBar.h"
#include "Parameters.h"

CentrePanelMenuBar::CentrePanelMenuBar (ReallyBasicDelayAudioProcessor& processor)
    : InterfacePanel (processor)
{
    // Set up Panel attributes
    setSize (RBD::centrePanelMenuBarWidth, RBD::centrePanelMenuBarHeight);
    setName ("CenterMenu");

    // Set up Stereo Spread Slider
    if (mProcessor.getTotalNumOutputChannels() == 2)
    {
        mSpreadSlider = std::make_unique<ParameterSlider> (mProcessor.parameters,
                                                           Parameter::StereoSpread);

        // TODO: Reposition the slider once custom graphics are introduced
        auto bounds = getLocalBounds().withX (RBD::defaultLabelWidth + 11);
        //const int sliderWidth = 200;
        //bounds.setWidth (sliderWidth);
        bounds.setTop (3);
        bounds.setRight (getLocalBounds().getCentreX() + 10);
        mSpreadSlider->setBounds (bounds);
        addAndMakeVisible (mSpreadSlider.get());

        // Set up Stereo Spread Slider label
        mSpreadLabel = std::make_unique<SliderLabel> (mSpreadSlider.get(), true);
        addAndMakeVisible (mSpreadLabel.get());
    }
}
