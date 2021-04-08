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

#include "extra_mod_section.h"

#include "colors.h"
#include "fonts.h"
#include "modulation_look_and_feel.h"

ExtraModSection::ExtraModSection(String name) : SynthSection(name) {
  addModulationButton((aftertouch_mod_ = std::make_unique<ModulationButton>("aftertouch")).get());
  aftertouch_mod_->setLookAndFeel(ModulationLookAndFeel::instance());

  addModulationButton((note_mod_ = std::make_unique<ModulationButton>("note")).get());
  note_mod_->setLookAndFeel(ModulationLookAndFeel::instance());

  addModulationButton((velocity_mod_ = std::make_unique<ModulationButton>("velocity")).get());
  velocity_mod_->setLookAndFeel(ModulationLookAndFeel::instance());

  addModulationButton((mod_wheel_mod_ = std::make_unique<ModulationButton>("mod_wheel")).get());
  mod_wheel_mod_->setLookAndFeel(ModulationLookAndFeel::instance());

  addModulationButton((pitch_wheel_mod_ = std::make_unique<ModulationButton>("pitch_wheel")).get());
  pitch_wheel_mod_->setLookAndFeel(ModulationLookAndFeel::instance());

  addModulationButton((random_mod_ = std::make_unique<ModulationButton>("random")).get());
  random_mod_->setLookAndFeel(ModulationLookAndFeel::instance());
}

ExtraModSection::~ExtraModSection() {
  aftertouch_mod_ = nullptr;
  note_mod_ = nullptr;
  velocity_mod_ = nullptr;
  mod_wheel_mod_ = nullptr;
  pitch_wheel_mod_ = nullptr;
  random_mod_ = nullptr;
}

void ExtraModSection::drawTextToRightOfComponent(Graphics& g, Component* component, String text) {
  float space = size_ratio_ * 6.0f;
  g.drawText(text, component->getRight() + space, component->getY(), getWidth() / 2,
             component->getHeight(), Justification::centredLeft, false);
}

void ExtraModSection::paintBackground(Graphics& g) {
  SynthSection::paintBackground(g);

  g.setColour(Colors::control_label_text);
  g.setFont(Fonts::instance()->proportional_regular().withPointHeight(size_ratio_ * 10.0f));

  drawTextToRightOfComponent(g, aftertouch_mod_.get(), TRANS("AFTERTOUCH"));
  drawTextToRightOfComponent(g, note_mod_.get(), TRANS("NOTE"));
  drawTextToRightOfComponent(g, velocity_mod_.get(), TRANS("VELOCITY"));
  drawTextToRightOfComponent(g, mod_wheel_mod_.get(), TRANS("MOD WHEEL"));
  drawTextToRightOfComponent(g, pitch_wheel_mod_.get(), TRANS("PITCH WHEEL"));
  drawTextToRightOfComponent(g, random_mod_.get(), TRANS("RANDOM"));
}

void ExtraModSection::resized() {
  int button_width = getModButtonWidth();
  int title_width = getTitleWidth();
  int x = size_ratio_ * 30;
  int x2 = getWidth() / 2 + size_ratio_ * 15.0f;
  float space = (getHeight() - title_width - (3.0f * button_width)) / 4.0f;

  aftertouch_mod_->setBounds(x, title_width + space, button_width, button_width);
  note_mod_->setBounds(x, aftertouch_mod_->getBottom() + space, button_width, button_width);
  velocity_mod_->setBounds(x, note_mod_->getBottom() + space, button_width, button_width);
  mod_wheel_mod_->setBounds(x2, title_width + space, button_width, button_width);
  pitch_wheel_mod_->setBounds(x2, mod_wheel_mod_->getBottom() + space, button_width, button_width);
  random_mod_->setBounds(x2, pitch_wheel_mod_->getBottom() + space, button_width, button_width);

  SynthSection::resized();
}
