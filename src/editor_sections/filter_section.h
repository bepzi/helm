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
#ifndef FILTER_SECTION_H
#define FILTER_SECTION_H

#include <JuceHeader.h>
#include "filter_selector.h"
#include "filter_response.h"
#include "synth_button.h"
#include "synth_section.h"
#include "text_slider.h"

class FilterSection : public SynthSection {
  public:
    FilterSection(String name);
    ~FilterSection();

    void paintBackground(Graphics& g) override;
    void resized() override;
    void reset() override;
    void sliderValueChanged(Slider* changed_slider) override;

    void resizeLowPass(float x, float y, float width, float height);
    void resizeHighPass(float x, float y, float width, float height);

    void setActive(bool active) override;
    void resetResponse();

  private:
    std::unique_ptr<SynthButton> filter_on_;
    std::unique_ptr<FilterSelector> filter_shelf_;
    std::unique_ptr<SynthSlider> cutoff_;
    std::unique_ptr<SynthSlider> resonance_;
    std::unique_ptr<SynthSlider> blend_;
    std::unique_ptr<FilterResponse> filter_response_;
    std::unique_ptr<SynthSlider> fil_env_depth_;
    std::unique_ptr<SynthSlider> keytrack_;
    std::unique_ptr<TextSlider> filter_style_;
    std::unique_ptr<SynthSlider> drive_;

    Path low_pass_;
    Path high_pass_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterSection)
};

#endif // FILTER_SECTION_H
