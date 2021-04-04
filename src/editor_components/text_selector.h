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

#ifndef TEXT_SELECTOR_H
#define TEXT_SELECTOR_H

#include <JuceHeader.h>
#include "synth_slider.h"

class TextSelector : public SynthSlider {
  public:
    TextSelector(String name);

    void mouseDown(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;

    void setLongStringLookup(const std::string* lookup) { long_lookup_ = lookup; }

  private:
    const std::string* long_lookup_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextSelector)
};

#endif // TEXT_SELECTOR_H
