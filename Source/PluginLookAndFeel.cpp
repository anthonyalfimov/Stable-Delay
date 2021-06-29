/*
  ==============================================================================

    PluginLookAndFeel.cpp
    Created: 9 Mar 2021 4:23:02pm
    Author:  Anthony Alfimov

  ==============================================================================
*/

#include "PluginLookAndFeel.h"
#include "MainPanel.h"

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

    // Popup colours
    setColour (PopupMenu::backgroundColourId, RBD::popupBgColour);
    setColour (PopupMenu::textColourId, RBD::textNormalColour);

    // TextButton colours
    setColour (TextButton::buttonColourId, RBD::controlNormalColour);
    setColour (TextButton::buttonOnColourId, RBD::controlHoverColour);
    setColour (TextButton::textColourOffId, RBD::textNormalColour);
    setColour (TextButton::textColourOnId, RBD::textActiveColour);

    // HyperlinkButton colours
    setColour (HyperlinkButton::textColourId, RBD::textLinkColour);

    // Temporary Slider colours (before we supply custom graphics)
    setColour (Slider::thumbColourId, RBD::textNormalColour);
    setColour (Slider::backgroundColourId, RBD::meterBgColour);
    setColour (Slider::trackColourId, RBD::sliderTrackColour);
}

//= UTILITIES ==================================================================

void PluginLookAndFeel::drawUpDownArrowButton (Graphics& g, Colour arrowColour,
                                               bool isUpArrow,
                                               int buttonX, int buttonY,
                                               int buttonW, int buttonH)
{
    Rectangle<float> bounds (buttonX, buttonY, buttonW, buttonH);
    const int halfWidth = 7;    // half of the arrow's width
    const int halfHeight = isUpArrow ? -2 : 2;   // half of the arrow's height

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

void PluginLookAndFeel::drawButtonBackground (Graphics& g, Button& button,
                                              const Colour& /*backgroundColour*/,
                                              bool shouldDrawButtonAsHighlighted,
                                              bool shouldDrawButtonAsDown)
{
    Colour fillColour;

    if (shouldDrawButtonAsDown)
        fillColour = button.findColour (TextButton::buttonOnColourId).brighter();
    else if (shouldDrawButtonAsHighlighted)
        fillColour = button.findColour (TextButton::buttonOnColourId);
    else
        fillColour = button.findColour (TextButton::buttonColourId);

    g.setColour (fillColour);

    const auto bounds = button.getLocalBounds().toFloat();
    const float cornerSize = RBD::defaultCornerSize;

    auto flatOnLeft   = button.isConnectedOnLeft();
    auto flatOnRight  = button.isConnectedOnRight();
    auto flatOnTop    = button.isConnectedOnTop();
    auto flatOnBottom = button.isConnectedOnBottom();

    if (flatOnLeft || flatOnRight || flatOnTop || flatOnBottom)
    {
        Path path;
        path.addRoundedRectangle (bounds.getX(), bounds.getY(),
                                  bounds.getWidth(), bounds.getHeight(),
                                  cornerSize, cornerSize,
                                  ! (flatOnLeft  || flatOnTop),
                                  ! (flatOnRight || flatOnTop),
                                  ! (flatOnLeft  || flatOnBottom),
                                  ! (flatOnRight || flatOnBottom));

        g.fillPath (path);
    }
    else
    {
        g.fillRoundedRectangle (bounds, cornerSize);
    }
}

Font PluginLookAndFeel::getTextButtonFont (TextButton& /*button*/,
                                           int buttonHeight)
{
    if (buttonHeight >= largeButtonMinHeight)
        return RBD::largeFont;

    return RBD::mainFont;
}

void PluginLookAndFeel::drawButtonText (Graphics& g, TextButton& button,
                                        bool /*shouldDrawButtonAsHighlighted*/,
                                        bool /*shouldDrawButtonAsDown*/)
{
    Font font (getTextButtonFont (button, button.getHeight()));
    g.setFont (font);
    g.setColour (button.findColour (button.getToggleState() ? TextButton::textColourOnId
                                                            : TextButton::textColourOffId)
                       .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));

    const int cornerSize = RBD::defaultCornerSize;
    auto textBounds = button.getLocalBounds().reduced (cornerSize);

    if (button.getHeight() >= largeButtonMinHeight)
    {
        textBounds.removeFromBottom (largeFontBottomOffset);
        textBounds.removeFromRight (largeFontRightOffset);
    }

    g.drawFittedText (button.getButtonText(), textBounds,
                      Justification::centred, 1, 1.0f);
}

//= POPUP MENUS ================================================================

void PluginLookAndFeel::drawPopupMenuBackground (Graphics& g, int width, int height)
{
    g.setColour (findColour (PopupMenu::backgroundColourId));
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
    Colour fillColour = isHighlighted ? RBD::popupItemHoverColour
                                      : RBD::popupItemColour;

    g.setColour (fillColour);
    g.fillRect (bounds);

    auto tickBounds = bounds.removeFromLeft (bounds.getHeight());

    if (isTicked)
    {
        tickBounds = tickBounds.withSizeKeepingCentre (6, 6);
        g.setColour (RBD::toggleHandleColour);
        g.fillRoundedRectangle (tickBounds.toFloat(), 1.0f);
    }

    Colour textColour = isTicked ? RBD::textActiveColour : RBD::textNormalColour;
    g.setColour (textColour);
    g.setFont (getPopupMenuFont());
    g.drawFittedText (text, bounds, Justification::centredLeft, 1, 1.0f);
}

void PluginLookAndFeel::drawPopupMenuUpDownArrow (Graphics& g,
                                                  int width, int height,
                                                  bool isScrollUpArrow)
{
    const auto bgColour = RBD::popupItemColour;

    const float gradientStartY = 0.5f * height;
    const float gradientEndY = isScrollUpArrow ? height : 0.0f;

    g.setGradientFill (ColourGradient (bgColour,
                                       0.0f, gradientStartY,
                                       bgColour.withAlpha (0.0f),
                                       0.0f, gradientEndY,
                                       false));
    g.fillRect (0, 0, width , height);

    const auto arrowColour = findColour (PopupMenu::textColourId);
    drawUpDownArrowButton (g, arrowColour, isScrollUpArrow, 0, 0, width, height);
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
    textBorder.reduce (comboBoxButtonWidth + 23, 11);
    g.setColour (Colours::hotpink);
    //g.fillRect (textBorder);
    // MARK: End debug paint

    const Colour arrowColour = comboBox.isPopupActive() ? RBD::arrowActiveColour
                             : comboBox.findColour (ComboBox::arrowColourId);
    drawUpDownArrowButton (g, arrowColour, false, buttonX, buttonY, buttonW, buttonH);
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
        label.setBorderSize ({ 0, 0, largeFontBottomOffset, largeFontRightOffset });
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
    int itemHeight = popupMenuItemHeight;
    int width = comboBox.getWidth();

    auto* parent = comboBox.findParentComponentOfClass<MainPanel>();
    jassert (parent != nullptr);

    return PopupMenu::Options().withTargetComponent (&comboBox)
                               .withItemThatMustBeVisible (comboBox.getSelectedId())
                               .withInitiallySelectedItem (comboBox.getSelectedId())
                               .withMaximumNumColumns (1)
                               .withMinimumWidth (width)
                               .withStandardItemHeight (itemHeight)
                               .withParentComponent (parent);
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
