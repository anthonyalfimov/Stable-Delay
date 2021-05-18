/*
  ==============================================================================

    PluginLookAndFeel.cpp
    Created: 9 Mar 2021 4:23:02pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "PluginLookAndFeel.h"

PluginLookAndFeel::PluginLookAndFeel()
{
    // TODO: What's the difference b/w loading the image from file and embedding it?
    //  E.g., using ImageCache::getFromFile(const File &file)
    //  Look at JUCE's DemoRunner image handling, in particular
    //  Image getImageFromAssets (const char* assetName) utility function

    // Store image assets
    mKnobImage = ImageCache::getFromMemory (BinaryData::RBD_Knob_80_2x_png,
                                            BinaryData::RBD_Knob_80_2x_pngSize);

    // Label colours
    setColour (Label::backgroundColourId, RBD::noColour);
    setColour (Label::textColourId, RBD::textNormalColour);
    setColour (Label::outlineColourId, RBD::controlOutlineColour);

    // ComboBox colours
    setColour (ComboBox::backgroundColourId, RBD::controlNormalColour);
    setColour (ComboBox::outlineColourId, RBD::controlOutlineColour);
    setColour (ComboBox::arrowColourId, RBD::arrowNormalColour);
    setColour (ComboBox::textColourId, RBD::textNormalColour);

    // Popup colours (for comboBox popup)
    setColour (PopupMenu::backgroundColourId, RBD::popupBgColour);

    // TextButton colours
    setColour (TextButton::buttonColourId, RBD::controlNormalColour);
    setColour (TextButton::textColourOnId, RBD::textNormalColour);
    setColour (TextButton::textColourOffId, RBD::textNormalColour);

    // HyperlinkButton colours
    setColour (HyperlinkButton::textColourId, RBD::textLinkColour);

    // Temporary Slider colours (before we supply custom graphics)
    setColour (Slider::thumbColourId, RBD::textNormalColour);
    setColour (Slider::backgroundColourId, RBD::meterBgColour);
    setColour (Slider::trackColourId, RBD::sliderTrackColour);
}

//= LABELS =====================================================================

void PluginLookAndFeel::drawLabel (Graphics& g, Label& label)
{
    // Draw Label background
    g.setColour (label.findColour (Label::backgroundColourId));
    g.fillRoundedRectangle (label.getLocalBounds().toFloat(),
                            RBD::defaultCornerSize);

    // MARK: Not handling editable labels

    // Draw Label text
    auto alpha = label.isEnabled() ? 1.0f : 0.5f;
    const Font font (getLabelFont (label));

    g.setColour (label.findColour (Label::textColourId).withMultipliedAlpha (alpha));
    g.setFont (font);

    const auto textBounds = label.getBorderSize()
                                 .subtractedFrom (label.getLocalBounds());

    int maxNumberOfLines
    = jmax (1, static_cast<int> (static_cast<float> (textBounds.getHeight())
                                 / font.getHeight()));

    g.drawFittedText (label.getText(), textBounds, label.getJustificationType(),                        maxNumberOfLines, 1.0f /* don't squeeze my text! */);

    // MARK: Not painting the outline
}

//= BUTTONS ====================================================================

Font PluginLookAndFeel::getTextButtonFont (TextButton& /*button*/,
                                           int /*buttonHeight*/)
{
    return RBD::mainFont;
}

void PluginLookAndFeel::drawButtonBackground (Graphics& g, Button& button,
                                              const Colour& /*backgroundColour*/,
                                              bool shouldDrawButtonAsHighlighted,
                                              bool shouldDrawButtonAsDown)
{
    auto fillColour = RBD::controlNormalColour;

    if (shouldDrawButtonAsDown)
        fillColour = RBD::controlActiveColour;
    else if (shouldDrawButtonAsHighlighted)
        fillColour = RBD::controlHoverColour;

    #warning Non-namespaced Rectangle might be a problem on Windows
    const auto bounds = button.getLocalBounds().toFloat().reduced (0.5f);

    g.setColour (fillColour);
    const float cornerSize = 2.0f * RBD::defaultCornerSize;
    g.fillRoundedRectangle (bounds.reduced (1.0f), cornerSize);
}

//= POPUP MENUS ================================================================

// NOTE: Making the popup bg colour transparent will switch the MenuWindow to
//       being transparent (if desktop supports that).
//       If you want to use rounded corners or force the component to use
//       transparency regardless of desktop (e.g. if you're parenting the
//       popup to the editor), call the `preparePopupMenuWindow()` method
//       to manually set it as not opaque.

