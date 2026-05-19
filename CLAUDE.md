# Maintainer Notes

## CI/CD

- This repo currently has legacy `.travis.yml` only; no current GitHub Actions workflow was present during the Audio96 audit.
- For practical verification, compile a Teensy 4.1 sketch with Teensyduino 1.59: `arduino-cli compile -b teensy:avr:teensy41 <sketch-path>`.
- Do not use Teensyduino 1.60 for this repo; it is intentionally avoided because of a blocking 1.60 bug in this environment.
- The available hardware validation context is Teensy 4.1 only. See `AUDIO96_AUDIT_REPORT.md` for the 96 kHz port audit and import decisions.
