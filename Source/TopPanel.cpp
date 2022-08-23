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

    // MARK: Remove to re-enable keyboard control
    mPresetList->setWantsKeyboardFocus (false);

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

    // MARK: Remove to re-enable keyboard control
    mNewPreset->setWantsKeyboardFocus (false);
    
    mSavePreset = std::make_unique<TextButton>();
    bounds.translate (buttonWidth + 2, 0);
    mSavePreset->setBounds (bounds);
    mSavePreset->setButtonText ("SAVE");
    mSavePreset->addListener (this);
    addAndMakeVisible (mSavePreset.get());

    // MARK: Remove to re-enable keyboard control
    mSavePreset->setWantsKeyboardFocus (false);
    
    mSaveAsPreset = std::make_unique<TextButton>();
    bounds.translate (buttonWidth + 2, 0);
    mSaveAsPreset->setBounds (bounds);
    mSaveAsPreset->setButtonText ("SAVE AS");
    mSaveAsPreset->addListener (this);
    addAndMakeVisible (mSaveAsPreset.get());

    // MARK: Remove to re-enable keyboard control
    mSaveAsPreset->setWantsKeyboardFocus (false);

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
    
    mSaveAsDialogue = std::make_unique<AlertWindow> ("Save As",
                                                     "Please enter a name for the preset",
                                                     AlertWindow::NoIcon);
    
    // FIXME: Properly position the dialogue window in relation to the plugin
    mSaveAsDialogue->centreAroundComponent (this, getWidth(), getHeight());
    
    mSaveAsDialogue->addTextEditor ("presetName", currentPresetName, "Preset name: ");
    mSaveAsDialogue->addButton ("Save", 1);
    mSaveAsDialogue->addButton ("Cancel", 0);

    mSaveAsDialogue->enterModalState(true,
        ModalCallbackFunction::create([this, presetManager](int result)
        {
            mSaveAsDialogue->exitModalState (result);

            if (result == 1)
            {
                const String presetName
                = mSaveAsDialogue->getTextEditorContents ("presetName");
                presetManager->saveAsPreset (presetName);
                updatePresetList();
            }

            // Delete the Save As dialogue window
            mSaveAsDialogue.reset();
        }));
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

    // If the text is a valid Preset Name, the corresponding preset will be
    //  selected as well. Don't send notification so that updating the list
    //  doesn't reload the preset and discard user changes.
    mPresetList->setText (presetManager->getCurrentPresetName(),
                          dontSendNotification);
}
