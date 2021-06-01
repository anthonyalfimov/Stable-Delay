/*
  ==============================================================================

    AboutPanel.h
    Created: 4 May 2021 9:17:36pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include "PopupPanel.h"

class AboutPanel final  : public PopupPanel
{
public:
    AboutPanel (std::unique_ptr<PopupPanel>& owner);

//==============================================================================
    /** @internal */
    void resized() override;
    /** @internal */
    void paint (Graphics& g) override;

private:
    Label mTitleLabel { "title", "Really Basic Delay" };
    Label mVersionLabel { "version" };
    Label mAuthorLabel { "author", "Developed by Anthony Alfimov" };
    Label mLicenseLabel { "license", "> License info goes here <" };

    HyperlinkButton mSourceButton { "Source Code",
                                    URL ("https://github.com/anthonyalfimov") };

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AboutPanel)
};