void PluginLookAndFeel::drawPopupMenuBackground (Graphics& g, int width, int height)
{
    g.setColour (findColour (PopupMenu::backgroundColourId));
    //g.fillRoundedRectangle (0, 0, width, height, RBD::defaultCornerSize);
    g.fillAll();
}

Font PluginLookAndFeel::getPopupMenuFont()
{
    return RBD::mainFont;
}

void PluginLookAndFeel::drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                                           bool /*isSeparator*/, bool /*isActive*/,
                                           bool isHighlighted, bool isTicked,
                                           bool /*hasSubMenu*/, const String& text,
                                           const String& /*shortcutKeyText*/,
                                           const Drawable* /*icon*/,
                                           const Colour* /*textColourToUse*/)
{
    Rectangle<int> bounds (area);   // create editable copy of passed bounds to adjust
    bounds.removeFromBottom (1);
    Colour fillColour = isHighlighted ? RBD::controlActiveColour
                                      : RBD::popupItemBgColour;

    g.setColour (fillColour);
    g.fillRect (bounds);

    Colour textColour = isTicked ? RBD::textActiveColour : RBD::textNormalColour;
    g.setColour (textColour);

    if (area.getHeight() >= largePopupMenuItemHeight)
    {   // Large popup
        g.setFont (RBD::largeFont);
        g.drawFittedText (text, bounds, Justification::centred, 1, 1.0f);
    }
    else
    {   // Normal popup
        auto tickBounds = bounds.removeFromLeft (bounds.getHeight());

        if (isTicked)
        {
            g.setColour (RBD::toggleHandleColour);
            tickBounds = tickBounds.withSizeKeepingCentre (6, 6);
            g.fillRoundedRectangle (tickBounds.toFloat(), 1.0f);
        }

        g.setFont (RBD::mainFont);
        g.drawFittedText (text, bounds, Justification::centredLeft, 1, 1.0f);
    }
}

void PluginLookAndFeel::getIdealPopupMenuItemSize (const String& /*text*/,
                                                   bool isSeparator,
                                                   int standardMenuItemHeight,
                                                   int& /*idealWidth*/,
                                                   int& idealHeight)
{
    // NB: Currently not setting the ideal width to keep the width fixed

    // TODO: Set ideal width based on the passed text for preset manager sub-menus
    //  Currently we don't have a need for dynamic adjustment of menu width
    //  for its content. But a preset manager, especially its sub-menus,
    //  could definitely benefit from that. Is it possible to keep the main menu
    //  a fixed width, while make all sub-menus dynamically sized?

    const float fontHeight = getPopupMenuFont().getHeight();

    if (isSeparator)
    {
        idealHeight = (standardMenuItemHeight > 0) ? standardMenuItemHeight / 2
                                                   : roundToInt (fontHeight);
        return;
    }

    idealHeight = (standardMenuItemHeight > 0) ? standardMenuItemHeight
                                               : roundToInt (fontHeight * 1.75f);
}

//= COMBOBOXES =================================================================

void PluginLookAndFeel::drawComboBox (Graphics& g, int width, int height,
                                      bool isButtonDown,
                                      int buttonX, int buttonY,
                                      int buttonW, int buttonH,
                                      ComboBox& comboBox)
{
    // TODO: Consider resolving highlight status in mouse callbacks
    //  If we check whether the comboBox should be highlighted in mouse
    //  callbacks like it is done with SliderLabel class, we only need to
    //  get the respective colourID colour and paint the comboBox here.
    //  The specific colour would be set in mouse callbacks, and it can be
    //  different for different subclasses.

    bool shouldHighlight
    = comboBox.isPopupActive() || comboBox.isMouseOverOrDragging (true);

    const Colour comboBoxColour = shouldHighlight ? RBD::controlHoverColour
                                : comboBox.findColour (ComboBox::backgroundColourId);
    g.setColour (comboBoxColour);
    g.fillRoundedRectangle (0.0f, 0.0f, width, height, RBD::defaultCornerSize);

    // MARK: Begin debug paint
    Rectangle<int> textBorder (0, 0, width, height);
    textBorder.reduce (comboBoxButtonWidth + 4, 8);
    g.setColour (Colours::hotpink);
    //g.fillRect (textBorder);
    // MARK: End debug paint

    const Colour arrowColour = comboBox.isPopupActive() ? RBD::arrowActiveColour
                             : comboBox.findColour (ComboBox::arrowColourId);
    drawComboBoxButton (g, arrowColour, buttonX, buttonY, buttonW, buttonH);
}

