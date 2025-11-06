#pragma once							// PlayerAudio.h
#include <JuceHeader.h>

class PlayerAudio : public juce::AudioSource
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

    std::unique_ptr<juce::ResamplingAudioSource> resampler;
    double currentSampleRate = 0.0;

public:
    PlayerAudio();
    ~PlayerAudio() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate)override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)override;
    void releaseResources()override;

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


    void setSleep(double minutes);
    void stopSleep();
    bool isSleepTimerActive() const { return sleepTimer; }
    double getTime() const;
    std::vector<std::pair<double, juce::String>> getMarks() const;

    track currentTrack;

    track getTrackInfo() const
    {
        return currentTrack;
    };

    void setSpeed(double speed);



private:

    std::vector<std::pair<double, juce::String>> myMarks;

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;


    bool sleepTimer = false;
    double sleepduration = 0.0;
    double sleepstart = 0.0;
    juce::File currentFile;


    bool theloop = false;


};