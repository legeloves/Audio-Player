#include "MainComponent.h"
//MainComponent.cpp

MainComponent::MainComponent()
{
    addAndMakeVisible(player1);
    addAndMakeVisible(player2);


    setSize(600, 250);

    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixer.addInputSource(&player1.getAudioSource(), false);
    mixer.addInputSource(&player2.getAudioSource(), false);

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
    player2.releaseResources();
}
void MainComponent::resized() {
    auto area = getLocalBounds();
    auto halfHeight = area.getHeight() / 2;

    player1.setBounds(area.removeFromTop(halfHeight));
    player2.setBounds(area);
}