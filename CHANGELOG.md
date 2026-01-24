# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.0.1-develop] - 2026-01-24

### Added
- **Firmware Versioning**: Introduced a versioning system in `include/param.h` (Major, Minor, Patch, Branch).
- **Serial Reset**: Added functionality to reset the ESP32 via serial commands.
- **Project Setup**: Configured `.vscode/extensions.json` with recommended extensions (PlatformIO, C++ tools) for optimal development.
- **Hardware ID**: Defined `BOARD_VERSION` for the ESP32-3248S035 hardware.

### Changed
- **Documentation Overhaul**: 
    - Redesigned `README.md` with professional headers and emojis.
    - Added a modular "Firmware Design" section documenting Application, Web Server, Display, Network, and Storage interfaces.
    - Refined "Build Pre-requisites" to clearly outline manual `TFT_eSPI` and `LVGL` configuration steps.
- **Repository Maintenance**: 
    - Cleaned up the `develop` branch history.
    - Aborted problematic rebases and established a clean, force-pushed state for the branch.

### Fixed
- **Push Conflicts**: Resolved non-fast-forward push errors and merged diverging remote changes.
- **Library Configurations**: Ensured build flags in `platformio.ini` correctly include the necessary header paths and debug levels.
