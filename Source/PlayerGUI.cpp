#include "PlayerGUI.h"
#include "Playlist.h"

PlayerGUI::PlayerGUI() {

    juce::Colour backgroundColour = juce::Colour(0xFF1C2424);  
    juce::Colour buttonColour = juce::Colour(0xFF373B3B);          juce::Colour sliderColour = juce::Colour(0xFF27F5F5);      
    juce::Colour textColour = juce::Colours::white;

    juce::Colour Pink = juce::Colour(0xfff8b5d0);
    juce::Colour Blue = juce::Colour(0xff87ceeb);

    for (auto* btn : { &loadButton , &restartButton , &stopButton , &playButton , &loopButton
        , &goToStartButton , &goToEndButton , &skipForwardButton, &skipBackwardButton ,&muteButton,
       &unmuteButton, &nextButton , &previousButton , &A_Button , &B_Button , &clear_AB_Button,&addMarkButton
        , &clearMarksButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
        btn->setColour(juce::TextButton::buttonColourId, buttonColour);
        btn->setColour(juce::TextButton::buttonOnColourId, sliderColour); 
        btn->setColour(juce::TextButton::textColourOnId, textColour);
        btn->setColour(juce::TextButton::textColourOffId, textColour);
    }


    marksComboBox.addListener(this);
    marksComboBox.setColour(juce::ComboBox::backgroundColourId, buttonColour);
    marksComboBox.setColour(juce::ComboBox::textColourId, textColour);
    marksComboBox.setColour(juce::ComboBox::arrowColourId, textColour);
    marksComboBox.setTextWhenNothingSelected("Select Mark");
    addAndMakeVisible(marksComboBox);

    sleepTimerButton.addListener(this);
    addAndMakeVisible(sleepTimerButton);
    sleepTimerButton.setColour(juce::TextButton::buttonColourId, buttonColour);
    sleepTimerButton.setColour(juce::TextButton::buttonOnColourId, sliderColour);
    sleepTimerButton.setColour(juce::TextButton::textColourOnId, textColour);
    sleepTimerButton.setColour(juce::TextButton::textColourOffId, textColour);

    sleepTimerCombo.addItem("5 minutes", 1);
    sleepTimerCombo.addItem("15 minutes", 2);
    sleepTimerCombo.addItem("30 minutes", 3);
    sleepTimerCombo.addItem("60 minutes", 4);
    sleepTimerCombo.setSelectedId(0);
    sleepTimerCombo.setColour(juce::ComboBox::backgroundColourId, buttonColour);
    sleepTimerCombo.setColour(juce::ComboBox::textColourId, textColour);
    sleepTimerCombo.setColour(juce::ComboBox::arrowColourId, textColour);
    addAndMakeVisible(sleepTimerCombo);

    timeSlider.setRange(0.0, 1.0, 0.0001);
    timeSlider.setValue(0.0);
    timeSlider.addListener(this);
    timeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    timeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    timeSlider.setColour(juce::Slider::trackColourId, sliderColour);
    timeSlider.setColour(juce::Slider::thumbColourId, textColour);
    timeSlider.setColour(juce::Slider::backgroundColourId, buttonColour);
    addAndMakeVisible(timeSlider);

    infoLabel.setText("No track loaded", juce::dontSendNotification);
    infoLabel.setJustificationType(juce::Justification::centred);
    infoLabel.setColour(juce::Label::textColourId, textColour);
    infoLabel.setColour(juce::Label::backgroundColourId, juce::Colours::transparentBlack);
    infoLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    addAndMakeVisible(infoLabel);

    currentTimeLabel.setText("0:00", juce::dontSendNotification);
    currentTimeLabel.setJustificationType(juce::Justification::centred);
    currentTimeLabel.setColour(juce::Label::textColourId, textColour);
    addAndMakeVisible(currentTimeLabel);

    totalTimeLabel.setText("0:00", juce::dontSendNotification);
    totalTimeLabel.setJustificationType(juce::Justification::centred);
    totalTimeLabel.setColour(juce::Label::textColourId, textColour);
    addAndMakeVisible(totalTimeLabel);

    startTimer(50);

    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    volumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    volumeSlider.setColour(juce::Slider::trackColourId, sliderColour);
    volumeSlider.setColour(juce::Slider::thumbColourId, textColour);
    volumeSlider.setColour(juce::Slider::backgroundColourId, buttonColour);
    addAndMakeVisible(volumeSlider);


    speedSlider.setRange(0.5, 2.0, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);
    speedSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 60, 20);
    speedSlider.setColour(juce::Slider::trackColourId, sliderColour);
    speedSlider.setColour(juce::Slider::thumbColourId, textColour);
    speedSlider.setColour(juce::Slider::backgroundColourId, buttonColour);
    speedSlider.setColour(juce::Slider::textBoxBackgroundColourId, buttonColour);
    speedSlider.setColour(juce::Slider::textBoxTextColourId, textColour);
    speedSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(speedSlider);
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
    g.fillAll(juce::Colour(0xFF1C2424)); 
}

