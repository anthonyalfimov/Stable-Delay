/*
  ==============================================================================

    AboutPanel.cpp
    Created: 4 May 2021 9:17:36pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "AboutPanel.h"
#include "InterfaceConstants.h"

AboutPanel::AboutPanel (std::unique_ptr<AboutPanel>& owner)
    : mOwner (owner)
{
    // Set up Panel attributes
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
    mBackground = getLocalBounds().withSizeKeepingCentre (RBD::aboutPanelWidth,
                                                          RBD::aboutPanelHeight);

    auto bounds = mBackground.reduced (10);

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
    // Dim the rest of the UI:
    g.fillAll (RBD::aboutPanelDimColour);

    // Draw About panel background:
    auto bounds = mBackground.toFloat();
    auto outerCornerSize = RBD::defaultCornerSize * 2.0f;
    g.setColour (RBD::aboutPanelBgColour);
    g.fillRoundedRectangle (bounds, outerCornerSize);

    // Draw About Panel border:
    bounds.reduce (5.5, 5.5);
    auto innerCornerSize = RBD::defaultCornerSize;
    g.setColour (RBD::textNormalColour);
    g.drawRoundedRectangle (bounds, innerCornerSize, 1.0f);
}

void AboutPanel::mouseDown (const MouseEvent& event)
{
    mOwner.reset();
}
