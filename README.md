<h2 align="center">Helm</h2>

Helm is a free, cross-platform, polyphonic synthesizer that runs on
GNU/Linux, Mac, and Windows as a standalone program and as a
LV2/VST3/AU plugin.

This is a **custom fork** of Helm aimed at keeping its build systems
and dependencies up to date. New features will not be developed in
this repo.

![alt tag](http://tytel.org/static/images/helm_screenshot.png)

### Features

 - 32 voice polyphony
 - Interactive visual interface
 - Powerful modulation system with live visual feedback
 - Dual oscillators with cross modulation and up to 15 unison oscillators each
 - Sub oscillator with shuffle waveshaping
 - Oscillator feedback and saturation for waveshaping
 - 12 different waveforms
 - Blending between 12 or 24dB low/band/high pass filter
 - Low/Band/High Shelf filters
 - 2 monophonic and 1 polyphonic LFO
 - Step sequencer
 - Lots of modulation sources including polyphonic aftertouch
 - Simple arpeggiator
 - Effects: Formant filter, stutter, delay, distortion, reverb

### Building

To build everything:

```bash
cmake -S . -B build
cmake --build build -j$(nproc)
```

To build one type of plugin:

```bash
cmake --build build --target HelmPlugin_<PLUGIN_FORMAT>
```

Where `<PLUGIN_FORMAT>` is one of: `LV2`, `AU`, or `VST3`.

To build the standalone:

```bash
cmake --build build --target HelmStandalone
```
