#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    // Register formats for waveform and player
    formatManager.registerBasicFormats();

    // Define colors
    juce::Colour Pink = juce::Colour(0xfff8b5d0);
    juce::Colour Blue = juce::Colour(0xff87ceeb);

    // Add buttons
    for (auto* btn : { &loadButton, &restartButton, &stopButton, &playButton, &loopButton,
                       &goToStartButton, &goToEndButton, &skipForwardButton, &skipBackwardButton,
                       &muteButton, &unmuteButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);

        btn->setColour(juce::TextButton::buttonColourId, Pink);
        btn->setColour(juce::TextButton::buttonOnColourId, Blue);
        btn->setColour(juce::TextButton::textColourOnId, juce::Colours::black);
        btn->setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    }

    // Volume slider setup
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    // Speed slider setup
    speedSlider.setRange(0.5, 2.0, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);

    // Position label
    positionLabel.setText("0.0 / 0.0", juce::dontSendNotification);
    positionLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(positionLabel);

    // Waveform setup
    thumbnail.addChangeListener([this]() { repaint(); });
    startTimerHz(30); // update waveform position 30 times per second
}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::releaseResources() {}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e90ff)); // blue background

    // Draw waveform
    auto waveformArea = getLocalBounds().reduced(20, 150);
    g.setColour(juce::Colours::black);
    g.drawRect(waveformArea);

    if (thumbnail.getNumChannels() > 0)
    {
        g.setColour(juce::Colours::white);
        thumbnail.drawChannels(g, waveformArea, 0.0, thumbnail.getTotalLength(), 1.0f);

        // Draw position pointer
        double posRatio = playerAudio.getLength() > 0.0
                            ? playerAudio.getPosition() / playerAudio.getLength()
                            : 0.0;
        int posX = waveformArea.getX() + (int)(posRatio * waveformArea.getWidth());
        g.setColour(juce::Colours::red);
        g.drawLine((float)posX, (float)waveformArea.getY(),
                   (float)posX, (float)waveformArea.getBottom(), 2.0f);
    }
    else
    {
        g.setColour(juce::Colours::white);
        g.drawFittedText("No audio loaded", waveformArea, juce::Justification::centred, 1);
    }

    // Update progress label
    double current = playerAudio.getPosition();
    double total = playerAudio.getLength();
    juce::String text = juce::String(current, 1) + " / " + juce::String(total, 1);
    positionLabel.setText(text, juce::dontSendNotification);
}

void PlayerGUI::resized()
{
    int y = 20;

    loadButton.setBounds(20, y, 100, 40);
    playButton.setBounds(140, y, 80, 40);
    restartButton.setBounds(240, y, 80, 40);
    stopButton.setBounds(340, y, 80, 40);
    loopButton.setBounds(440, y, 80, 40);
    goToStartButton.setBounds(540, y, 100, 40);
    goToEndButton.setBounds(660, y, 100, 40);
    skipForwardButton.setBounds(780, y, 80, 40);
    skipBackwardButton.setBounds(870, y, 80, 40);
    muteButton.setBounds(970, y, 80, 40);
    unmuteButton.setBounds(1060, y, 80, 40);

    volumeSlider.setBounds(20, 80, getWidth() - 40, 30);
    speedSlider.setBounds(20, 120, getWidth() - 40, 30);

    positionLabel.setBounds(20, getHeight() - 40, getWidth() - 40, 30);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        juce::FileChooser chooser("Select an audio file...");
        if (chooser.browseForFileToOpen())
        {
            auto file = chooser.getResult();
            playerAudio.loadURL(juce::URL(file));
            thumbnail.setSource(new juce::FileInputSource(file));
        }
    }
    else if (button == &playButton)
    {
        playerAudio.play();
    }
    else if (button == &stopButton)
    {
        playerAudio.stop();
    }
    else if (button == &restartButton)
    {
        playerAudio.restart();
    }
    else if (button == &loopButton)
    {
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
    else if (button == &muteButton)
    {
        if (!isMuted)
        {
            lastVolume = volumeSlider.getValue();
            playerAudio.setGain(0.0);
            isMuted = true;
        }
    }
    else if (button == &unmuteButton)
    {
        if (isMuted)
        {
            playerAudio.setGain(lastVolume);
            volumeSlider.setValue(lastVolume, juce::dontSendNotification);
            isMuted = false;
        }
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider && !isMuted)
    {
        playerAudio.setGain(volumeSlider.getValue());
    }
    else if (slider == &speedSlider)
    {
        playerAudio.setSpeed(speedSlider.getValue());
    }
}

void PlayerGUI::timerCallback()
{
    repaint();
}
