#pragma once						// PlayerGUI.h
#include <JuceHeader.h>
#include "PlayerAudio.h"
#include "PlaylistAudio.h"



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


    void paintOverChildren(juce::Graphics& g) override;
    void play();

    std::unique_ptr<juce::FileChooser> fileChooser;
    juce::String formatTime(double seconds);
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    void updateTrackInfo();


    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();


    bool loadFile(const juce::File& file) { return playlistAudio.loadFile(file); }
    void setPosition(double pos) { playlistAudio.setPosition(pos); }
    juce::File getCurrentFile() { return playlistAudio.getCurrentFile(); }
    double getPosition() const { return playlistAudio.getPosition(); }
    void timerCallback() override;

private:
    PlaylistAudio playlistAudio;
    double loop_A = -1.0;
    double loop_B = -1.0;
    bool A_BLoop = false;


    bool loopwork = false;
    bool isMuted = false;
    double lastVolume = 0.5;


    juce::Label currentTimeLabel;
    juce::Label totalTimeLabel;


    // GUI elements
    juce::TextButton loadButton{ "Load File" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton playButton{ "play" };
    juce::TextButton loopButton{ "loop" };
    juce::TextButton goToEndButton{ "Go to end" };
    juce::TextButton goToStartButton{ "Go to start" };
    juce::TextButton skipBackwardButton{ "<<" };
    juce::TextButton skipForwardButton{ ">>" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton unmuteButton{ "Unmute" };
    juce::TextButton nextButton{ "Next" };
    juce::TextButton previousButton{ "Previous" };
    juce::TextButton A_Button{ "A" };
    juce::TextButton B_Button{ "B" };
    juce::TextButton clear_AB_Button{ "clear A_B" };

    juce::TextButton addMarkButton{ " Add marker" };
    juce::TextButton clearMarksButton{ "Clear all markers" };
    juce::Label infoLabel;


    juce::Slider timeSlider;
    juce::Slider volumeSlider;




    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};

