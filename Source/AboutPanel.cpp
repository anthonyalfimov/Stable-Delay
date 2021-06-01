/*
  ==============================================================================

    AboutPanel.cpp
    Created: 4 May 2021 9:17:36pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "AboutPanel.h"
#include "InterfaceConstants.h"

AboutPanel::AboutPanel (std::unique_ptr<PopupPanel>& owner) : PopupPanel (owner)
{
    // Set up Panel attributes
    setName ("AboutPanel");
    panelColour = RBD::aboutPanelBgColour;

    // Set up the Title Label
    mTitleLabel.setFont (RBD::titleFont);
    addAndMakeVisible (mTitleLabel);
    mTitleLabel.addMouseListener (this, false);     // close panel when clicking on label

    // Set up Version Label
    const auto buildDate = Time::getCompilationDate();
    mVersionLabel.setText ("Version " + StringRef (ProjectInfo::versionString)
                           + ". Build date: "
                           + buildDate.toString (true, true, false, true),
                           dontSendNotification);
    mVersionLabel.setFont (RBD::descriptorFont);
    addAndMakeVisible (mVersionLabel);
    mVersionLabel.addMouseListener (this, false);   // close panel when clicking on label

    // Set up Author Label
    mAuthorLabel.setFont (RBD::descriptorFont);
    addAndMakeVisible (mAuthorLabel);
    mAuthorLabel.addMouseListener (this, false);    // close panel when clicking on label

    // Set up License Label
    mLicenseLabel.setFont (RBD::descriptorFont);
    mLicenseLabel.setJustificationType (Justification::centred);
    addAndMakeVisible (mLicenseLabel);
    mLicenseLabel.addMouseListener (this, false);   // close panel when clicking on label

    // Set up Source Code link
    mSourceButton.setFont (RBD::descriptorFont.withStyle (Font::underlined), false);
    addAndMakeVisible (mSourceButton);
}

void AboutPanel::resized()
{
    panelBounds = getLocalBounds().withSizeKeepingCentre (RBD::aboutPanelWidth,
                                                          RBD::aboutPanelHeight);
    auto bounds = panelBounds.reduced (10);

    auto titleHeight = 34;
    auto lineHeight = 22;

    mTitleLabel.setBounds (bounds.removeFromTop (titleHeight));
    mVersionLabel.setBounds (bounds.removeFromTop (lineHeight));
    mAuthorLabel.setBounds (bounds.removeFromTop (lineHeight));

    mSourceButton.setBounds (bounds.removeFromBottom (lineHeight * 1.5));
    mLicenseLabel.setBounds (bounds.removeFromBottom (lineHeight));
}

void AboutPanel::paint (Graphics& g)
{
    // Dim the rest of the UI:
    g.fillAll (RBD::aboutPanelDimColour);

    // Draw About panel background:
    PopupPanel::paint (g);

    // Draw About Panel border:
    const float innerCornerSize = panelCornerSize / 2.0f;
    g.setColour (RBD::textNormalColour);
    g.drawRoundedRectangle (panelBounds.toFloat().reduced (5.5f),
                            innerCornerSize, 1.0f);
}
