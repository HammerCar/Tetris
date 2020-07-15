#ifndef TETROMINO_H
#define TETROMINO_H

#include <QColor>

#include "point.hh"
#include "tetromino_rotation_offsets.hh"

class Engine;

struct Block
{
	Point position;
	QColor color;
};

class Tetromino
{
public:
	Tetromino(Engine* engine = nullptr, const std::vector<Offset>& offsets_ = {});
	Tetromino(const Tetromino& other);
	~Tetromino();

	std::vector<Block*> blocks_;

	bool move(const Point& direction);
	bool rotate(const bool& clockwise, const bool& shouldOffset = true);

	Tetromino& operator=(const Tetromino& other);

private:
	Engine* engine_;

	int rotation_ = 0;
	std::vector<Offset> offsets_;


	void move_without_checking(const Point& direction);

	void rotate_block(Block* block, const Point& origin, const bool& clockwise);
	bool find_offset(const int& old_index);

	bool is_valid_position();

	// Modulo operation that works for positive and negative values
	int mod(const int& x, const int& m);
};

#endif // TETROMINO_H
