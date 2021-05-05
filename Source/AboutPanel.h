/*
  ==============================================================================

    AboutPanel.h
    Created: 4 May 2021 9:17:36pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AboutPanel  : public Component
{
public:
    AboutPanel (std::unique_ptr<AboutPanel>& owner);

//==============================================================================
    /** @internal */
    void resized() override;
    /** @internal */
    void paint (Graphics& g) override;
    /** @internal */
    void mouseDown (const MouseEvent& event) override;

private:
    std::unique_ptr<AboutPanel>& mOwner;

//==============================================================================
//  Component class containing the About information:
    class InfoBox  : public Component
    {
    public:
        InfoBox();

    //==========================================================================
        /** @internal */
        void resized() override;
        /** @internal */
        void paint (Graphics& g) override;

    private:
        Label mTitleLabel { "title", "Really Basic Delay" };
        Label mVersionLabel { "version" };
        Label mAuthorLabel { "author", "Developed by Anthony Alfimov" };
        Label mLicenseLabel { "license", "> License info goes here <" };

        // TODO: Update custom LookAndFeel with methods for HyperlinkButton
        HyperlinkButton mSourceButton { "Source Code",
                                        URL ("https://github.com/anthonyalfimov") };

    //==========================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InfoBox)
    };

//==============================================================================
    InfoBox mInfoBox;

//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AboutPanel)
};
