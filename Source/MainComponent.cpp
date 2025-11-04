#include "MainComponent.h"
//MainComponent.cpp

MainComponent::MainComponent()
{
addAndMakeVisible(player1);
setSize(600, 250);
setAudioChannels(0, 2);
loadLastSession();

}

MainComponent::~MainComponent()
{
saveCurrentSession();
shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
player1.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
player1.releaseResources();
}
void MainComponent::resized() {
player1.setBounds(getBounds());
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

void MainComponent::saveCurrentSession()
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