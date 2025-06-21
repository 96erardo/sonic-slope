#ifndef CONSTANTS_H
#define CONSTANTS_H

const int WINDOW_WIDTH = 480;

const int WINDOW_HEIGHT = 320;

const int GRID_SIZE = 32;

const int BITWISE_OFFSET = pow(2, ((sizeof(int) * 8) / 2)) / 2;

namespace Direction {
  enum Direction {
    top,
    bottom,
    left,
    right
  };
}

#endif