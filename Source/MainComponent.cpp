#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(playerGUI);
    setSize(1200, 600);
}

MainComponent::~MainComponent() {}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void MainComponent::resized()
{
    playerGUI.setBounds(getLocalBounds());
}
