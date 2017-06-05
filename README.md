# libbl
A header-only library for manipulating the Kuando BusyLight brand of status lights.

## Motivation
The [Kuando BusyLight](http://www.busylight.com) is a great Lync / Skype for Business presence indicator light that's useful in office scenarios - it reflects your status in Lync or Skype for Business into the physical world and lets folks know if it's okay to bother you or not.

Windows support is fantastic, but macOS and Linux support are lacking. I work from home, and use this light to indicate to my family when I'm busy or not. I wanted to use my light while running macOS.

## Description
This is a header-only library consisting of a single file, lamp.h. It defines a number of functions that handle structuring writes to an attached BusyLight. I'm using signal11's excellent [hidapi](https://github.com/signal11/hidapi) under the covers to abstract away the platform-specific USB HID device interaction differences.

Note that this is just a library for interfacing with the light; this library doesn't integrate with Skype for Business, or provide any UI whatsoever for actually setting the lamp color.
