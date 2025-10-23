#include "PlayerGUI.h"

PlayerGUI::PlayerGUI() {
    // Add buttons


    juce::Colour Pink = juce::Colour(0xfff8b5d0);
    juce::Colour Blue = juce::Colour(0xff87ceeb);

    for (auto* btn : { &loadButton , &restartButton , &stopButton , &playButton , &loopButton
         , &goToStartButton , &goToEndButton , &skipForwardButton, &skipBackwardButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);

        btn->setColour(juce::TextButton::buttonColourId, Pink);
        btn->setColour(juce::TextButton::buttonOnColourId, Blue);
        btn->setColour(juce::TextButton::textColourOnId, juce::Colours::black);
        btn->setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    }
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
}
void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}
void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e90ff));

}
void PlayerGUI::resized()
{
   
    loadButton.setBounds(20, 20, 100, 40);      
    playButton.setBounds(130, 20, 80, 40);      
    stopButton.setBounds(220, 20, 80, 40);      
    restartButton.setBounds(310, 20, 80, 40);   
    loopButton.setBounds(400, 20, 80, 40);      

        goToStartButton.setBounds(20, 70, 100, 40);   
    skipBackwardButton.setBounds(130, 70, 80, 40); 
    skipForwardButton.setBounds(220, 70, 80, 40);  
    goToEndButton.setBounds(310, 70, 100, 40);     

    
    volumeSlider.setBounds(20, 120, 460, 30);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        juce::FileChooser chooser("Select audio files...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    playerAudio.loadFile(file);
                }
            });
    }
    else if (button == &playButton) {

        playerAudio.play();
    }
    else if (button == &restartButton)
    {
        playerAudio.setPosition(0.0);
        playerAudio.play();
    }
    else if (button == &stopButton)
    {
        playerAudio.stop();

    }
    else if (button == &loopButton) {
        loopwork = !loopwork;
        playerAudio.looping(loopwork);

    }
    else if (button == &goToStartButton)
    {
        playerAudio.setPosition(0.0);
    }
    else if (button == &goToEndButton)
    {
        playerAudio.setPosition(playerAudio.getLength());
    }
    else if (button == &skipForwardButton)
    {
        playerAudio.skipForward(10);
    }
    else if (button == &skipBackwardButton)
    {
        playerAudio.skipBackward(10);
    }

}
void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
}




PlayerGUI::~PlayerGUI() {}

