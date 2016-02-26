# OSVR HDK Video Status App
> Maintained at <https://github.com/sensics/OSVR-HDK-Video-Status>
>
> For details, see <http://osvr.github.io>
>
> For support, see <http://support.osvr.com>

This is a very minimal, command-line application useful for troubleshooting.

On recent firmwares, the OSVR HDK reports, in the upper bits of the tracking report version number byte, status data that includes the video input status as it's seen received at the HMD. That is, it can report the "ground truth" about the video signal reaching the device after any operating system and device driver control panels perform their rotations and other mangling on it. The OSVR driver for the HDK decodes this information and reports it in an analog (serving as an `enum` of sorts) on the HDK device, at the semantic path `semantic/status/videoStatus`.

This application opens that semantic path and registers a callback, so that it constantly receives this video status data. As soon as it initially receives the first status data, and every time the status changes, it will print to the console a message with the video status "decoded" to English:

- **unknown/unavailable**
	- usually means you have a very old HDK (or prototype) or very old firmware that doesn't report this data.
	- Contact support, there may be a firmware upgrade available for you.
- **no input**
	- usually means one of the following:
		- HDMI unplugged
		- display disabled
		- using "direct mode" but no application is running
		- may also appear briefly during the transition from extended mode to running a direct mode application on some systems.
- **portrait video input**
	- receiving 1080x1920, native resolution input that can be scanned directly to the display
	- highest performance mode
- **landscape video input**
	- receiving 1920x1080, landscape video input
	- Increased compatibility with:
		- "Mirror"/"Clone" mode
		- Some HDMI extenders/transmitters/switches
		- Some other HDMI accessories
	- Necessarily adds 1 frame (16ms) latency because of having to receive the full frame in landscape before being able to scan it out to the display in portrait

## Building

This should be able to build essentially anywhere OSVR builds - its only dependencies are OSVR's ClientKit C++ interface (which currently also requires a few Boost headers as of Feb 2016), a C++11 compiler and standard library, and CMake 3.1+. See the "Releases" link on GitHub for pre-built binary snapshots.

## License

This project: Licensed under the Apache License, Version 2.0.