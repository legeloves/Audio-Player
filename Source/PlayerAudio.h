#pragma once							// PlayerAudio.h
#include <JuceHeader.h>
#include<vector>

class PlayerAudio
{
protected:
    struct track {
        juce::String Title;
        juce::String Artist;
        juce::String FileName;
        int hour = 0;
        int min = 0;
        int sec = 0;
    };

private:

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    bool theloop = false;

  


public:
    PlayerAudio();
    ~PlayerAudio();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    bool loadFile(const juce::File& file);
    void play();
    void stop();
    void setGain(float gain);
    void setPosition(double pos);
    double getPosition() const;
    double getLength() const;
    void looping(bool loop);
    void skipForward( double seconds) ;
    void skipBackward(double seconds);


  
    track currentTrack;
    track getTrackInfo() const
    {
        return currentTrack;
    };
};

