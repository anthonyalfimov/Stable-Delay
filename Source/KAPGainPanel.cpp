/*
  ==============================================================================

    KAPGainPanel.cpp
    Created: 11 Jan 2021 4:33:22pm
    Author:  Anthony

  ==============================================================================
*/

#include "KAPGainPanel.h"
#include "KAPUiHelpers.h"

KAPGainPanel::KAPGainPanel (KadenzeAudioPluginAudioProcessor* inProcessor)
    : KAPPanelBase (inProcessor)
{
    setSize (GAIN_PANEL_WIDTH, GAIN_PANEL_HEIGHT);
    setName ("GainPanel");
}

KAPGainPanel::~KAPGainPanel()
{
    
}

void KAPGainPanel::setParameterID (KAPParameter inParameter)
{
    mSlider = std::make_unique<KAPParameterSlider> (mProcessor->parameters,
                                                    KAPParameterID[inParameter]);
    
    const int sliderSize = 54;
    
    // TODO: is there a better way to position slider at the center of parent component?
    //    A: there's `centreWithSize()`, as well as the ability to set size and position separately,
    //       and set position relative to parent size. But I haven't found another way to set this
    //       up with a single line where you can modify the bounds freely.
    mSlider->setBounds (getLocalBounds().withSizeKeepingCentre (sliderSize, sliderSize));
    
    addAndMakeVisible (mSlider.get());
}

void KAPGainPanel::paint (Graphics& g)
{
    KAPPanelBase::paint(g);
    paintComponentLabel (g, mSlider.get());
}
