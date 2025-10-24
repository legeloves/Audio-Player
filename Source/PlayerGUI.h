#pragma once
#include <JuceHeader.h>
#include "AudioPlayer.h"

class PlayerGUI  : public juce::Component,
                   public juce::Button::Listener,
                   public juce::Slider::Listener,
                   private juce::Timer
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void releaseResources();

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;

private:
    // Buttons
    juce::TextButton loadButton { "Load" };
    juce::TextButton playButton { "Play" };
    juce::TextButton stopButton { "Stop" };
    juce::TextButton restartButton { "Restart" };
    juce::TextButton loopButton { "Loop" };
    juce::TextButton goToStartButton { "Start" };
    juce::TextButton goToEndButton { "End" };
    juce::TextButton skipForwardButton { ">>" };
    juce::TextButton skipBackwardButton { "<<" };
    juce::TextButton muteButton { "Mute" };
    juce::TextButton unmuteButton { "Unmute" };

    // Sliders
    juce::Slider volumeSlider;
    juce::Slider speedSlider;

    // Waveform
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache { 5 };
    juce::AudioThumbnail thumbnail { 512, formatManager, thumbnailCache };

    juce::Label positionLabel;

    // Audio logic
    AudioPlayer playerAudio;
    bool loopwork = false;
    bool isMuted = false;
    double lastVolume = 0.5;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayerGUI)
};