void PluginLookAndFeel::drawComboBoxButton (Graphics& g, Colour arrowColour,
                                            int buttonX, int buttonY,
                                            int buttonW, int buttonH)
{
    Rectangle<float> bounds (buttonX, buttonY, buttonW, buttonH);
    const int halfWidth = 7;    // half of the arrow's width
    const int halfHeight = 2;   // half of the arrow's height

    // Create arrow shape
    Path arrow;
    arrow.startNewSubPath (bounds.getCentreX() - halfWidth,
                           bounds.getCentreY() - halfHeight);
    arrow.lineTo (bounds.getCentreX(), bounds.getCentreY() + halfHeight);
    arrow.lineTo (bounds.getCentreX() + halfWidth, bounds.getCentreY() - halfHeight);

    g.setColour (arrowColour);
    g.strokePath (arrow, PathStrokeType (2.0f, PathStrokeType::mitered,
                                               PathStrokeType::rounded));
}

Font PluginLookAndFeel::getComboBoxFont (ComboBox& comboBox)
{
    if (comboBox.getHeight() >= largeComboBoxMinHeight)
        return RBD::largeFont;

    return RBD::mainFont;
}

void PluginLookAndFeel::positionComboBoxText (ComboBox& comboBox, Label& label)
{
    label.setFont (getComboBoxFont (comboBox));

    auto bounds = comboBox.getLocalBounds();
    bounds.removeFromRight (comboBoxButtonWidth);

    if (comboBox.getHeight() >= largeComboBoxMinHeight)
    {   // Large ComboBox
        // Text is centred, so add the same padding on the left
        bounds.removeFromLeft (comboBoxButtonWidth);
        // Use borders to compensate for the intrinsic font offsets
        const int bottomBorder = comboBoxTextVerticalOffset;
        const int rightBorder = comboBoxTextHorizontalOffset;
        label.setBorderSize ({ 0, 0, bottomBorder, rightBorder });
    }
    else
    {   // Normal ComboBox
        label.setBorderSize ({ 1, 6, 1, 6 });
    }

    label.setBounds (bounds);
}

PopupMenu::Options PluginLookAndFeel::getOptionsForComboBoxPopupMenu (ComboBox& comboBox,
                                                                      Label& label)
{
    int itemHeight = 0;

    if (comboBox.getHeight() >= largeComboBoxMinHeight)
    {   // Large ComboBox
        itemHeight = largePopupMenuItemHeight;
    }
    else
    {   // Normal ComboBox
        itemHeight = popupMenuItemHeight;
    }

    return PopupMenu::Options().withTargetComponent (&comboBox)
                               .withItemThatMustBeVisible (comboBox.getSelectedId())
                               .withInitiallySelectedItem (comboBox.getSelectedId())
                               .withMinimumWidth (comboBox.getWidth())
                               .withMaximumNumColumns (1)
                               .withStandardItemHeight (itemHeight);
}

//= SLIDERS ====================================================================

void PluginLookAndFeel::drawRotarySlider (Graphics& g,
                                          int x, int y, int width, int height,
                                          float sliderPosProportional,
                                          float rotaryStartAngle,
                                          float rotaryEndAngle,
                                          Slider& /*slider*/)
{

    // Draw raster knob image
    const int diameter = jmin (width, height);
    auto bounds
    = Rectangle<int> (x, y, width, height).withSizeKeepingCentre (diameter, diameter);

    g.setOpacity (1.0f);    // Make sure the image is drawn opaque
    g.drawImage (mKnobImage, bounds.toFloat());

    // Draw knob mark
    const float angle = jmap (sliderPosProportional, rotaryStartAngle, rotaryEndAngle);
    const auto origin = bounds.getCentre();

    // TODO: Test the performance of adding line segment and rounding its corners
    //  If this is slow, look for alternatives: drawing a rounded rectangle,
    //  creating the path object in the ctor and then applying a rotation
    //  affine transformation to position it when drawing, etc.
    Path markPath;
    markPath.addLineSegment ({ origin.getPointOnCircumference (knobMarkStart, angle),
                               origin.getPointOnCircumference (knobMarkEnd, angle) },
                             knobMarkThickness);
    markPath = markPath.createPathWithRoundedCorners (knobMarkThickness / 2.0f);

    g.setColour (RBD::sliderMarkColour);
    g.fillPath (markPath);

    // Reset the following changes to the graphics context at the end of the scope
    Graphics::ScopedSaveState saveState (g);
    g.reduceClipRegion (markPath);  // set clip region to the knob mark

    Path shadowPath (markPath);
    shadowPath.applyTransform (AffineTransform::translation (RBD::tickShadowOffset));
    shadowPath.addRectangle (shadowPath.getBounds().expanded (2.0f));
    shadowPath.setUsingNonZeroWinding (false);
    g.setColour (RBD::shadowColour);
    g.fillPath (shadowPath);
}
