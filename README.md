# Audio Player

Desktop audio player in C++ on JUCE 7 — transport controls, a playlist queue,
A/B looping, named markers and session restore.

## Features

- **Two players, one mixer.** A second player can be shown and mixed through
  `juce::MixerAudioSource`.
- **Playlist queue** — next / previous, or load a single file.
- **A/B loop** — set two points on the playhead and loop between them.
- **Markers** — add a named mark at the current position, jump to any of them
  from a dropdown, clear them all.
- **Sleep timer** — stop playback after a set number of minutes.
- **Transport** — play, stop, restart, skip ±10s, go to start / end, seek slider,
  volume, playback speed.
- **Session restore** — last file, volume and marks are saved through
  `juce::ApplicationProperties` and reloaded on the next start.

## Source layout

| File | Role |
|---|---|
| `Source/MainComponent.*` | app shell, mixer, session save/load |
| `Source/PlayerGUI.*` | controls, layout, `timerCallback` playhead updates |
| `Source/PlayerAudio.*` | `juce::AudioSource` built on `AudioTransportSource` + `ResamplingAudioSource` |
| `Source/playlist.*` | queue layered on top of the transport |

## Build

JUCE 7.0.x is not vendored — CMake looks for a checkout at `./JUCE`, or pass
`-DJUCE_DIR`.

```bash
git clone -b 7.0.12 --depth 1 https://github.com/juce-framework/JUCE.git JUCE
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Team project — Ahmed Eltaher Bakry, Youssef Mohamed, Bavly, Mostafa.
