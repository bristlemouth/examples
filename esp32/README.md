# ESP32 Bristlemouth Port

The following is a jumping on point for an ESP32 Bristlemouth App.

## Project Setup

In order to setup the build tools run the following commands:

```
conda create -n bristlemouth_esp32
conda activate bristlemouth_esp32
conda env update -f environment.yml
make init
```

This will setup all of the build tools necessary to compile the project.

## Building The Project

In order to build the project,
the conda environment must be activated with:

```
conda activate bristlemouth_esp32
```

Once the conda environment has been activated,
run the following command:

```
make build {env}
```

With `{env}` being one of the following:

- `prod`
    - for production level builds
- `dev`
    - for development level builds

## Flashing The Project

In order to flash the project,
the conda environment must be activated.
Once activated run the following command:

```
make flash {env} {path/to/device}
```

With `{env}` being the environment to flash,
and `{path/to/device}` being the path to the device.
For example,
on linux this may be `/dev/ttyUSB0`.
