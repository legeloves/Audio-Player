#include "PlayerGUI.h"
#include "playlist.h"
//playerGUI.cpp
PlayerGUI::PlayerGUI() {
    // Add buttons
    for (auto* btn : { &loadButton , &restartButton , &stopButton , &playButton , &loopButton
         , &goToStartButton , &goToEndButton , &skipForwardButton, &skipBackwardButton ,&muteButton, &unmuteButton, &nextButton , &previousButton})
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }
    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    //lableInfo
    addAndMakeVisible(infoLabel);
    infoLabel.setText("No track loaded", juce::dontSendNotification);
    infoLabel.setJustificationType(juce::Justification::centred);
    infoLabel.setColour(juce::Label::backgroundColourId, juce::Colours::darkblue);


}
void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playlistAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playlistAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playlistAudio.releaseResources();
}
void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}
void PlayerGUI::resized()
{
    int y = 20;

    loadButton.setBounds(20, y, 100, 40);
     playButton.setBounds(140, y, 80, 40);
    restartButton.setBounds(240, y, 80, 40);
    stopButton.setBounds(340, y, 80, 40);
    loopButton.setBounds(440,y,80,40);
    goToStartButton.setBounds(540, y, 100, 40);
    goToEndButton.setBounds(660, y, 100, 40);

    skipForwardButton.setBounds(780, y, 80, 40);
    skipBackwardButton.setBounds(900, y, 80, 40);
    muteButton.setBounds(1020, y, 80, 40);
    unmuteButton.setBounds(1140, y, 80, 40);
    nextButton.setBounds(1260,y,80,40);
    previousButton.setBounds(1380,y,80,40);




    volumeSlider.setBounds(20, 100, getWidth() - 40, 30);

    infoLabel.setBounds(getWidth()/2-70 ,140, 150, 150);

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
                    playlistAudio.loadFile(file);

                    //add to play list , update
                    playlistAudio.addToPlaylist(file);
                    updateTrackInfo();


                    auto info = playlistAudio.getTrackInfo();
                    juce::String text =  info.Artist.isNotEmpty() ? info.Artist + " - " + info.Title : info.Title;


                    if (info.hour > 0) {
                        text=text +  " | "
                            +juce::String(info.hour) + " : " 
                            + juce::String(info.min) + " : " 
                            + juce::String(info.sec) + " hours";
                    }
                    else if (info.min > 0) {
                        text = text + " | "
                            + juce::String(info.min) + " : "
                            + juce::String(info.sec) + " min";
                    }
                    else {
                        text = text + " | "
                            + juce::String(info.sec) + " sec";

                    }
                    
                    infoLabel.setText(text, juce::dontSendNotification);



                }
            });
    }
    else if (button == &playButton){

        playlistAudio.play();
    }
    else if (button == &restartButton)
    {
        playlistAudio.setPosition(0.0);
        playlistAudio.play();
    }
    else if (button == &stopButton)
    {
        playlistAudio.stop();

    }
    else if (button == &loopButton){
           loopwork = !loopwork;
           playlistAudio.looping(loopwork);

    }
    else if (button == &goToStartButton)
    {
        playlistAudio.setPosition(0.0);
    }
    else if (button == &goToEndButton)
    {
        playlistAudio.setPosition(playerAudio.getLength());
    }
    else if (button == &skipForwardButton)
   {
        playlistAudio.skipForward(10);
   }
    else if (button == &skipBackwardButton)
   {
        playlistAudio.skipBackward(10);
   }
    else if (button == &muteButton)
    {
        if (!isMuted)
        {
            lastVolume = volumeSlider.getValue();
            playlistAudio.setGain(0.0);
            isMuted = true;
        }
    }
    else if (button == &unmuteButton)
    {
        if (isMuted)
        {
            playlistAudio.setGain(lastVolume);
            volumeSlider.setValue(lastVolume, juce::dontSendNotification);
            isMuted = false;
        }
    }



    else if (button == &nextButton) {
        playlistAudio.nextTrack();
  
        updateTrackInfo();
    }


    else if(button == &previousButton)
    {
        playlistAudio.previousTrack();
   
        updateTrackInfo();
    }

    


}
void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
}


//update
void PlayerGUI::updateTrackInfo() {
    auto info = playlistAudio.getCurrentTrack();
    juce::String text = info.Artist.isNotEmpty() ? info.Artist + " - " + info.Title : info.Title;

    if (info.hour > 0) {
        text += " | " + juce::String(info.hour) + " : " + juce::String(info.min) + " : " + juce::String(info.sec) + " hours";
    }
    else if (info.min > 0) {
        text += " | " + juce::String(info.min) + " : " + juce::String(info.sec) + " min";
    }
    else {
        text += " | " + juce::String(info.sec) + " sec";
    }

    infoLabel.setText(text, juce::dontSendNotification);
}




PlayerGUI::~PlayerGUI() {}


