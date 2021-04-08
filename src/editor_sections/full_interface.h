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
#ifndef FULL_INTERFACE_H
#define FULL_INTERFACE_H

#include <JuceHeader.h>

#include "about_section.h"
#include "arp_section.h"
#include "bpm_section.h"
#include "contribute_section.h"
#include "global_tool_tip.h"
#include "open_gl_modulation_manager.h"
#include "oscilloscope.h"
#include "open_gl_background.h"
#include "open_gl_oscilloscope.h"
#include "open_gl_modulation_meter.h"
#include "overlay.h"
#include "patch_browser.h"
#include "patch_selector.h"
#include "synthesis_interface.h"
#include "synth_section.h"
#include "update_check_section.h"

class FullInterface : public SynthSection, public OpenGLRenderer {
  public:
    FullInterface(mopo::control_map controls, mopo::output_map modulation_sources,
                  mopo::output_map mono_modulations, mopo::output_map poly_modulations,
                  MidiKeyboardState* keyboard_state);
    ~FullInterface();

    void setOutputMemory(const float* output_memory);

    void createModulationSliders(mopo::output_map modulation_sources,
                                 mopo::output_map mono_modulations,
                                 mopo::output_map poly_modulations);

    void setToolTipText(String parameter, String value);

    void paint(Graphics& g) override;
    void paintBackground(Graphics& g) override;
    void resized() override;
    void buttonClicked(Button* clicked_button) override;
    void animate(bool animate = true) override;
    void checkBackground();

    void newOpenGLContextCreated() override;
    void renderOpenGL() override;
    void openGLContextClosing() override;

    void resetModulations() { modulation_manager_->reset(); }
    void setFocus() { synthesis_interface_->setFocus(); }
    void notifyChange() { patch_selector_->setModified(true); }
    void notifyFresh();
    void externalPatchLoaded(File patch) { patch_browser_->externalPatchLoaded(patch); }

  private:
    std::map<std::string, SynthSlider*> slider_lookup_;
    std::map<std::string, Button*> button_lookup_;
    std::unique_ptr<OpenGLModulationManager> modulation_manager_;
    std::unique_ptr<SynthSlider> arp_tempo_;

    std::unique_ptr<AboutSection> about_section_;
    std::unique_ptr<ContributeSection> contribute_section_;
    std::unique_ptr<UpdateCheckSection> update_check_section_;
    std::unique_ptr<Component> standalone_settings_section_;
    std::unique_ptr<ImageButton> logo_button_;
    std::unique_ptr<ArpSection> arp_section_;
    std::unique_ptr<SynthesisInterface> synthesis_interface_;
    std::unique_ptr<OpenGLOscilloscope> oscilloscope_;
    std::unique_ptr<BpmSection> bpm_section_;
    std::unique_ptr<GlobalToolTip> global_tool_tip_;
    std::unique_ptr<PatchSelector> patch_selector_;
    std::unique_ptr<PatchBrowser> patch_browser_;
    std::unique_ptr<SaveSection> save_section_;
    std::unique_ptr<DeleteSection> delete_section_;
    std::unique_ptr<VolumeSection> volume_section_;

    bool animate_;
    OpenGLContext open_gl_context;
    Image background_image_;
    OpenGLBackground background_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FullInterface)
};

#endif // FULL_INTERFACE_H
