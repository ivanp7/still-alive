# Remake of the Portal game end credits ('Still Alive' song)

## Dependencies:

* [station-app](https://github.com/ivanp7/station)
* SDL2

## Build dependencies:

* gcc-compatible compiler (like clang)
* objcopy
* pkg-config
* ninja
* make

## How to build:

1. Download and build the [station-app](https://github.com/ivanp7/station)
    * configure with recommended flags: `./configure -aoACM`
    * build: `ninja -v`
    * `build/station-app` will be built

2. Build this project
    * you may need to edit value of `STATION_PATH` in `Makefile`
    * build: `make`
    * `still-alive.station` will be built

3. (Optional) Copy the .station file to the station-app build directory

## How to run:

```
$ ./station-app ./still-alive.station
```

## Sample screenshot:

![Sample screenshot](screenshot.png)

## Video:

![Video](video.mp4)

