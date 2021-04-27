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
    setColour (ComboBox::arrowColourId, RBD::textNormalColour);
    setColour (ComboBox::textColourId, RBD::textNormalColour);

    // Popup colours (for comboBox popup)
    setColour (PopupMenu::backgroundColourId, RBD::popupBgColour);

    // TextButton colours
    setColour (TextButton::buttonColourId, RBD::controlNormalColour);
    setColour (TextButton::textColourOnId, RBD::textNormalColour);
    setColour (TextButton::textColourOffId, RBD::textNormalColour);

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

void PluginLookAndFeel::drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                                           bool /*isSeparator*/, bool /*isActive*/,
                                           bool isHighlighted, bool isTicked,
                                           bool /*hasSubMenu*/, const String& text,
                                           const String& /*shortcutKeyText*/,
                                           const Drawable* /*icon*/,
                                           const Colour* /*passedTextColour*/)
{
    Rectangle<int> bounds (area);   // create editable copy of passed bounds to adjust
    bounds.removeFromBottom (1);
    Colour fillColour = isHighlighted ? RBD::controlActiveColour
                                      : RBD::popupItemBgColour;

    g.setColour (fillColour);
    g.fillRect (bounds);

    Colour textColour = isTicked ? RBD::textActiveColour : RBD::textNormalColour;
    g.setColour (textColour);
    g.setFont (RBD::mainFont);

    // Offset the text
    bounds.setLeft (10);
    bounds.setY (1);

    g.drawFittedText (text, bounds, Justification::left, 1);
}

//= COMBOBOXES =================================================================

Font PluginLookAndFeel::getComboBoxFont (ComboBox& /*comboBox*/)
{
    return RBD::mainFont;
}

void PluginLookAndFeel::drawComboBox (Graphics& g, int width, int height,
                                      bool /*isButtonDown*/,
                                      int /*buttonX*/, int /*buttonY*/,
                                      int /*buttonW*/, int /*buttonH*/,
                                      ComboBox& comboBox)
{
    const Rectangle<float> comboBoxBounds (0.0f, 0.0f, width, height);

    // TODO: Set the comboBox colour in the mouse callbacks rather than here
    const Colour comboBoxColour
    = (comboBox.isPopupActive() || comboBox.isMouseOverOrDragging (true))
                                            ? RBD::controlHoverColour
                                            : RBD::controlNormalColour;
    g.setColour (comboBoxColour);
    g.fillRoundedRectangle (comboBoxBounds, RBD::defaultCornerSize);

    const Rectangle<float> arrowBounds (comboBoxBounds.withWidth (20.0f)
                                            .withRightX (width - 10.0f));

    // Create arrow shape
    Path arrow;
    arrow.startNewSubPath (arrowBounds.getX() + 3.0f,
                           arrowBounds.getCentreY() - 2.0f);
    arrow.lineTo (arrowBounds.getCentreX(), arrowBounds.getCentreY() + 2.0f);
    arrow.lineTo (arrowBounds.getRight() - 3.0f, arrowBounds.getCentreY() - 2.0f);


    const Colour arrowColour
    = comboBox.findColour (ComboBox::arrowColourId)
                          .withAlpha (comboBox.isPopupActive() ? 0.9f : 0.5f);
    g.setColour (arrowColour);
    g.strokePath (arrow, PathStrokeType (2.0f));
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

