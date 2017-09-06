# Temperature monitor for SNA-LGTC

This is a small containerized application for SNA-LGTC. It reads a SHT21 sensor
once per minute and pushes the temperature and relative humidity measurements
to the Videk management system.

It should be possible to build and run this container on both a PC and the
SNA-LGTC host board from the same Dockerfile. When running on a PC, the VESNA
SNC board should be connected over the Olimex ARM-USB-OCD programmer (both JTAG
and the serial line). When running on the SNA-LGTC host board there should be
a VESNA SNC guest board attached.

In both cases VESNA SNC board must be equipped with a SNE-SENS board with a
SHT21 sensor (there is also a `fake-sensor` branch available that works without
the SNE-SENS board).

This application has two components:

 * Firmware for SNC uses the *vesna-drivers* library to actually talk with the
   SHT21 sensor over the I2C bus. It exposes the sensor measurements on the
   serial line using the ALH protocol. The firmware is built at the same time
   as the container image and uploaded to the SNC board when the container is
   started.

 * A Python script running on the Linux system on the SNA-LGTC board. This
   script polls the sensor over the serial line and pushes the measurements to
   the Videk system. It registers the SHT21 sensor to Videk as necessary. The
   cluster and sensor node should already be registered as part of the SNA-LGTC
   boot process.

See also:

 * [VESNA firmware library](https://github.com/avian2/vesna-drivers/tree/logatec-3) (logatec-3 branch).
 * [Videk client](https://github.com/sensorlab/sna-lgtc-support/tree/master/videk-client) Python library.
 * [Videk](https://videk.ijs.si) sensor management system.
 * [VESNA ALH tools](https://github.com/avian2/vesna-alh-tools) Python library.

## Building the container

To build the container image, run the following command on the SNA-LGTC board.

Note: building the container requires cloning of the private `vesna-drivers`
GitHub repository. Because of this, you need to pass a GitHub access token to
the build process (the `ghtoken=` parameter below). You can generate a token at
your [GitHub settings page](https://github.com/settings/tokens). Select the
*Full control of private repositories* permission.

**Do not push the resulting container to Docker Hub or any other public
repository. It's best to revoke the access token immediately after building the
container.**

    $ docker build --build-arg=ghtoken=... -t logatec-temp-monitor .

To run the container in background:

    $ docker run -d --privileged -v /etc/videk/api.key:/etc/videk/api.key -v /etc/hostname:/etc/videk/hostname logatec-temp-monitor

It is possible to leave out the Videk configurations (`-v` options). In that
case, measurements will not be pushed to Videk.
