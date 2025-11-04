#pragma once						// PlayerGUI.h
#include <JuceHeader.h>
#include "PlayerAudio.h"
#include "Playlist.h" // playlist

class PlayerGUI : public juce::Component,
                  public juce::Button::Listener,
                  public juce::Slider::Listener,
                  public juce::Timer
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void resized() override;
    void paint(juce::Graphics& g) override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    void timerCallback() override;

private:
    PlaylistAudio playlistAudio;

    double loop_A = -1.0;
    double loop_B = -1.0;
    bool A_BLoop = false;
    bool loopwork = false;
    bool isMuted = false;
    double lastVolume = 0.5;

    // GUI elements
    juce::TextButton loadButton{ "Load File" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton loopButton{ "Loop" };
    juce::TextButton goToEndButton{ "Go to End" };
    juce::TextButton goToStartButton{ "Go to Start" };
    juce::TextButton skipBackwardButton{ "<<" };
    juce::TextButton skipForwardButton{ ">>" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton unmuteButton{ "Unmute" };
    juce::TextButton nextButton{ "Next" };
    juce::TextButton previousButton{ "Previous" };
    juce::TextButton A_Button{ "A" };
    juce::TextButton B_Button{ "B" };
    juce::TextButton clear_AB_Button{ "Clear A_B" };

    juce::Label infoLabel;
    juce::Slider volumeSlider;
    juce::Slider positionSlider;
    std::unique_ptr<juce::FileChooser> fileChooser;

    void updateTrackInfo();
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};