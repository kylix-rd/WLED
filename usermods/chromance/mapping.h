/*
 * Maps hex topology onto LED's
 * (C) Voidstar Lab LLC 2021
 */

#ifndef MAPPING_H_
#define MAPPING_H_

#define headof(S) (S * 14)
#define tailof(S) (headof(S) + 13)

// Beam 0 is at 12:00 and advance clockwise
// -1 means nothing connected on that side
int nodeConnections[25][6] = {
  {-1, 24, 29, 23, -1, -1}, // {-1, -1, 1, -1, 0, -1},
  {-1, -1, 25, -1, 24, -1}, // {-1, -1, 3, -1, 2, -1},
  {-1, -1,  5, -1,  4, -1}, // {-1, -1, 5, -1, 4, -1}, 
  {-1, -1, 34, -1, 33, -1}, // {-1, 0, 6, 12, -1, -1},
  {-1, -1, -1, 35, 38, 34}, // {-1, 2, 8, 14, 7, 1},
  
  {-1, 28, -1, 27, -1, 29}, // {-1, 4, 10, 16, 9, 3},
  {-1,  4,  9,  3, 28, 25}, // {-1, -1, -1, 18, 11, 5},
  {-1,  8, -1,  7, -1,  9}, // {-1, 7, -1, 13, -1, 6},
  {-1, 33, 39, 32,  8,  5}, // {-1, 9, -1, 15, -1, 8},
  {-1, 38, -1, 37, 39, -1}, // {-1, 11, -1, 17, -1, 10},
  
  {23, -1, 22, -1, -1, -1}, // {12, -1, 19, -1, -1, -1},
  {27, 26, 21, 14, -1, 22}, // {14, -1, 21, -1, 20, -1},
  { 3, -1,  2, -1, 26, -1}, // {16, -1, 23, -1, 22, -1},
  { 7,  6,  1, 17, 20,  2}, // {18, -1, -1, -1, 24, -1},
  {32, -1, 31, -1,  6, -1}, // {13, 20, 25, 29, -1, -1}, 
  
  {37, 36, -1, 30,  0, 31}, // {15, 22, 27, 31, 26, 21},
  {35, -1, -1, -1, 36, -1}, // {17, 24, -1, 33, 28, 23},
  {-1, 20, -1, 14, -1, 21}, // {-1, 26, -1, 30, -1, 25},
  {-1,  0, -1, 18, -1,  1}, // {-1, 28, -1, 32, -1, 27},
  {14, -1, 13, -1, -1, -1}, // {29, -1, 34, -1, -1, -1},
  
  {15, 16, 12, -1, -1, 13}, // {31, -1, 36, -1, 35, -1},
  {17, -1, 19, -1, 16, -1}, // {33, -1, -1, -1, 37, -1},
  {18, 10, -1, -1,  4, 19}, // {30, 35, 38, -1, -1, 34},
  {30, -1, -1, -1, 10, -1}, // {32, 37, -1, -1, 39, 36},
  {-1, 11, -1, -1, -1, 12}  // {-1, 39, -1, -1, -1, 38}
};

// First member: Node closer to ceiling
// Second: Node closer to floor
int segmentConnections[40][2] = {
  {15, 18}, // {0, 3},
  {13, 18}, // {0, 4},
  {12, 13}, // {1, 4},
  { 6, 12}, // {1, 5},
  { 2,  6}, // {2, 5},
  { 2,  8}, // {2, 6},
  {14, 13}, // {3, 7},
  { 7, 13}, // {4, 7},
  { 8,  7}, // {4, 8},
  { 6,  7}, // {5, 8},

  {23, 22}, // {5, 9},
  {22, 24}, // {6, 9},
  {20, 24}, // {3, 10},
  {20, 19}, // {7, 14},
  {11, 19}, // {4, 11},
  {17, 20}, // {8, 15},
  {21, 20}, // {5, 12},
  {13, 21}, // {9, 16},  
  {18, 22}, // {6, 13},
  {21, 22}, // {10, 14},

  {13, 17}, // {11, 14},
  {11, 17}, // {11, 15},
  {10, 11}, // {12, 15},
  { 0, 10}, // {12, 16},
  { 1,  0}, // {13, 16},
  { 1,  6}, // {14, 17},
  {12, 11}, // {15, 17},
  { 5, 11}, // {15, 18},
  { 6,  5}, // {16, 18},
  { 0,  5}, // {14, 19},

  {15, 23}, // {17, 22},
  {14, 15}, // {15, 20},
  { 8, 14}, // {18, 23},
  { 3,  8}, // {16, 21},
  { 3,  4}, // {19, 22},
  { 4, 16}, // {20, 22},
  {16, 15}, // {20, 23},
  { 9, 15}, // {21, 23},
  { 4,  9}, // {22, 24},
  { 8,  9}  // {23, 24}
};

