# InnoMetrics

The tool for collecting metric on Linux systems with X-11 server

# Requirements

Install the following packages (libraries with development headers) before building:

 - `sqlite3`
 - `curl`
 - X11 with extensions: `libxi` and  `libxmu`
 - GTK 3
 - `appindicator3-0.1`

Then proceed with CMake as usual:

```bash
$ mkdir build
$ cd build
$ cmake ../
$ make
$ make install
```
