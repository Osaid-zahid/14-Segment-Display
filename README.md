# 14-Segment Display
I made this project during my first semester. The constraints included no libraries except iostream and the prohibition of functions.
Renders arbitrary text as large ASCII-art characters using a 14-segment display encoding.

## Supported characters
Uppercase A–Z, digits 0–9, and the symbols `/ \ , - _` (space also works).

## Build & run
```bash
g++ -o display 14-Segment-Display.cpp
./display
```

## How it works
Each character is mapped to a 15-bit bitmask where each bit controls one segment
(a through n, plus G1/g2 for the center diagonals).  
The renderer iterates over every scan-line of the output height and, for each character,
decides which cells to fill with `#` based on which segment bits are set.

## Height
Fixed at 20 rows per character in the sample outputs.  
Change `int n = 20;` in `main()` to any multiple of 10 for other sizes.
