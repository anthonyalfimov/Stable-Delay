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

    // Set up FX Type ComboBox

    // Note: Initial ComboBox update happens when attachment is created
    //  Using the ParameterComboBox ctor that takes the item list allows to
    //  add the items before the attachment is created and ComboBox is updated

    mFxTypeComboBox = std::make_unique<ParameterComboBox> (mProcessor.parameters,
                                                           Parameter::FxType,
                                                           FxType::Label);
    const int comboBoxWidth = 95;
    auto bounds = getLocalBounds().withLeft (getWidth() - comboBoxWidth);
    mFxTypeComboBox->setBounds (bounds);
    addAndMakeVisible (mFxTypeComboBox.get());

    // Set up Stereo Spread Slider
    if (mProcessor.getTotalNumOutputChannels() == 2)
    {
        mSpreadSlider = std::make_unique<ParameterSlider> (mProcessor.parameters,
                                                           Parameter::StereoSpread);

        // TODO: Reposition the slider once custom graphics are introduced
        bounds.setX (RBD::defaultLabelWidth + 11);
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

void CentrePanelMenuBar::addFxTypeComboBoxListener (ComboBox::Listener* inListener)
{
    mFxTypeComboBox->addListener (inListener);
}

void CentrePanelMenuBar::removeFxTypeComboBoxListener (ComboBox::Listener* inListener)
{
    mFxTypeComboBox->removeListener (inListener);
}
