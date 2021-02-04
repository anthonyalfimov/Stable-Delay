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
// Perhaps, they could live in a relevant base class?

// TODO: Do we need to namespace Interface constants? Use better namespace name?

namespace RBD
{
    // TODO: Can we drop size constants by creating panels with relative sizes?
    // Panel sizes
    constexpr int mainPanelWidth = 750;
    constexpr int mainPanelHeight = 300;
    
    constexpr int topPanelWidth = mainPanelWidth;
    constexpr int topPanelHeight = 45;
    
    constexpr int gainPanelWidth = 100;
    constexpr int gainPanelHeight = mainPanelHeight - topPanelHeight;
    
    constexpr int centrePanelWidth = mainPanelWidth - 2 * gainPanelWidth;
    constexpr int centrePanelHeight = gainPanelHeight;
    
    constexpr int centrePanelMenuBarWidth = centrePanelWidth;
    constexpr int centrePanelMenuBarHeight = 35;
    
    constexpr int fxPanelWidth = centrePanelWidth;
    constexpr int fxPanelHeight = centrePanelHeight - centrePanelMenuBarHeight;
    
    // Label sizes
    constexpr static int labelHeight = 20;
    
    // Standard rounded corner radius
    constexpr static float defaultCornerSize = 3.0f;
    
    // TODO: Can we drop colour and font global constants by putting them into our LookAndFeel class?
    // TODO: Add descriptive colour names
    // const is static by default, explicitely state it to highlight intent
    const static Colour colour1 = Colour (105, 105, 105);
    const static Colour colour2 = Colour (0, 0, 0).withAlpha (0.0f);
    const static Colour colour3 = Colour (0, 0, 0).withAlpha (0.3f);
    const static Colour colour4 = Colour (0, 0, 0).withAlpha (0.6f);
    const static Colour colour5 = Colour (105, 105, 105).withAlpha (0.3f);
    const static Colour colour6 = Colour (0, 0, 0).withAlpha (0.8f);
    const static Colour colour7 = Colour (125, 125, 125);
    const static Colour colour8 = Colour (40, 40, 40);
    
    // Font
    const static Typeface::Ptr primaryFont
        = Typeface::createSystemTypefaceFor (BinaryData::OpenSansBold_ttf,
                                             BinaryData::OpenSansBold_ttfSize);

    // Styles
    const static Font font1 = Font (primaryFont).withPointHeight (11.0f);
    const static Font font2 = Font (primaryFont).withPointHeight (18.0f);
    const static Font font3 = Font (primaryFont).withPointHeight (48.0f);
}
