/*
  ==============================================================================

    InterfaceDefines.h
    Created: 11 Jan 2021 5:13:02pm
    Author:  Anthony

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

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

    // TODO: Move component size constants into the LookAndFeel class
    //  Once all component drawing is handled by the custom L&F class,
    //  these constants can be safely moved inside that class.

    // Meter sizes
    inline constexpr int meterChannelWidth = 18;
    inline constexpr int meterLegendWidth = meterChannelWidth + 2;

    // Control sizes
    inline constexpr int knobSize = 80;
    inline const Point<float> tickShadowOffset (0.7f, 0.7f);

    // Label sizes
    inline constexpr int labelHeight = 20;
    inline constexpr int defaultLabelWidth = knobSize;
    
    // Standard rounded corner radius
    inline constexpr float defaultCornerSize = 3.0f;
    
    // TODO: Move colour and font global constants into the LookAndFeel class
    //  Once all component drawing is handled by the custom L&F class,
    //  these constants can be safely moved inside that class.
    //  Aim to use ColourIds more as well.

    inline const Colour noColour = Colour (0, 0, 0).withAlpha (0.0f);
    inline const Colour textNormalColour = Colour (110, 110, 110);
    inline const Colour textActiveColour = Colour (125, 125, 125);
    inline const Colour textFxTypeColour = Colour (105, 105, 105).withAlpha (0.3f);
    inline const Colour controlNormalColour = Colour (0, 0, 0).withAlpha (0.3f);
    inline const Colour controlHoverColour = Colour (0, 0, 0).withAlpha (0.8f);
    inline const Colour controlActiveColour = Colour (105, 105, 105).withAlpha (0.3f);
    inline const Colour controlOutlineColour = noColour;
    inline const Colour sliderMarkColour = Colour (242, 242, 242);
    inline const Colour sliderTickColour = Colour (232, 232, 232);
    inline const Colour popupBgColour = Colour (40, 40, 40);
    inline const Colour popupItemBgColour = Colour (0, 0, 0).withAlpha (0.6f);
    inline const Colour meterBgColour = Colour (0, 0, 0).withAlpha (0.8f);
    inline const Colour meterFillColour = Colour (125, 125, 125);
    inline const Colour meterLegendColour = textNormalColour;
    inline const Colour meterClippingColour = Colour (0xFFDF4107);
    inline const Colour meterSaturationColour = Colour (0xFFE39105);
    inline const Colour shadowColour = Colour (0, 0, 0).withAlpha (0.27f);
    // Temporary Slider colours
    inline const Colour sliderTrackColour = Colour (35, 35, 35);

    // Font
    inline const Typeface::Ptr primaryFont
        = Typeface::createSystemTypefaceFor (BinaryData::OpenSansBold_ttf,
                                             BinaryData::OpenSansBold_ttfSize);

    // Styles
    inline const Font mainFont = Font (primaryFont).withPointHeight (11.0f);
    inline const Font titleFont = Font (primaryFont).withPointHeight (18.0f);
    inline const Font fxTypeFont = Font (primaryFont).withPointHeight (48.0f);
    inline const Font meterFont = Font (primaryFont).withPointHeight (9.0f);
}