/*
  ==============================================================================

    TopPanel.cpp
    Created: 11 Jan 2021 4:33:47pm
    Author:  Anthony

  ==============================================================================
*/

#include "TopPanel.h"

TopPanel::TopPanel (ReallyBasicDelayAudioProcessor& processor)
    : InterfacePanel (processor)
{
    // Set up Panel attributes
    setSize (RBD::topPanelWidth, RBD::topPanelHeight);
    setName ("MainMenu");

    // Set up the Preset List
    const int height = 25;
    const int presetListWidth = 200;
    
    mPresetList = std::make_unique<ComboBox> ("PresetList");
    auto bounds = getLocalBounds().withSizeKeepingCentre (presetListWidth, height);
    mPresetList->setBounds (bounds);
    mPresetList->addListener (this);
    addAndMakeVisible (mPresetList.get());
    updatePresetList();

    // Set up Preset List mouse event callbacks
    WeakReference<Component> presetListWeakReference (mPresetList.get());
    auto repaintPresetList = [presetListWeakReference] (const MouseEvent& /*event*/)
    {
        if (presetListWeakReference != nullptr)
            presetListWeakReference->repaint();
    };

    mPresetListMouseEventInvoker.onMouseEnter = repaintPresetList;
    mPresetListMouseEventInvoker.onMouseExit = repaintPresetList;
    //mPresetListMouseEventInvoker.onMouseUp = repaintPresetList;

    mPresetList->addMouseListener (&mPresetListMouseEventInvoker, true);

    // Set up the Preset Manager buttons
    const int buttonWidth = 65;
    const int padding = 10;
    
    // TODO: Repetitive button initialisation - can we make it more dry?
    //  OwnedArray of buttons and StringArray of labels?
    
    mNewPreset = std::make_unique<TextButton>();
    bounds.setWidth (buttonWidth);
    bounds.setX (padding);
    mNewPreset->setBounds (bounds);
    mNewPreset->setButtonText ("NEW");
    mNewPreset->addListener (this);
    addAndMakeVisible (mNewPreset.get());
    
    mSavePreset = std::make_unique<TextButton>();
    bounds.translate (buttonWidth, 0);
    mSavePreset->setBounds (bounds);
    mSavePreset->setButtonText ("SAVE");
    mSavePreset->addListener (this);
    addAndMakeVisible (mSavePreset.get());
    
    mSaveAsPreset = std::make_unique<TextButton>();
    bounds.translate (buttonWidth, 0);
    mSaveAsPreset->setBounds (bounds);
    mSaveAsPreset->setButtonText ("SAVE AS");
    mSaveAsPreset->addListener (this);
    addAndMakeVisible (mSaveAsPreset.get());

    // Set up the Title Logo
    mTitleLogo = std::make_unique<TitleLogo>();
    bounds = getLocalBounds().withLeft (mPresetList->getRight());
    bounds.removeFromRight (padding);
    mTitleLogo->setBounds (bounds);
    addAndMakeVisible (mTitleLogo.get());
}

void TopPanel::buttonClicked (Button* buttonThatWasPressed)
{
    PresetManager* presetManager = mProcessor.getPresetManager();
    
    if (buttonThatWasPressed == mNewPreset.get())
    {
        presetManager->createNewPreset();
        updatePresetList();
    }
    else if (buttonThatWasPressed == mSavePreset.get())
    {
        if (presetManager->isCurrentPresetSaved())
        {
            presetManager->savePreset();
        }
        else
        {
            displaySaveAsPopup();
        }
    }
    else if (buttonThatWasPressed == mSaveAsPreset.get())
    {
        displaySaveAsPopup();
    }
}
    
void TopPanel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == mPresetList.get())
    {
        PresetManager* presetManager = mProcessor.getPresetManager();
        const int index = mPresetList->getSelectedItemIndex();
        
        presetManager->loadPreset (index);
    }
}
    
void TopPanel::displaySaveAsPopup()
{
    PresetManager* presetManager = mProcessor.getPresetManager();
    
    const String currentPresetName = presetManager->getCurrentPresetName();
    
    AlertWindow saveAsDialogue ("Save As",
                                "Please enter a name for the preset",
                                AlertWindow::NoIcon);
    
    // FIXME: Properly position the dialogue window in relation to the plugin
    saveAsDialogue.centreAroundComponent (this, getWidth(), getHeight());
    
    saveAsDialogue.addTextEditor ("presetName", currentPresetName, "Preset name: ");
    saveAsDialogue.addButton ("Save", 0);
    saveAsDialogue.addButton ("Cancel", 1);

    #warning Jules said to always avoid modal loops in a plugin
    //  Does this apply here? Why should we avoid it? What's the alternative?
    if (saveAsDialogue.runModalLoop() == 0) // if exit code is 0
    {
        const String presetName = saveAsDialogue.getTextEditorContents ("presetName");
        presetManager->saveAsPreset (presetName);
        updatePresetList();
    }
}

void TopPanel::updatePresetList()
{
    PresetManager* presetManager = mProcessor.getPresetManager();
    
    mPresetList->clear (dontSendNotification);  // we'll be rebuilding the list, don't alert listeners
    
    const int numPresets = presetManager->getNumberOfPresets();
    
    for (int i = 0; i < numPresets; ++i)
    {
        // IDs start with 1:
        mPresetList->addItem (presetManager->getPresetName (i), i + 1);
    }

    // This also sets the comboBox selected item, if it's a valid Item Text
    mPresetList->setText (presetManager->getCurrentPresetName());
}