// First member: Strip number
// Second: LED index closer to ceiling
// Third: LED index closer to floor
int ledAssignments[40][3] = {
  {0, headof(0), tailof(0)}, // {2, headof(3), tailof(3)},
  {0, tailof(1), headof(1)}, // {2, tailof(2), headof(2)},
  {0, tailof(2), headof(2)}, // {1, headof(10), tailof(10)},
  {0, tailof(3), headof(3)}, // {1, tailof(9), headof(9)},
  {0, tailof(4), headof(4)}, // {1, headof(4), tailof(4)},
  {0, headof(5), tailof(5)}, // {1, tailof(3), headof(3)},
  {0, headof(6), tailof(6)}, // {2, tailof(6), headof(6)},
  {0, tailof(7), headof(7)}, // {3, tailof(11), headof(11)},
  {0, tailof(8), headof(8)}, // {1, headof(11), tailof(11)},
  {0, tailof(9), headof(9)}, // {1, tailof(8), headof(8)},

  {1, headof(10), tailof(10)}, // {1, headof(12), tailof(12)},
  {1, headof(11), tailof(11)}, // {0, tailof(11), headof(11)},
  {1, tailof(12), headof(12)}, // {2, headof(4), tailof(4)},
  {1, tailof(13), headof(13)}, // {3, tailof(10), headof(10)},
  {1, tailof(14), headof(14)}, // {2, tailof(1), headof(1)},
  {1, headof(15), tailof(15)}, // {1, tailof(7), headof(7)},
  {1, tailof(16), headof(16)}, // {1, headof(5), tailof(5)},
  {1, tailof(17), headof(17)}, // {0, tailof(10), headof(10)},
  {1, headof(18), tailof(18)}, // {1, tailof(2), headof(2)},
  {1, tailof(19), headof(19)}, // {2, headof(5), tailof(5)},

  {2, headof(20), tailof(20)}, // {3, tailof(4), headof(4)},
  {2, tailof(21), headof(21)}, // {3, headof(5), tailof(5)},
  {2, tailof(22), headof(22)}, // {0, headof(5), tailof(5)},
  {2, tailof(23), headof(23)}, // {0, tailof(4), headof(4)},
  {2, tailof(24), headof(24)}, // {1, tailof(1), headof(1)},
  {2, tailof(25), headof(25)}, // {3, tailof(9), headof(9)},
  {2, headof(26), tailof(26)}, // {0, headof(6), tailof(6)},
  {2, headof(27), tailof(27)}, // {1, tailof(6), headof(6)},
  {2, tailof(28), headof(28)}, // {0, tailof(9), headof(9)},
  {2, tailof(29), headof(29)}, // {3, tailof(3), headof(3)},

  {3, tailof(30), headof(30)}, // {3, tailof(8), headof(8)},
  {3, tailof(31), headof(31)}, // {3, headof(6), tailof(6)},
  {3, tailof(32), headof(32)}, // {0, tailof(8), headof(8)},
  {3, tailof(33), headof(33)}, // {0, tailof(3), headof(3)},
  {3, tailof(34), headof(34)}, // {3, tailof(2), headof(2)},
  {3, headof(35), tailof(35)}, // {3, headof(7), tailof(7)},
  {3, headof(36), tailof(36)}, // {0, headof(7), tailof(7)},
  {3, tailof(37), headof(37)}, // {0, tailof(2), headof(2)},
  {3, tailof(38), headof(38)}, // {3, tailof(1), headof(1)},
  {3, tailof(39), headof(39)}  // {0, tailof(1), headof(1)}
};

// Border nodes are on the very edge of the network.
// Ripples fired here don't look very impressive.
int numberOfBorderNodes = 10;
int borderNodes[] = {0, 1, 2, 3, 4, 10, 16, 19, 23, 24}; // {0, 1, 2, 3, 6, 10, 13, 19, 21, 24};

// Cube nodes link three equiangular segments
// Firing ripples that always turn in one direction will draw a cube
int numberOfCubeNodes = 8;
int cubeNodes[] = {5, 7, 9, 12, 14, 17, 18}; // {7, 8, 9, 11, 12, 17, 18};

// Firing ripples that always turn in one direction will draw a starburst
int starburstNode = 13; // 15;

#endif
