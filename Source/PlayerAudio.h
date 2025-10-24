#pragma once
#include <JuceHeader.h>

class AudioPlayer
{
public:
    AudioPlayer();
    ~AudioPlayer();

    void loadURL(const juce::URL& audioURL);
    void play();
    void stop();
    void restart();
    void setPosition(double posInSeconds);
    double getPosition() const;
    double getLength() const;
    void skipForward(double seconds);
    void skipBackward(double seconds);
    void setGain(double gain);
    void setSpeed(double ratio);
    void looping(bool shouldLoop);

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource { &transportSource, false, 2 };
    bool isLooping = false;
};
