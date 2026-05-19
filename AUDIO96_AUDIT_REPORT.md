# Audio96 Diff Audit Report

Date: 2026-05-18

## Context

This repository starts from upstream PJRC Audio commit `3039be2` and is intended for open source publishing as a 96 kHz fork. The verified working reference was a private Audio96 tree, tested on the available hardware setup: Teensy 4.1. No other board or codec hardware was available here.

Important hardware/toolchain scope: the known working setup is Teensy 4.1 audio playback with Teensyduino 1.59. Stay on Teensyduino 1.59; 1.60 is intentionally not targeted because it introduced a blocking bug in this environment. PWM output and USB Audio bidirectional streaming to a PC are not working/validated in this fork and should not be presented as supported 96 kHz paths without separate work.

Goal: keep the library aligned with official PJRC Audio while applying the changes needed for fixed 96 kHz operation. Avoid importing project-local custom audio objects or public APIs.

## Imported

- Added `Audio96Config.h` and `Audio96.h`.
- Included `Audio96Config.h` before `AudioStream.h` in standalone object headers, so object-level includes use `AUDIO_SAMPLE_RATE_EXACT == 96000.0f`.
- Renamed Arduino and PlatformIO metadata to `Audio96`.
- Updated README language from 44.1 kHz to 96 kHz and removed unsupported PWM/USB Audio bullets from the public-facing hardware list.
- SGTL5000:
  - Set `CHIP_CLK_CTRL` to 96 kHz, 256*Fs.
  - Changed AVC attack/decay math to use `AUDIO_SAMPLE_RATE_EXACT`.
  - Kept `ana_ctrl` shadow state synchronized after enable.
- TLV320AIC3206:
  - Defaulted `AIC_FS` to 96000.
  - Updated PLL/divider constants for 96 kHz operation.
  - Made ADC HP filter helper default to `AUDIO_SAMPLE_RATE_EXACT`.
- ADC/PDB:
  - Updated Kinetis PDB periods to 96 kHz.
  - Updated ADC DC blocker coefficients/comments for 2 Hz at 96 kHz.
- Playback/tools:
  - `AudioPlaySdWav` now accepts only 96 kHz, 16-bit, stereo PCM, matching the verified Teensy 4.1 setup and avoiding device-side resampling.
  - `AudioPlayMemory` position/length calculations now treat format codes as system, half, and quarter of 96 kHz.
  - Raw playback millis comments no longer mention the old 44.1 kHz constant.
  - `wav2sketch` and GUI docs now describe 96 kHz output.
- Outputs:
  - ADAT clock setup now derives from `AUDIO_SAMPLE_RATE_EXACT * 2`.
  - DAC FTM setup now uses `AUDIO_SAMPLE_RATE_EXACT` instead of the old 44117.64706 literal.
- Quantizer:
  - Added 96 kHz support by reusing the existing 48 kHz coefficients, matching the verified tree behavior.

## Reviewed But Not Imported

- New project-local objects: `effect_iq`, `synth_fft`, `synth_iq`, `synth_naturalism`, `synth_schroeder`, and `play_dual_buffer`.
- `Audio.h` includes for those custom objects.
- `AudioPlayQueue::queuedBuffers()` / `freeBuffers()`: useful project-local API but not required for 96 kHz and would expand the public surface.
- `output_i2s.cpp`: imported the verified-tree SAI register changes because the newer PJRC symbols `I2S_TCSR_FEIE` and `I2S_TCR1_TFW` did not compile against the required Teensyduino 1.59 / Teensy 4.1 toolchain. This is one of the intentional differences from current upstream.
- `output_pwm.cpp`: the verified reference changed DMA allocation style, but PWM output is not a working/validated target in this 96 kHz fork. The file is left aligned with current upstream aside from fixed-rate config includes.
- `effect_fade.cpp/h`, `effect_envelope.cpp`, `effect_reverb.cpp`, `effect_freeverb.cpp`, `utility/dspinst.h`, `input_tdm.cpp`, `input_tdm2.cpp`, `output_tdm2.cpp`, and wavetable example PROGMEM edits: reviewed as non-96 kHz or likely older/local behavior. These were left aligned with current upstream except for the mechanical `Audio96Config.h` include where needed.
- Private-tree docs about custom objects and project-specific behavior were not copied.

## Verification Notes

- Static checks should confirm:
  - No project-local custom objects are referenced.
  - Headers that include `AudioStream.h` include `Audio96Config.h` first.
  - Remaining `44100`/`44117` references outside examples are either compatibility constants, source sample metadata, or sample-rate tables rather than active fixed-rate configuration.
- Compile check performed with `teensy:avr 1.59.0`: a temporary Teensy 4.1 sketch including `Audio96.h`, `AudioSynthWaveformSine`, `AudioOutputI2S`, and `AudioControlSGTL5000` compiled successfully.
- Hardware verification is limited to the prior private Teensy 4.1 result. This repository was audited and statically checked, but not flashed to hardware during this port. PWM output and USB Audio need separate implementation/testing before support claims.
- Toolchain policy: use Teensyduino 1.59 for verification and development. Do not upgrade verification to Teensyduino 1.60.

## Future Test Command

When Teensyduino 1.59 tooling is available, compile at least one Teensy 4.1 sketch against this repository:

```bash
arduino-cli compile -b teensy:avr:teensy41 <sketch-path>
```

For playback validation, use a 96 kHz, 16-bit, stereo PCM WAV and confirm the sketch reports or audibly plays the expected file.
