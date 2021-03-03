/*
  ==============================================================================

    RBDInterfaceDefines.h
    Created: 11 Jan 2021 5:13:02pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// TODO: Do we need global constants, or can we place them as members somewhere?
// The benefit of global constants is that they are all collected in one place
//  Perhaps, they could live in a relevant base class?

// TODO: Do we need to namespace Interface constants? Use better namespace name?

namespace RBD
{
    // TODO: Can we drop size constants by creating panels with relative sizes?

    // C++17 "inline" keyword ensures that only one copy of the constant
    //  is created for the whole project

    // Panel sizes

    inline constexpr int mainPanelWidth = 750;
    inline constexpr int mainPanelHeight = 300;

    inline constexpr int topPanelWidth = mainPanelWidth;
    inline constexpr int topPanelHeight = 45;

    inline constexpr int gainPanelWidth = 100;
    inline constexpr int gainPanelHeight = mainPanelHeight - topPanelHeight;

    inline constexpr int centrePanelWidth = mainPanelWidth - 2 * gainPanelWidth;
    inline constexpr int centrePanelHeight = gainPanelHeight;

    inline constexpr int centrePanelMenuBarWidth = centrePanelWidth;
    inline constexpr int centrePanelMenuBarHeight = 35;

    inline constexpr int fxPanelWidth = centrePanelWidth;
    inline constexpr int fxPanelHeight = centrePanelHeight - centrePanelMenuBarHeight;

    // Control sizes
    inline constexpr int defaultKnobSize = 80;

    // Label sizes
    inline constexpr int labelHeight = 20;
    
    // Standard rounded corner radius
    inline constexpr float defaultCornerSize = 3.0f;
    
    // TODO: Can we move colour and font global constants into LookAndFeel class?
    // TODO: Add descriptive colour names
    inline const Colour colour1 = Colour (105, 105, 105);
    inline const Colour colour2 = Colour (0, 0, 0).withAlpha (0.0f);
    inline const Colour colour3 = Colour (0, 0, 0).withAlpha (0.3f);
    inline const Colour colour4 = Colour (0, 0, 0).withAlpha (0.6f);
    inline const Colour colour5 = Colour (105, 105, 105).withAlpha (0.3f);
    inline const Colour colour6 = Colour (0, 0, 0).withAlpha (0.8f);
    inline const Colour colour7 = Colour (125, 125, 125);
    inline const Colour colour8 = Colour (40, 40, 40);
    
    // Font
    inline const Typeface::Ptr primaryFont
        = Typeface::createSystemTypefaceFor (BinaryData::OpenSansBold_ttf,
                                             BinaryData::OpenSansBold_ttfSize);

    // Styles
    inline const Font font1 = Font (primaryFont).withPointHeight (11.0f);
    inline const Font font2 = Font (primaryFont).withPointHeight (18.0f);
    inline const Font font3 = Font (primaryFont).withPointHeight (48.0f);
}
