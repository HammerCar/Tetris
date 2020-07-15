#ifndef TETROMINO_ROTATION_OFFSETS_H
#define TETROMINO_ROTATION_OFFSETS_H

#include <vector>
#include "point.hh"

typedef  std::vector<Point> Offset;

const std::vector<Offset> JLSTZ_OFFSETS  {
	{ { 0, 0 }, { 0,  0 }, { 0, 0 }, {  0,  0 } },
	{ { 0, 0 }, { 1,  0 }, { 0, 0 }, { -1,  0 } },
	{ { 0, 0 }, { 1, -1 }, { 0, 0 }, { -1, -1 } },
	{ { 0, 0 }, { 0,  2 }, { 0, 0 }, {  0,  2 } },
	{ { 0, 0 }, { 1,  2 }, { 0, 0 }, { -1,  2 } },
};

const std::vector<Offset> I_OFFSETS  {
	{ {  0, 0 }, { -1,  0 }, { -1, 1 }, { 0,  1 } },
	{ { -1, 0 }, {  0,  0 }, {  1, 1 }, { 0,  1 } },
	{ {  2, 0 }, {  0,  0 }, { -2, 1 }, { 0,  1 } },
	{ { -1, 0 }, {  0,  1 }, {  1, 0 }, { 0, -1 } },
	{ {  2, 0 }, {  0, -2 }, { -2, 0 }, { 0,  2 } },
};

const std::vector<Offset> O_OFFSETS  {
	{ { 0, 0 }, { 0, -1 }, { -1, -1 }, { -1, 0 } },
};

#endif // TETROMINO_ROTATION_OFFSETS_H
