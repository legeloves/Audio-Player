#include "PlayerGUI.h"
#include "playlist.h"
//playerGUI.cpp
PlayerGUI::PlayerGUI() {
    // Add buttons
    for (auto* btn : { &loadButton , &restartButton , &stopButton , &playButton , &loopButton
         , &goToStartButton , &goToEndButton , &skipForwardButton, &skipBackwardButton ,&muteButton,
        &unmuteButton, &nextButton , &previousButton , &A_Button , &B_Button , &clear_AB_Button})
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

    // position slider
    startTimer(50);
    positionSlider.setRange(0.0, 1.0);
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);

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
    nextButton.setBounds(480, 220,80,40);
    previousButton.setBounds(380,220,80,40);
    A_Button.setBounds(20, 220, 80, 40);
    B_Button.setBounds(140, 220, 80, 40);
   clear_AB_Button.setBounds(260, 220, 100, 40);






    volumeSlider.setBounds(20, 100, getWidth() - 40, 30);

    positionSlider.setBounds(20, 180, getWidth() - 40, 20);


    infoLabel.setBounds(getWidth()/2-70 ,250, 150, 150);

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
        playlistAudio.setPosition(playlistAudio.getLength());
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

    else if (button == &A_Button)
    {
        loop_A = playlistAudio.getPosition();
    }

    else if (button == &B_Button)
    {
        loop_B = playlistAudio.getPosition();
        if (loop_A >= 0 && loop_B > loop_A)
            A_BLoop = true;
    }

    else if (button == &clear_AB_Button)
    {
        A_BLoop = false;
        loop_A = loop_B = -1.0;
    }

}
void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playlistAudio.setGain((float)slider->getValue());
    if (slider == &positionSlider)
    {
        double length = playlistAudio.getLength();
        double Pos = positionSlider.getValue() * length;
        playlistAudio.setPosition(Pos);
    }

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

void PlayerGUI::timerCallback()
{
    double len = playlistAudio.getLength();
    if (len > 0.0)
    {
        double position = playlistAudio.getPosition() / len;
        positionSlider.setValue(position, juce::dontSendNotification);
    }
    if (A_BLoop)
    {
        double p = playlistAudio.getPosition();
        if (p >= loop_B)
            playlistAudio.setPosition(loop_A);
    }

}
PlayerGUI :: ~PlayerGUI()  = default;


