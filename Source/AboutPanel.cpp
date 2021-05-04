/*
  ==============================================================================

    AboutPanel.cpp
    Created: 4 May 2021 9:17:36pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "AboutPanel.h"
#include "InterfaceConstants.h"

AboutPanel::AboutPanel()
{
    // Set up Panel attributes
    setSize (RBD::aboutPanelWidth, RBD::aboutPanelHeight);
    setName ("AboutPanel");

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

void AboutPanel::resized()
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

void AboutPanel::paint (Graphics& g)
{
    auto border = getLocalBounds().toFloat().reduced (5.5);
    g.setColour (RBD::textNormalColour);
    g.drawRoundedRectangle (border, RBD::defaultCornerSize * 2.0f, 1.0f);
}
