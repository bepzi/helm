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
#ifndef LFO_SECTION_H
#define LFO_SECTION_H

#include <JuceHeader.h>
#include "open_gl_wave_viewer.h"
#include "synth_section.h"
#include "retrigger_selector.h"
#include "tempo_selector.h"
#include "wave_selector.h"
#include "wave_viewer.h"

class LfoSection : public SynthSection {
  public:
    LfoSection(String name, std::string value_preprend, bool retrigger, bool can_animate = false);
    ~LfoSection();

    void paintBackground(Graphics& g) override;
    void resized() override;
    void reset() override;

  private:
    bool can_animate_;
    std::unique_ptr<OpenGLWaveViewer> wave_viewer_;
    std::unique_ptr<WaveSelector> wave_selector_;

    std::unique_ptr<RetriggerSelector> retrigger_;
    std::unique_ptr<SynthSlider> amplitude_;
    std::unique_ptr<SynthSlider> frequency_;
    std::unique_ptr<SynthSlider> tempo_;
    std::unique_ptr<TempoSelector> sync_;
    std::unique_ptr<ModulationButton> modulation_button_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LfoSection)
};

#endif // LFO_SECTION_H
