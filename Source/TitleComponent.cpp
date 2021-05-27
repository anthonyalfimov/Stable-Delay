/*
  ==============================================================================

    TitleComponent.cpp
    Created: 27 May 2021 4:51:55pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "TitleComponent.h"
#include "InterfaceConstants.h"
#include "MainPanel.h"

TitleComponent::TitleComponent()
{
    // Set up the Title Label
    mTitleLabel.setFont (RBD::titleFont);
    mTitleLabel.setJustificationType (Justification::centredRight);
    mTitleLabel.setBorderSize ({ 1, 0, 1, 0 });
    addAndMakeVisible (mTitleLabel);

    // Set up Title Label mouse events callbacks
    mLabelMouseEventInvoker.onMouseDown = [this] (const MouseEvent& /*event*/)
    {
        auto* parent = findParentComponentOfClass<MainPanel>();

        if (parent == nullptr)
        {
            jassertfalse;
            return;
        }

        mAboutPanel = std::make_unique<AboutPanel> (parent, mAboutPanel);
    };
    
    mTitleLabel.addMouseListener (&mLabelMouseEventInvoker, false);
}

void TitleComponent::resized()
{
    int titleWidth = RBD::titleFont.getStringWidth (mTitleLabel.getText());
    auto titleLabelBounds = getLocalBounds().removeFromRight (titleWidth);
    mTitleLabel.setBounds (titleLabelBounds);
}
