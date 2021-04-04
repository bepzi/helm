add_library(mopo STATIC
  src/alias.cpp
  src/arpeggiator.cpp
  src/biquad_filter.cpp
  src/bit_crush.cpp
  src/bypass_router.cpp
  src/delay.cpp
  src/distortion.cpp
  src/envelope.cpp
  src/feedback.cpp
  src/formant_manager.cpp
  src/ladder_filter.cpp
  src/linear_slope.cpp
  src/magnitude_lookup.cpp
  src/memory.cpp
  src/midi_lookup.cpp
  src/mono_panner.cpp
  src/operators.cpp
  src/oscillator.cpp
  src/portamento_slope.cpp
  src/processor.cpp
  src/processor_router.cpp
  src/resonance_lookup.cpp
  src/reverb_all_pass.cpp
  src/reverb_comb.cpp
  src/reverb.cpp
  src/sample_decay_lookup.cpp
  src/simple_delay.cpp
  src/smooth_filter.cpp
  src/smooth_value.cpp
  src/state_variable_filter.cpp
  src/step_generator.cpp
  src/stutter.cpp
  src/trigger_operators.cpp
  src/value.cpp
  src/voice_handler.cpp)

target_include_directories(mopo PUBLIC src)

target_compile_features(mopo PUBLIC cxx_std_11)
