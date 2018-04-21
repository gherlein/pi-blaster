# pi-blaster-mqtt

This Raspberry Pi (RPi) service subscribes to an MQTT broker running on the RPi and processes commands to set GPIO pins to PWM values.  It's based on the excellent pi-blaster software that uses a local device file approach for control.  Like pi-blaster, this software provides PWM control with low CPU impact on the RPi.

## How to install

### Install Prerequisites

You need to have an MQTT broker installed on the RPi:

```
sudo apt install -y mosquitto
```

### Build and install directly from source

The build environment is based on Autotools to allow for compilation on the
Raspberry Pi, or cross-compilation.

You may need to install `autoconf` to build. On Raspbian and other Debian based
systems this can be achieved with:

    sudo apt-get install autoconf

You will need to install the MQTT libraries:

    sudo apt-get install -y libmosquitto-dev libmosquitto1-dbg mosquitto-clients mosquitto-dev mosquitto

Building is extremely simple:

    ./autogen.sh
    ./configure
    make

To start pi-blaster-mqtt and have it relaunched automatically on every reboot:

    sudo make install

Or to start pi-blaster manually run:

    sudo ./pi-blaster-mqtt

And to uninstall, simply run:

    sudo make uninstall

This will stop pi-blaster-mqtt and prevent it from starting automatically on the next
reboot.

### Build your own deb package and install with dpkg

The Debian package relies on systemd which means you must have Raspbian 8 or
later (aka "Jessie"). Run `cat /etc/debian_version`) to check what version
you are currently running.

Install the debian tools required to compile and prepare the package:

    sudo apt-get install debhelper dh-autoreconf dh-systemd dpkg-dev \
      init-system-helpers autoconf

And build the package:

    dpkg-buildpackage -us -uc -i && sudo dpkg -i ../pi-blaster*.deb


### Install with docker

If you have docker on your RPi, you can run this image

```bash
docker run -it --privileged --rm -v /dev:/dev sarfata/pi-blaster-mqtt
```

Or build from source in git repo

```bash
docker build -t pib .

docker run -it --privileged --rm -v /dev:/dev pib
```

## How to use

### GPIO Pin Numbers

You must use the GPIO number (BCM xx in the diagram below).

[![List of pins thanks to pinout.xyz](pinout.xyz.png)](http://pinout.xyz)

### Controlling One Pin at a Time

From the RPi or from a host on the same network, you can use the Mosquitto CLI to test:

```
mosquitto_pub -h <host> -t pi-blaster-mqtt/text -m "<pin>=<value>"
```
For example, from the RPi, to set pin 4 to a typically neutral value (150ms, or 0.15 seconds) use this command:

```
mosquitto_pub -h localhost -t pi-blaster-mqtt/text -m "4=0.15"
```

Likewise to set pin 4 to a typically max value (200ms, or 0.20 seconds) use this command:

```
mosquitto_pub -h localhost -t pi-blaster-mqtt/text -m "4=0.20"
```

### Controlling Multiple Pins at a Time

From the RPi or from a host on the same network, you can use the Mosquitto CLI to test:

```
mosquitto_pub -h <host> -t pi-blaster-mqtt/line -m "<pin>=<value>|<pin>=<value>|<pin>=<value>"
```
For example, from the RPi, to set pin 4 to a typically neutral value (150ms, or 0.15 seconds) use this command:

```
mosquitto_pub -h localhost -t pi-blaster-mqtt/line -m "4=0.15|17=0.15|27=0.15"
```

## How to adjust the frequency and the resolution of the PWM

On startup, pi-blaster-mqtt gives you the frequency of the PWM, the number of steps
that you can control, the maximum and the minimum period of a pulse.

    sudo ./pi-blaster-mqtt
    Using hardware:               PWM
    Number of channels:           8
    PWM frequency:                100 Hz
    PWM steps:                    1000
    Maximum period (100  %):      10000us
    Minimum period (0.100%):      10us

You can adjust those by changing a few defines at the top of the source code:

 * `NUM_SAMPLES`: The number of steps
 * `SAMPLE_US`: The time of one step (minimum period)

If you do not need a resolution of 1000 steps (approximately equivalent to a 10
bit DAC), then you can reduce the number of samples or increase the duration of
the steps.

Richard Hirst who wrote the original code recommended not going below 2us for
`SAMPLE_US`.

## Options

To override the default list of supported GPIO pins and specify fewer (or more)
you can specify a comma separated list of GPIO numbers. This is also the default
list:

    --gpio 4,17,18,27,21,22,23,24,25

To use the BCM2835's PCM peripheral instead of its PWM peripheral to time the
DMA transfers, pass the option:

    --pcm

This is useful if you are already using the chip's PWM peripheral, for example
for audio output.

To invert the pulse (off = pin HIGH, pulse = pin LOW), use:

    --invert

This can be useful for common anode LEDs or other devices that expect an
active-low signal.

To keep pi-blaster-mqtt running in the foreground without running as a daemon use:

    -D

To view help or version information, use:

    --help

    --version

## TODO

By default this software connects to the Mosquitto broker running on the RPi. I plan to support running that broker on another host and allow a command line parameter (see above) to specify that hostname/IP address.  This is not yet a high priority.


## Working with Git

If you want to totally clean up the folder to get ready to do a 'git push" here's the easiest way to do it:

```
make distclean
git clean -xf
rm -Rf autom4te.cache/
```


## Warnings and other caveats

**Pins being used by pi-blaster will be configured as outputs. Do not plug
something on an input or you might destroy it!**

This daemon uses the hardware PWM generator of the raspberry pi to get precise
timings. This might interfere with your sound card output.  There is
experimental support for a PCM time-source. If you are interested, I suggest you
look at Richard Hirst original project (ServoBlaster) and try the `--pcm`
option.

## Contributors to pi-blaster-mqtt

* Greg Herlein (https://github.com/gherlein)

## Contributors to pi-blaster

* Richard Hirst for [ServoBlaster](https://github.com/richardghirst/PiBits)
* Thomas Sarlandie (https://github.com/sarfata)
* Michael Vitousek (https://github.com/mvitousek/pi-blaster)
* Pete Nelson (https://github.com/petiepooo)
* Edgar Siva (https://github.com/edgarsilva)
* Alex Lennon (https://github.com/ajlennon)
* Lara Maia (https://github.com/LaraCraft304)
* Pattrick Hüper (https://github.com/phueper)
* Boyuan Yang (https://github.com/hosiet)
* penfold42 (https://github.com/penfold42)
* Thomas McVay (https://github.com/ThomasMcVay)
* Pavle Petrovic (https://github.com/pavlecc)

## Want to support this project?

The best way to contribute is to write code for the features you would like to
see and to make a pull-requests.

## History 

Based on the most excellent [pi-blaster](https://github.com/sarfata/pi-blaster) by Thomas Sarlandie.

Pi-blaster project is based on the excellent work of Richard Hirst for
[ServoBlaster](https://github.com/richardghirst/PiBits).

## License

Released under The MIT License.

Note from Thomas Sarlandie: This project was initially released by Richard Hist under the GPL v3
License. Richard gave me explicit permission to distribute this derivative work
under the MIT License.

    Copyright (c) 2018 Greg Herlein - Thomas Sarlandie - Richard Hirst

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.


