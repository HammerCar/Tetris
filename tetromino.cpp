#include "tetromino.hh"

#include "engine.hh"

const Point rotation_matrix_cw[] =  { { 0, -1 }, {  1, 0 } };
const Point rotation_matrix_ccw[] = { { 0,  1 }, { -1, 0 } };


Tetromino::Tetromino(Engine* engine, const std::vector<Offset>& offsets)
	: engine_(engine), offsets_(offsets) {}

Tetromino::~Tetromino()
{

}

bool Tetromino::move(const Point& direction)
{
	move_without_checking(direction);

	if (!is_valid_position())
	{
		// Undo movement and return false
		move_without_checking(-direction);
		return false;
	}

	// Block has been moved successfully
	return true;
}

bool Tetromino::rotate(const bool& clockwise, const bool& shouldOffset)
{
	int old_rotation = rotation_;
	rotation_ += clockwise ? 1 : -1;
	rotation_ = mod(rotation_, 4);

	for (Block* block : blocks_)
	{
		Point p = blocks_.at(0)->position;
		rotate_block(block, p, clockwise);
	}

	if (!shouldOffset)
		return true;

	if (!find_offset(old_rotation))
	{
		rotate(!clockwise, false);
		return false;
	}

	return true;
}

Tetromino& Tetromino::operator=(const Tetromino& other)
{
	this->blocks_ = other.blocks_;
	this->engine_ = other.engine_;
	this->rotation_ = other.rotation_;
	this->offsets_ = other.offsets_;

	return *this;
}

void Tetromino::move_without_checking(const Point& direction)
{
	for (Block* block : blocks_)
	{
		block->position += direction;
	}
}

void Tetromino::rotate_block(Block* block, const Point& origin, const bool& clockwise)
{
	Point relative = block->position - origin;
	const Point* rotation_matrix = clockwise ? (const Point*)rotation_matrix_cw
											 : (const Point*)rotation_matrix_ccw;

	block->position = {
		(rotation_matrix[0].x * relative.x) + (rotation_matrix[1].x * relative.y),
		(rotation_matrix[0].y * relative.x) + (rotation_matrix[1].y * relative.y),
	};

	block->position += origin;
}

bool Tetromino::find_offset(const int& old_index)
{
	for (Offset offset : offsets_)
	{
		Point end_offset = offset[old_index] - offset[rotation_];

		if (move(end_offset))
			return true;
	}

	return false;
}

bool Tetromino::is_valid_position()
{
	for (Block* tetromino_block : blocks_)
	{
		Point position = tetromino_block->position;

		if (position.y < 0 ||
			position.x < 0 || position.x >= engine_->width_)
		{
			// Block is either going under the screen
			// or to the left or right of the screen
			return false;
		}

		for (Block* block : engine_->blocks_)
		{
			if (tetromino_block == block) continue;

			if (position == block->position)
			{
				// Block is inside another block
				return false;
			}
		}
	}

	return true;
}

int Tetromino::mod(const int& x, const int& m)
{
	return (x % m + m) % m;
}
