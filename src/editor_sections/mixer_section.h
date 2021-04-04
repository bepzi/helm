/* Copyright 2013-2017 Matt Tytel
 *
 * helm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef MIXER_SECTION_H
#define MIXER_SECTION_H

#include <JuceHeader.h>
#include "synth_section.h"

class MixerSection : public SynthSection {
  public:
    MixerSection(String name);
    ~MixerSection();

    void paintBackground(Graphics& g) override;
    void resized() override;

  private:
    ScopedPointer<SynthSlider> osc_1_;
    ScopedPointer<SynthSlider> osc_2_;
    ScopedPointer<SynthSlider> sub_;
    ScopedPointer<SynthSlider> noise_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MixerSection)
};

#endif // MIXER_SECTION_H
