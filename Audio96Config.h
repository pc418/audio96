#pragma once

// Audio96 global configuration.
// Keep this visible before AudioStream.h so standalone object headers use 96 kHz.
#ifndef AUDIO_SAMPLE_RATE_EXACT
#define AUDIO_SAMPLE_RATE_EXACT 96000.0f
#endif

#ifndef AUDIO_SAMPLE_RATE
#define AUDIO_SAMPLE_RATE AUDIO_SAMPLE_RATE_EXACT
#endif

#define AUDIO96_FIXED_SAMPLE_RATE 96000.0f
