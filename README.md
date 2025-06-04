# D30 Linux POS Development Guide

## Project Overview

This is a Linux-based POS (Point of Sale) application built using the LVGL UI framework. The project is structured as a cross-compiled application targeting ARM Linux systems.

## Development Environment Setup

### Required Tools

1. Cross-compiler toolchain:
   - GCC ARM  Please refer to [DevelopEnvironmentGuide](https://github.com/DspreadOrg/qpos-linux-tools/blob/e71932079c738a85cd89bbcffd716318075275ef/EnvironmentBuilding/DevelopEnvironmentGuide.md) file.
2. Build system:
   - CMake
   - Make

### Directory Structure

```
app_demo/
├── build/ # Build output directory 
├── inc/ # Header files
│ └── dspread/ # Device-specific headers
├── pkg_bat/ # Build scripts
├── release/ # Release binaries
│ └── bin/ # Stripped executables
└── src/ # Source code
├── business/ # Business logic
├── http/ # HTTP communication
├── lvgl/ # LVGL UI framework
│ ├── porting/ # Platform-specific adaptations
│ └── src/ # LVGL source
├── network/ # Network functionality
├── ota/ # Over-the-air updates
├── ui/ # UI implementation
└── utils/ # Utility functions
```

## Building the Application

### Using Batch Scripts

For Windows development environment:

1.Regular build:

   cd app_demo\pkg_bat
   compile.bat

2.Clean and rebuild:

   cd app_demo\pkg_bat
   recompile.bat



## UI Development with LVGL

This project uses LVGL (Light and Versatile Graphics Library) for the user interface. Key UI files include:

- `src/ui/ui_enter_pin.c`: PIN entry screen
- `src/ui/ui_qrcode.c`: QR code display screen

### LVGL Features Used

- Widgets: Labels, Lines, Buttons
- Drawing: Lines, Layers, Letters
- Extra components: PNG support, LED widgets, Menu widgets

## LVGL Configuration

The project uses LVGL for UI rendering. Key LVGL components include:

- File system drivers (e.g., POSIX implementation)
- Bidirectional text support
- Custom UI components (PIN entry, QR code, settings screens)

## Development Workflow

1. Set up the development environment with required tools
2. Configure the build mode (debug/production) in the appropriate batch file
3. Build the project using the compile or recompile scripts
4. Test the application
5. Package the application for deployment

## Deployment

After packaging, the application can be deployed to the target POS device. The main executable is  LVGL_UI

which is specified in the ympkg.xml  configuration.

