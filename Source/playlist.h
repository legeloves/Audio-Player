/*
==============================================================================

    playlist.h
    Created: 3 Nov 2025 1:52:06am
    Author:  Bavly

  ==============================================================================
*/

#pragma once
#include "PlayerAudio.h"
#include <vector>

class PlaylistAudio : public PlayerAudio
{
public:
    PlaylistAudio() = default;


    void addToPlaylist(const juce::File& file);

    void playTrack(int index);

    void nextTrack();

    void previousTrack();

    int getCurrentTrackIndex() const;

    const std::vector<track>& getPlaylist() const { return playlist; }

    const track& getCurrentTrack() const { return playlist[currentTrackIndex]; }


private:
    std::vector<track> playlist;
    int currentTrackIndex = -1;
};