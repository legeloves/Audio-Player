#include "PlayerAudio.h"

PlayerAudio::PlayerAudio() {
    formatManager.registerBasicFormats();
}



void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock(bufferToFill);

    double currentposition = transportSource.getCurrentPosition();
    double length = transportSource.getLengthInSeconds();

    if (currentposition >= length - 0.1) {
        if (theloop == true) {
            transportSource.setPosition(0.0);
            transportSource.start();
        }
        else
        {
            transportSource.stop();
        }

    }

}

void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file) {
    if (file.existsAsFile())
    {
        currentFile = file;
        if (auto* reader = formatManager.createReaderFor(file))
        {

            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

            // Create new reader source
            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

            // Attach safely
            transportSource.setSource(readerSource.get(),
                0,
                nullptr,
                reader->sampleRate);

            transportSource.setPosition(0.0);
            transportSource.start();

            currentTrack.FileName = file.getFileName();


            const int time = reader->lengthInSamples / reader->sampleRate;

            currentTrack.hour = time / 3600;
            currentTrack.min = (time - currentTrack.hour * 3600) / 60;
            currentTrack.sec = time - currentTrack.hour * 3600 - currentTrack.min * 60;


            auto titleTag = reader->metadataValues["TITLE"];
            auto artistTag = reader->metadataValues["ARTIST"];

            currentTrack.Title = titleTag.isNotEmpty() ? titleTag : file.getFileNameWithoutExtension();
            currentTrack.Artist = artistTag.isNotEmpty() ? artistTag : "";
        }
    }
    return true;
}

void PlayerAudio::play() {
    transportSource.start();
}

void PlayerAudio::stop() {
    transportSource.stop();
}

void PlayerAudio::setGain(float gain) {
    transportSource.setGain(gain);
}

void PlayerAudio::setPosition(double pos) {
    transportSource.setPosition(pos);
}

double PlayerAudio::getPosition() const {
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLength() const {
    return transportSource.getLengthInSeconds();
}

void PlayerAudio::looping(bool loop) {
    theloop = loop;
}

void PlayerAudio::skipForward(double seconds)
{
    double newPosition = transportSource.getCurrentPosition() + seconds;
    if (newPosition > transportSource.getLengthInSeconds())
    {
        newPosition = transportSource.getLengthInSeconds();
    }
    transportSource.setPosition(newPosition);
}

void PlayerAudio::skipBackward(double seconds)
{
    double newPosition = transportSource.getCurrentPosition() - seconds;
    if (newPosition < 0)
    {
        newPosition = 0;
    }
    transportSource.setPosition(newPosition);
}
juce::File PlayerAudio::getCurrentFile() const
{
    return currentFile;
}


std::vector<std::pair<double, juce::String>> PlayerAudio::getMarks() const
{
    return myMarks;
}
void PlayerAudio::deleteAllMarks()
{
    myMarks.clear();
}

void PlayerAudio::addMark(double time, const juce::String& name)
{

    if (time >= 0 && time <= transportSource.getLengthInSeconds())
    {

        juce::String markerName = name;
        if (markerName.isEmpty())
        {
            markerName = "Mark " + juce::String(myMarks.size() + 1);
        }


        myMarks.push_back(std::make_pair(time, markerName));

        std::sort(myMarks.begin(), myMarks.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });
    }
}

void PlayerAudio::visitMark(int pos)
{
    if (pos >= 0 && pos < myMarks.size())
    {
        transportSource.setPosition(myMarks[pos].first);
    }
}

PlayerAudio::~PlayerAudio() {

}




