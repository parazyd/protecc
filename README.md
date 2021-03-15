# protecc

protecc is a Linux kernel module that will shut down your computer when
a predefined USB device is removed from the system.

It is based on [silk-guardian](https://github.com/NateBrune/silk-guardian)
which itself is based on [usbkill](https://github.com/hephaest0s/usbkill).
However, it operates in an inverse way to its predecessors: instead of
panicking based on any USB activity, protecc is designed to only watch
for the removal of one defined USB key before going into panic mode.

## Installation
First edit `config.h` with your preferred setings. Most importantly,
make sure you set your chosen USB's device ID correctly. Then compile
and install the kernel module:

```shell
make
insmod protecc.ko
```

## Why?

https://web.archive.org/web/20180613183514/https://www.ccn.com/fbi-illegally-stole-ross-ulbrichts-laptop-brought-silk-road/


## Usage

Using protecc properly requires some physical availability to remove
the USB key even when distracted, or the device is being forcefully
removed from you.

In theory, it is possible to have a USB device tied around your wrist
with some wire or string and then forcefully shut down the computer by
moving your arm - making the USB device unplug.


## Feature List

- Shutdown the computer when chosen USB key is removed.
- Remove files before shutdown.
- Secure deletion of chosen files before shutdown.
- No dependencies.


## Future extension ideas (exclusive and/or optional)

- Verification
  - Perhaps some change of USB identify over time
  - Status bits in USB can be changed for steganography
  - All states in USB can be used to make sure the device is the true device
- Timetracking (allow only certain time of day when the device is allowed
  to be attached, for a certain amount of time)
- The device should be disconnected for a certain amount per day
