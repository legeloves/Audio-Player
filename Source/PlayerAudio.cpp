    #include "PlayerAudio.h"
    //PlayerAudio.cpp
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

        double currentpothision = transportSource.getCurrentPosition();
        double length  = transportSource.getLengthInSeconds();

        if (currentpothision >= length - 0.1) {
            if (theloop == true) {
                transportSource.setPosition(0.0);
                transportSource.start();
            }
            else
                transportSource.stop();

        }

    }

    void PlayerAudio::releaseResources()
    {
        transportSource.releaseResources();
    }

    bool PlayerAudio::loadFile(const juce::File &file) {
        if (file.existsAsFile())
        {
            if (auto* reader = formatManager.createReaderFor(file))
            {
                // 🔑 Disconnect old source first
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
                transportSource.start();


                // ------- Fill track info -------
                currentTrack.FileName = file.getFileName();


                const int time = reader->lengthInSamples / reader->sampleRate;

                    currentTrack.hour = time / 3600;
                    currentTrack.min = (time - currentTrack.hour * 3600) / 60;
                    currentTrack.sec = time - currentTrack.hour * 3600 - currentTrack.min * 60;


                    auto titleTag = reader->metadataValues["TITLE"];
                    auto artistTag = reader->metadataValues["ARTIST"];

                    currentTrack.Title = titleTag.isNotEmpty() ? titleTag : file.getFileNameWithoutExtension();
                    currentTrack.Artist = artistTag.isNotEmpty() ? artistTag : "";

                // --------------------------------



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

    void PlayerAudio::skipBackward( double seconds)
    {
        double newPosition = transportSource.getCurrentPosition() - seconds;
        if (newPosition <0)
        {
            newPosition = 0;
        }
        transportSource.setPosition(newPosition);
    }


    PlayerAudio::~PlayerAudio() {

    }
