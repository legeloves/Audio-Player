#include "AudioPlayer.h"

AudioPlayer::AudioPlayer()
{
    formatManager.registerBasicFormats();
}

AudioPlayer::~AudioPlayer() {}

void AudioPlayer::loadURL(const juce::URL& audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr)
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource (new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}

void AudioPlayer::play()
{
    transportSource.start();
}

void AudioPlayer::stop()
{
    transportSource.stop();
}

void AudioPlayer::restart()
{
    setPosition(0.0);
    play();
}

void AudioPlayer::setPosition(double posInSeconds)
{
    transportSource.setPosition(posInSeconds);
}

double AudioPlayer::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double AudioPlayer::getLength() const
{
    return transportSource.getLengthInSeconds();
}

void AudioPlayer::skipForward(double seconds)
{
    setPosition(getPosition() + seconds);
}

void AudioPlayer::skipBackward(double seconds)
{
    setPosition(getPosition() - seconds);
}

void AudioPlayer::setGain(double gain)
{
    transportSource.setGain(gain);
}

void AudioPlayer::setSpeed(double ratio)
{
    resampleSource.setResamplingRatio(ratio);
}

void AudioPlayer::looping(bool shouldLoop)
{
    isLooping = shouldLoop;
    if (readerSource != nullptr)
        readerSource->setLooping(shouldLoop);
}
