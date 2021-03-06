cmake_minimum_required(VERSION 3.15)

# Set a default build type if none was specified
set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

project(HELM
  VERSION 0.10.0
  DESCRIPTION "A cross-platform polyphonic synthesizer, by Matt Tytel."
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# It's "bad form" to set this globally, but it's convenient.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ==================================================

add_subdirectory(concurrentqueue)
add_subdirectory(mopo)
add_subdirectory(JUCE)

juce_add_binary_data(HelmData SOURCES
  fonts/DroidSansMono.ttf
  fonts/Roboto-Regular.ttf
  fonts/Roboto-Light.ttf
  fonts/Roboto-Thin.ttf
  images/helm_icon_16_1x.png
  images/helm_icon_16_2x.png
  images/helm_icon_32_1x.png
  images/helm_icon_32_2x.png
  images/helm_icon_128_1x.png
  images/helm_icon_128_2x.png
  images/helm_icon_256_1x.png
  images/helm_icon_256_2x.png
  images/helm_icon_512_1x.png
  images/helm_icon_512_2x.png
  images/modulation_selected_active_1x.png
  images/modulation_selected_active_2x.png
  images/modulation_selected_inactive_1x.png
  images/modulation_selected_inactive_2x.png
  images/modulation_unselected_active_1x.png
  images/modulation_unselected_active_2x.png
  images/modulation_unselected_inactive_1x.png
  images/modulation_unselected_inactive_2x.png)

# ==================================================

juce_add_plugin(HelmPlugin
  ICON_BIG images/helm_icon_512_2x.png
  ICON_SMALL images/helm_icon_128_2x.png

  IS_SYNTH TRUE
  NEEDS_MIDI_INPUT TRUE
  NEEDS_MIDI_OUTPUT FALSE
  IS_MIDI_EFFECT FALSE
  EDITOR_WANTS_KEYBOARD_FOCUS TRUE

  PRODUCT_NAME "helm"
  COMPANY_NAME "Matt Tytel"
  PLUGIN_MANUFACTURER_CODE "Tyte"
  FORMATS VST3 AU LV2

  LV2_URI "http://tytel.org/helm"
  LV2_WANTS_LATENCY FALSE)

juce_generate_juce_header(HelmPlugin)

target_sources(HelmPlugin PRIVATE
  src/common/border_bounds_constrainer.cpp
  src/common/file_list_box_model.cpp
  src/common/helm_common.cpp
  src/common/load_save.cpp
  src/common/midi_manager.cpp
  src/common/startup.cpp
  src/common/synth_base.cpp
  src/common/synth_gui_interface.cpp
  src/editor_components/bpm_slider.cpp
  src/editor_components/filter_response.cpp
  src/editor_components/filter_selector.cpp
  src/editor_components/global_tool_tip.cpp
  src/editor_components/graphical_envelope.cpp
  src/editor_components/graphical_step_sequencer.cpp
  src/editor_components/midi_keyboard.cpp
  src/editor_components/modulation_button.cpp
  src/editor_components/modulation_highlight.cpp
  src/editor_components/modulation_meter.cpp
  src/editor_components/modulation_slider.cpp
  src/editor_components/open_gl_background.cpp
  src/editor_components/open_gl_component.cpp
  src/editor_components/open_gl_envelope.cpp
  src/editor_components/open_gl_modulation_meter.cpp
  src/editor_components/open_gl_oscilloscope.cpp
  src/editor_components/open_gl_peak_meter.cpp
  src/editor_components/open_gl_wave_viewer.cpp
  src/editor_components/oscilloscope.cpp
  src/editor_components/patch_selector.cpp
  src/editor_components/retrigger_selector.cpp
  src/editor_components/synth_button.cpp
  src/editor_components/synth_slider.cpp
  src/editor_components/tempo_selector.cpp
  src/editor_components/text_selector.cpp
  src/editor_components/text_slider.cpp
  src/editor_components/wave_selector.cpp
  src/editor_components/wave_viewer.cpp
  src/editor_components/xy_pad.cpp
  src/editor_sections/about_section.cpp
  src/editor_sections/arp_section.cpp
  src/editor_sections/bpm_section.cpp
  src/editor_sections/contribute_section.cpp
  src/editor_sections/delay_section.cpp
  src/editor_sections/delete_section.cpp
  src/editor_sections/distortion_section.cpp
  src/editor_sections/dynamic_section.cpp
  src/editor_sections/envelope_section.cpp
  src/editor_sections/extra_mod_section.cpp
  src/editor_sections/feedback_section.cpp
  src/editor_sections/filter_section.cpp
  src/editor_sections/formant_section.cpp
  src/editor_sections/full_interface.cpp
  src/editor_sections/lfo_section.cpp
  src/editor_sections/mixer_section.cpp
  src/editor_sections/modulation_manager.cpp
  src/editor_sections/noise_section.cpp
  src/editor_sections/open_gl_modulation_manager.cpp
  src/editor_sections/oscillator_section.cpp
  src/editor_sections/patch_browser.cpp
  src/editor_sections/reverb_section.cpp
  src/editor_sections/save_section.cpp
  src/editor_sections/step_sequencer_section.cpp
  src/editor_sections/stutter_section.cpp
  src/editor_sections/sub_section.cpp
  src/editor_sections/synthesis_interface.cpp
  src/editor_sections/synth_section.cpp
  src/editor_sections/update_check_section.cpp
  src/editor_sections/voice_section.cpp
  src/editor_sections/volume_section.cpp
  src/look_and_feel/browser_look_and_feel.cpp
  src/look_and_feel/colors.cpp
  src/look_and_feel/default_look_and_feel.cpp
  src/look_and_feel/fonts.cpp
  src/look_and_feel/modulation_look_and_feel.cpp
  src/look_and_feel/shaders.cpp
  src/look_and_feel/text_look_and_feel.cpp
  src/plugin/helm_editor.cpp
  src/plugin/helm_plugin.cpp
  src/synthesis/dc_filter.cpp
  src/synthesis/detune_lookup.cpp
  src/synthesis/fixed_point_oscillator.cpp
  src/synthesis/fixed_point_wave.cpp
  src/synthesis/gate.cpp
  src/synthesis/helm_engine.cpp
  src/synthesis/helm_lfo.cpp
  src/synthesis/helm_module.cpp
  src/synthesis/helm_oscillators.cpp
  src/synthesis/helm_voice_handler.cpp
  src/synthesis/noise_oscillator.cpp
  src/synthesis/peak_meter.cpp
  src/synthesis/resonance_cancel.cpp
  src/synthesis/trigger_random.cpp
  src/synthesis/value_switch.cpp)

target_include_directories(HelmPlugin PUBLIC
  src/common
  src/editor_components
  src/editor_sections
  src/look_and_feel
  src/plugin
  src/synthesis)

target_compile_definitions(HelmPlugin PUBLIC
  # Helm tries to detect at runtime whether it's actually installed,
  # or whether it's just being queried for its factory presets by
  # the LV2 .ttl generator. If it's the latter, Helm needs to know
  # where its factory presets are on the developer's disk. Not ideal
  # for a number of reasons, but it'll have to do for now.
  HELM_LV2GEN_FACTORY_PRESET_PATH="${CMAKE_SOURCE_DIR}/patches"

  JUCE_DISPLAY_SPLASH_SCREEN=0
  JUCE_VST3_CAN_REPLACE_VST2=0
  JUCE_WEB_BROWSER=0
  JUCE_USE_CURL=0
  JUCE_USE_XRANDR=0)

target_link_libraries(HelmPlugin
  PUBLIC
    HelmData
    concurrentqueue
    mopo

    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_opengl

    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    # juce::juce_recommended_warning_flags
    )

# ==================================================

juce_add_gui_app(HelmStandalone
  ICON_BIG images/helm_icon_512_2x.png
  ICON_SMALL images/helm_icon_128_2x.png

  PRODUCT_NAME "helm"
  COMPANY_NAME "Matt Tytel")

juce_generate_juce_header(HelmStandalone)

target_sources(HelmStandalone PRIVATE
  src/standalone/helm_editor.cpp
  src/standalone/helm_computer_keyboard.cpp
  src/standalone/main.cpp)

target_include_directories(HelmStandalone PRIVATE
  src/standalone)

target_compile_definitions(HelmStandalone PRIVATE
  JUCE_WASAPI=1
  JUCE_DIRECTSOUND=1
  JUCE_ALSA=1
  JUCE_JACK=1
  JUCE_JACK_CLIENT_NAME="Helm"
  JUCE_ALSA_MIDI_NAME="Helm")

target_link_libraries(HelmStandalone PRIVATE HelmPlugin)
