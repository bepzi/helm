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
#ifndef STUTTER_SECTION_H
#define STUTTER_SECTION_H

#include <JuceHeader.h>
#include "synth_button.h"
#include "synth_section.h"
#include "synth_slider.h"
#include "tempo_selector.h"

class StutterSection : public SynthSection {
  public:
    StutterSection(String name);
    ~StutterSection();

    void paintBackground(Graphics& g) override;
    void resized() override;

  private:
    ScopedPointer<SynthSlider> stutter_frequency_;
    ScopedPointer<SynthSlider> stutter_tempo_;
    ScopedPointer<TempoSelector> stutter_sync_;

    ScopedPointer<SynthSlider> resample_frequency_;
    ScopedPointer<SynthSlider> resample_tempo_;
    ScopedPointer<TempoSelector> resample_sync_;

    ScopedPointer<SynthSlider> stutter_softness_;
    ScopedPointer<SynthButton> on_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StutterSection)
};

#endif // STUTTER_SECTION_H