void PlayerGUI::resized()
{
    int width = getWidth();
    int height = getHeight();
    int size = 50;
    int center = width / 2;

    previousButton.setBounds(center - 150, height - 80, size, size);
    skipBackwardButton.setBounds(center - 90, height - 80, size, size);
    playButton.setBounds(center - 30, height - 85, 60, 70);
    skipForwardButton.setBounds(center + 40, height - 80, size, size);
    nextButton.setBounds(center + 100, height - 80, size, size);


    marksComboBox.setBounds(width - 220, 65, 200, 25);

    sleepTimerButton.setBounds(width - 150, height - 100, 100, 30);
    sleepTimerCombo.setBounds(width - 150, height - 65, 100, 25);

    stopButton.setBounds(center - 160, height - 140, 60, 30);
    restartButton.setBounds(center - 90, height - 140, 60, 30);
    loopButton.setBounds(center + 30, height - 140, 60, 30);
    goToStartButton.setBounds(center - 250, height - 140, 80, 30);
    goToEndButton.setBounds(center + 100, height - 140, 80, 30);

    loadButton.setBounds(20, 20, 100, 30);
    muteButton.setBounds(130, 20, 80, 30);
    unmuteButton.setBounds(220, 20, 80, 30);
    addMarkButton.setBounds(width - 220, 20, 100, 35);
    clearMarksButton.setBounds(width - 110, 20, 90, 35);

    A_Button.setBounds(20, height - 50, 40, 30);
    B_Button.setBounds(70, height - 50, 40, 30);
    clear_AB_Button.setBounds(120, height - 50, 80, 30);

    int timelineY = 120;
    int timelineWidth = width - 160;

    currentTimeLabel.setBounds(center - 250, timelineY - 25, 60, 20);
    timeSlider.setBounds(center - 180, timelineY, 360, 25);
    totalTimeLabel.setBounds(center + 190, timelineY - 25, 60, 20);

    infoLabel.setBounds(center - 200, 10, 400, 35);

    volumeSlider.setBounds(width - 50, 80, 30, height - 200);

    speedSlider.setBounds(center - 180, timelineY + 40, 360, 25);
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
                    playlistAudio.addToPlaylist(file);
                    updateTrackInfo();

                    auto info = playlistAudio.getTrackInfo();
                    juce::String text = info.Artist.isNotEmpty() ? info.Artist + " - " + info.Title : info.Title;

                    if (info.hour > 0) {
                        text = text + " | "
                            + juce::String(info.hour) + " : "
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
    else if (button == &playButton) {
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
    else if (button == &loopButton) {
        loopwork = !loopwork;
        playlistAudio.looping(loopwork);

        if (loopwork) {
            loopButton.setButtonText("loop ON");
        }
        else
            loopButton.setButtonText("loop OF");
    }
    else if (button == &goToStartButton)
    {
        playlistAudio.setPosition(0.0);
    }
    else if (button == &goToEndButton)
    {
        playlistAudio.setPosition(playlistAudio.getLength());
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
    else if (button == &muteButton)
    {
        if (!isMuted)
        {
            lastVolume = volumeSlider.getValue();
            playlistAudio.setGain(0.0);
            isMuted = true;
        }
    }
    else if (button == &previousButton)
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
    else if (button == &skipForwardButton)
    {
        playlistAudio.skipForward(10);
    }
    else if (button == &skipBackwardButton)
    {
        playlistAudio.skipBackward(10);
    }
    else if (button == &addMarkButton)
    {

        double time = playlistAudio.getPosition();
        playlistAudio.addMark(time);
        updateMarksComboBox();
    }
    else if (button == &clearMarksButton)
    {
        playlistAudio.deleteAllMarks();
        marksComboBox.clear();
    }
    else if (button == &sleepTimerButton)
    {
        if (sleepTimerCombo.getSelectedId() == 0)
        {
            playlistAudio.stopSleep();
            sleepTimer = false;
            sleepTimerButton.setButtonText("Sleep Timer");
        }
        else
        {
            double minutes = 0;
            switch (sleepTimerCombo.getSelectedId())
            {
            case 1: minutes = 5; break;
            case 2: minutes = 15; break;
            case 3: minutes = 30; break;
            case 4: minutes = 60; break;
            }

            playlistAudio.setSleep(minutes);
            sleepTimer = true;
            sleepTimerButton.setButtonText("Cancel Timer");
        }
    }
}


void PlayerGUI::comboBoxChanged(juce::ComboBox* comboBox)
{
    if (comboBox == &marksComboBox)
    {
        int selectedId = marksComboBox.getSelectedId();
        if (selectedId > 0)
        {
            
            playlistAudio.visitMark(selectedId - 1);
        }
    }
}


void PlayerGUI::updateMarksComboBox()
{
    marksComboBox.clear();

    auto marks = playlistAudio.getMarks();
    for (int i = 0; i < marks.size(); ++i)
    {
        const auto& mark = marks[i];
        juce::String displayText = juce::String(i + 1) + ": " + formatTime(mark.first);
        if (mark.second.isNotEmpty())
        {
            displayText += " - " + mark.second;
        }
        marksComboBox.addItem(displayText, i + 1);  
    }
}


void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playlistAudio.setGain((float)slider->getValue());
    else if (slider == &timeSlider)
    {
        if (slider->isMouseButtonDown())
        {
            double totalTime = playlistAudio.getLength();
            if (totalTime > 0)
            {
                double Time = slider->getValue() * totalTime;
                playlistAudio.setPosition(Time);
            }
        }
    }
    else if (slider == &speedSlider)
    {
        playlistAudio.setSpeed(speedSlider.getValue());
    }
}

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

void PlayerGUI::paintOverChildren(juce::Graphics& g)
{
    if (playlistAudio.getLength() > 0)
    {
        auto marks = playlistAudio.getMarks();
        double audioTime = playlistAudio.getLength();

        if (audioTime > 0 && !marks.empty())
        {
            auto sliderBounds = timeSlider.getBounds();
            int audioY = sliderBounds.getY() + sliderBounds.getHeight() / 2;

            for (int i = 0; i < marks.size(); i++)
            {
                const auto& mark = marks[i];
                double markTime = mark.first;
                double position = markTime / audioTime;
                int x = sliderBounds.getX() + static_cast<int>(position * sliderBounds.getWidth());

                juce::Colour colour = juce::Colour(0xFF27F5F5);
                g.setColour(colour);
                juce::Path triangle;
                triangle.addTriangle(
                    static_cast<float>(x - 6), static_cast<float>(audioY - 10),
                    static_cast<float>(x + 6), static_cast<float>(audioY - 10),
                    static_cast<float>(x), static_cast<float>(audioY - 2)
                );
                g.fillPath(triangle);

                g.drawLine(static_cast<float>(x),
                    static_cast<float>(audioY - 2),
                    static_cast<float>(x),
                    static_cast<float>(audioY + 8),
                    2.0f);

                g.setColour(juce::Colours::white);
                g.setFont(9.0f);
                g.drawText(juce::String(i + 1),
                    x - 6, audioY - 22, 12, 10,
                    juce::Justification::centred);
            }
        }
    }
}

juce::String PlayerGUI::formatTime(double seconds)
{
    if (seconds < 0)
    {
        seconds = 0;
    }
    int totalSeconds = static_cast<int>(seconds);
    int minutes = totalSeconds / 60;
    int secs = totalSeconds % 60;

    return juce::String::formatted("%d:%02d", minutes, secs);
}

void PlayerGUI::timerCallback()
{
    double totalTime = playlistAudio.getLength();
    double currentTime = playlistAudio.getPosition();

    if (totalTime > 0)
    {
        double position = currentTime / totalTime;
        timeSlider.setValue(position, juce::dontSendNotification);

        currentTimeLabel.setText(formatTime(currentTime), juce::dontSendNotification);
        totalTimeLabel.setText(formatTime(totalTime), juce::dontSendNotification);
    }
    else
    {
        currentTimeLabel.setText("0:00", juce::dontSendNotification);
        totalTimeLabel.setText("0:00", juce::dontSendNotification);
    }

    if (A_BLoop)
    {
        double p = playlistAudio.getPosition();
        if (p >= loop_B)
            playlistAudio.setPosition(loop_A);
    }

    if (sleepTimer && playlistAudio.isSleepTimerActive())
    {
        double remaining = playlistAudio.getTime();
        if (remaining > 0)
        {
            int mins = (int)(remaining / 60);
            int secs = (int)remaining % 60;
            sleepTimerButton.setButtonText("Stop in " + juce::String(mins) + ":" + juce::String(secs).paddedLeft('0', 2));
        }
    }
    static size_t lastMarkElement = 0;
    auto currentMarks = playlistAudio.getMarks();
    if (currentMarks.size() != lastMarkElement)
    {
        updateMarksComboBox();
        lastMarkElement = currentMarks.size();
    }
    repaint();
}

PlayerGUI::~PlayerGUI() {}
