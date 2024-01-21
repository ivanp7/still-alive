# Remake of the Portal game credits ('Still Alive' song)

## Screenshot:

![Screenshot](screenshot.png)

## Full video:

![Video](https://raw.githubusercontent.com/ivanp7/still-alive/master/video.webm)

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

