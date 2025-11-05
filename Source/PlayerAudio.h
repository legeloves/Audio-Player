#pragma once							// PlayerAudio.h
#include <JuceHeader.h>

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
public:
    PlayerAudio();
    ~PlayerAudio();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    bool loadFile(const juce::File& file);
    void play();
    void stop();
    juce::File getCurrentFile() const;
    void setGain(float gain);
    void setPosition(double pos);
    double getPosition() const;
    double getLength() const;
    void looping(bool loop);
    void skipForward(double seconds);
    void skipBackward(double seconds);


    void addMark(double time, const juce::String& name = "");
    void visitMark(int pos);
    void deleteAllMarks();
    std::vector<std::pair<double, juce::String>> getMarks() const;

    track currentTrack;
    track getTrackInfo() const
    {
        return currentTrack;
    };





private:

    std::vector<std::pair<double, juce::String>> myMarks;

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;


    juce::File currentFile;


    bool theloop = false;


};

