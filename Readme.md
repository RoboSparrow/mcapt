# mCapt

(WIP) Very simple and useless app for recording and visualising mouse events (Linux)

The application uses the `/dev/input/mice` input stream.

 - advantage: All mouse devices are recording into this stream, no mouse detection neccessary
 - disadvantage: Legacy format with limited data (no mousewheel by default)

Future plans:

 - replay mouse data
 - absolute position capture: (only if we can keep dependencies minimal)


## Docs

Mousedev event handler API

 - https://www.kernel.org/doc/html/v5.5/input/input.html#mousedev

PS/2 packet format(s)

 - https://www.eecg.utoronto.ca/~jayar/ece241_08F/AudioVideoCores/ps2/ps2.html
 - https://wiki.osdev.org/Mouse_Input

## Usage

### Record data

```
Usage: (sudo) mcapt [OPTION]... [DATALOG]
Record mouse movement into a comma-separated data file (DATALOG).
       -v, --verbose         enable verbose output
       -h, --help        show this help :)
```

Mouse movements are stored as comma-separated data. See `data/dlog.csv` if DATALOG path was not specified

* data/dlog.csv

```csv
; time=1711060390 device=/dev/input/mice
1220632,-11,3,0,0,0
1228569,-9,2,0,0,0
1236311,-5,2,0,0,0
; time=1711060394
```

data format:

```
<elapsed microseconds>,<relative x>,<relative y>,<left button>,<right button>,<middle button>
```

### Visualize data:

These logs can be exported into an SVG

```bash
./vis ./data/dlog.csv
# * writing to './data/dlog.svg'

```
