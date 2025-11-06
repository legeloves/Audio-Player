#include "playlist.h"

void PlaylistAudio::addToPlaylist(const juce::File& file)
{
    if (!file.existsAsFile())
        return;

    if (loadFile(file))
    {
        track t;
        t.FileName = file.getFullPathName();
        t.Title = currentTrack.Title;
        t.Artist = currentTrack.Artist;
        t.hour = currentTrack.hour;
        t.min = currentTrack.min;
        t.sec = currentTrack.sec;

        playlist.push_back(t);

        if (currentTrackIndex == -1)
            currentTrackIndex = 0;
    }
}

void PlaylistAudio::playTrack(int index)
{
    if (index < 0 || index >= playlist.size()) return;
    currentTrackIndex = index;
    loadFile(juce::File(playlist[index].FileName));
    play();
}

void PlaylistAudio::nextTrack()
{
    if (playlist.empty())
        return;

    currentTrackIndex = (currentTrackIndex + 1) % playlist.size();
    playTrack(currentTrackIndex);
}

void PlaylistAudio::previousTrack()
{
    if (playlist.empty())
        return;

    currentTrackIndex = (currentTrackIndex - 1 + playlist.size()) % playlist.size();//(i-1 + n) % n

    playTrack(currentTrackIndex);
}

int PlaylistAudio::getCurrentTrackIndex() const
{
    return currentTrackIndex;
}