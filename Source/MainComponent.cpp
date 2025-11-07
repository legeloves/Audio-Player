#include "MainComponent.h"

MainComponent::MainComponent()
{

    juce::PropertiesFile::Options options;
    options.applicationName = "SimpleAudioPlayer";
    options.filenameSuffix = "settings";
    options.osxLibrarySubFolder = "Application Support";
    appProperties.setStorageParameters(options);

    addAndMakeVisible(player1);

    addAndMakeVisible(player2Button);
    player2Button.addListener(this);
    player2Button.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFF373B3B));
    player2Button.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xFF373B3B));
    player2Button.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    player2Button.setColour(juce::TextButton::textColourOffId, juce::Colours::white);

    player2.setVisible(false);
    addChildComponent(player2);

    setSize(800, 400);

    loadLastSession();

    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    saveLastSession();
    shutdownAudio();
}

void MainComponent::saveLastSession()
{
    juce::PropertiesFile::Options options;
    options.applicationName = "SimpleAudioPlayer";
    options.folderName = "Settings";
    options.filenameSuffix = "settings";

    juce::PropertiesFile settingsFile(options);

    juce::String currentFile = player1.getCurrentFile().getFullPathName();
    double currentPosition = player1.getPosition();

    settingsFile.setValue("lastFile", currentFile);
    settingsFile.setValue("lastPosition", currentPosition);


    settingsFile.save();    

}

void MainComponent::loadLastSession()
{
    juce::PropertiesFile::Options options;
    options.applicationName = "SimpleAudioPlayer";
    options.folderName = "Settings";
    options.filenameSuffix = "settings";

    juce::PropertiesFile settings(options);

    juce::String lastFilePath = settings.getValue("lastFile", "");
    double lastPosition = settings.getDoubleValue("lastPosition", 0.0);

    if (lastFilePath.isNotEmpty())
    {
        juce::File dataFile(lastFilePath);
        if (dataFile.existsAsFile())
        {
            player1.loadFile(dataFile);
            player1.setPosition(lastPosition);
        }
    }


}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);


    if (player2Visibility)
    {
        player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    mixer.addInputSource(&player1.getAudioSource(), false);


    if (player2Visibility)
    {
        mixer.addInputSource(&player2.getAudioSource(), false);
    }

    mixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{

    mixer.getNextAudioBlock(bufferToFill);

}

void MainComponent::releaseResources()
{
    mixer.releaseResources();
    player1.releaseResources();
    if (player2Visibility)
    {
        player2.releaseResources();
    }

}

void MainComponent::resized()
{
    auto area = getLocalBounds();

    int buttonWidth = getWidth() - 100;
    int buttonHeight = 25;
    int X = (getWidth() - buttonWidth) / 2;
    int Y = 10;

    player2Button.setBounds(X, Y, buttonWidth, buttonHeight);
    player2Button.setButtonText("SHOW SECOND PLAYER");

    area.removeFromTop(40);

    if (player2Visibility)
    {
        auto halfHeight = area.getHeight() / 2;
        player1.setBounds(area.removeFromTop(halfHeight));
        player2.setBounds(area);
    }
    else
    {
        player1.setBounds(area);
    }
}

void MainComponent::showPlayer2()
{
    player2Visibility = true;
    player2.setVisible(true);
    player2Button.setButtonText("Hide Player 2");


    mixer.addInputSource(&player2.getAudioSource(), false);

    saveLastSession();
    resized();
}

void MainComponent::hidePlayer2()
{
    player2Visibility = false;
    player2.setVisible(false);
    player2Button.setButtonText("Show Player 2");

    mixer.removeInputSource(&player2.getAudioSource());
    saveLastSession();

    resized();
}

void MainComponent::buttonClicked(juce::Button* button)
{
    if (button == &player2Button)
    {
        if (player2Visibility)
        {
            hidePlayer2();
        }
        else
        {
            showPlayer2();
        }

        saveLastSession();
    }
}
