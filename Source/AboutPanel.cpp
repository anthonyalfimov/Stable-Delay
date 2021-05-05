/*
  ==============================================================================

    AboutPanel.cpp
    Created: 4 May 2021 9:17:36pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "AboutPanel.h"
#include "InterfaceConstants.h"

//==============================================================================
//  AboutPanel::InfoBox
//==============================================================================

AboutPanel::InfoBox::InfoBox()
{
    // Set up the Title Label
    mTitleLabel.setFont (RBD::titleFont);
    mTitleLabel.setJustificationType (Justification::topLeft);
    addAndMakeVisible (mTitleLabel);

    // Set up Version Label
    const auto buildDate = Time::getCompilationDate();
    mVersionLabel.setText ("Version " + StringRef (ProjectInfo::versionString)
                           + ". Build date: "
                           + buildDate.toString (true, true, false, true),
                           dontSendNotification);
    addAndMakeVisible (mVersionLabel);

    // Set up Author Label
    addAndMakeVisible (mAuthorLabel);

    // Set up License Label
    mLicenseLabel.setJustificationType (Justification::centred);
    addAndMakeVisible (mLicenseLabel);

    // Set up Source Code link
    addAndMakeVisible (mSourceButton);
}

void AboutPanel::InfoBox::resized()
{
    auto bounds = getLocalBounds().reduced (10);

    auto titleHeight = 30;
    auto textHeight = 20;

    mTitleLabel.setBounds (bounds.removeFromTop (titleHeight));
    mVersionLabel.setBounds (bounds.removeFromTop (textHeight));
    mAuthorLabel.setBounds (bounds.removeFromTop (textHeight));

    mSourceButton.setBounds (bounds.removeFromBottom (textHeight));
    mLicenseLabel.setBounds (bounds.removeFromBottom (textHeight));
}

void AboutPanel::InfoBox::paint (Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    auto cornerSize = RBD::defaultCornerSize * 2.0f;
    g.setColour (RBD::aboutPanelBgColour);
    g.fillRoundedRectangle (bounds, cornerSize);

    g.setColour (RBD::textNormalColour);
    bounds.reduce (5.5, 5.5);
    g.drawRoundedRectangle (bounds, RBD::defaultCornerSize, 1.0f);
}

//==============================================================================
//  AboutPanel
//==============================================================================

AboutPanel::AboutPanel (std::unique_ptr<AboutPanel>& owner)
    : mOwner (owner)
{
    // Set up Panel attributes
    setName ("AboutPanel");

    // Set up the InfoBox
    addAndMakeVisible (mInfoBox);
}

void AboutPanel::resized()
{
    auto bounds = getLocalBounds().withSizeKeepingCentre (RBD::aboutPanelWidth,
                                                          RBD::aboutPanelHeight);
    mInfoBox.setBounds (bounds);
}

void AboutPanel::paint (Graphics& g)
{
    g.fillAll (RBD::aboutPanelDimColour);
}

void AboutPanel::mouseDown (const MouseEvent& event)
{
    mOwner.reset();
}
