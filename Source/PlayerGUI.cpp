#include "MainComponent.h"


MainComponent::MainComponent()
{
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

    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
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
        int X = (getWidth() - buttonWidth) /2; 
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

    
    resized();
}

void MainComponent::hidePlayer2()
{
    player2Visibility = false;
    player2.setVisible(false);
   player2Button.setButtonText("Show Player 2");

    mixer.removeInputSource(&player2.getAudioSource());

    
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
    }
}
