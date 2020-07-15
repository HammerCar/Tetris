#include "engine.hh"

#include <ctime>

#include "tetromino_rotation_offsets.hh"

const int points_for_lines[] = { 0, 40, 100, 300, 1200 };
const int lines_per_level = 6;


Engine::Engine(const int& width, const int& height)
	: width_(width), height_(height)
{
	// Setting random engine ready for the first real call.
	int seed = time(nullptr); // You can change seed value for testing purposes
	randomEng_.seed(seed);
	distr_ = std::uniform_int_distribution<int>(0, NUMBER_OF_TETROMINOS - 1);
	distr_(randomEng_); // Wiping out the first random number (which is almost always 0)

	current_tetromino_ = nullptr;
	hold_tetromino_ = nullptr;
}

Engine::~Engine()
{
	clear();
}

void Engine::start()
{
	clear();

	score = 0;
	tetrises = 0;
	lines_removed = 0;
	level = 0;

	next_tetromino_ = NUMBER_OF_TETROMINOS;
	hold_tetromino_ = nullptr;
	can_hold = true;

	set_new_tetromino();
}

Engine::Game_state Engine::tick()
{
	if (!current_tetromino_->move(DOWN))
	{
		int removed = remove_full_lines();
		lines_removed += removed;
		if (removed == 4)
		{
			tetrises++;
		}

		if (lines_removed / lines_per_level > level)
		{
			level++;
			game_speed -= game_speed / 10;
		}

		score += points_for_lines[removed] * (level + 1);

		if (check_gameover())
		{
			return Game_state::GAMEOVER;
		}

		set_new_tetromino();

		if (removed)
		{
			return Game_state::LINE_REMOVED;
		}

		return Game_state::NEW_BLOCK;
	}

	return Game_state::PLAYING;
}

void Engine::move_horizontal(const int& direction)
{
	current_tetromino_->move({ direction, 0 });
}

void Engine::drop()
{
	while (current_tetromino_->move(DOWN)) {}
}

bool Engine::rotate(const bool& clockwise)
{
	return current_tetromino_->rotate(clockwise);
}

void Engine::hold()
{
	if (!can_hold)
	{
		return;
	}

	can_hold = false;

	for (Block* block : current_tetromino_->blocks_)
	{
		blocks_.erase(std::find(blocks_.begin(), blocks_.end(), block));
	}

	Tetromino* temp = current_tetromino_;
	current_tetromino_ = hold_tetromino_;
	hold_tetromino_ = temp;

	if (current_tetromino_)
	{
		Point start = { width_ / 2, height_ + 1 };
		Point move = start - current_tetromino_->blocks_.at(0)->position;

		for (Block* block : current_tetromino_->blocks_)
		{
			block->position += move;
			blocks_.push_back(block);
		}
	}
	else
	{
		set_new_tetromino();
	}
}

Block* Engine::get_drop_position()
{
	int count = 0;
	while (current_tetromino_->move(DOWN))
	{
		count++;
	}

	Block* drop_blocks = new Block[4];
	for (unsigned int i = 0; i < 4; i++)
	{
		drop_blocks[i].position = current_tetromino_->blocks_.at(i)->position;
		drop_blocks[i].color = QColor(255, 255, 255);
	}

	for (int i = 0; i < count; i++)
	{
		current_tetromino_->move(UP);
	}

	return drop_blocks;
}

std::vector<Block*> Engine::get_next_tetromino()
{
	Tetromino* next = new_tetromino(next_tetromino_, false);
	std::vector<Block*> blocks = next->blocks_;

	delete next;
	return blocks;
}

std::vector<Block*> Engine::get_hold_tetromino()
{
	if (!hold_tetromino_)
	{
		return {};
	}

	std::vector<Block*> blocks = hold_tetromino_->blocks_;
	return blocks;
}

void Engine::clear()
{
	if (current_tetromino_)
	{
		delete current_tetromino_;
		current_tetromino_ = nullptr;
	}

	if (hold_tetromino_)
	{
		for (Block* block : hold_tetromino_->blocks_)
		{
			delete block;
		}

		delete hold_tetromino_;
	}

	for (Block* block : blocks_)
	{
		delete block;
	}

	blocks_.clear();
}

void Engine::set_new_tetromino()
{
	can_hold = true;

	Tetromino_kind kind = next_tetromino_ == NUMBER_OF_TETROMINOS ? (Tetromino_kind)distr_(randomEng_)
																  : next_tetromino_;
	delete current_tetromino_;
	current_tetromino_ = new_tetromino(kind);

	next_tetromino_ = (Tetromino_kind)distr_(randomEng_);
}

Tetromino* Engine::random_tetromino()
{
	return new_tetromino((Tetromino_kind)distr_(randomEng_), true);
}

Tetromino* Engine::new_tetromino(const Tetromino_kind& kind, const bool& add_to_blocks)
{
	switch (kind)
	{
	case HORIZONTAL:
		return create_tetromino(QColor(0, 255, 255), { { 0, 0 }, { -1, 0 }, { 1, 0 }, { 2, 0 } }, I_OFFSETS, add_to_blocks);

	case LEFT_CORNER:
		return create_tetromino(QColor(0, 0, 255), { { 0, 0 }, { -1, 1 }, { -1, 0 }, { 1, 0 } }, JLSTZ_OFFSETS, add_to_blocks);

	case RIGHT_CORNER:
		return create_tetromino(QColor(255, 127, 0), { { 0, 0 }, { -1, 0 }, { 1, 0 }, { 1, 1 } }, JLSTZ_OFFSETS, add_to_blocks);

	case SQUARE:
		return create_tetromino(QColor(255, 127, 0), { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } }, O_OFFSETS, add_to_blocks);

	case STEP_UP_RIGHT:
		return create_tetromino(QColor(0, 255, 0), { { 0, 0 }, { -1, 0 }, { 0, 1 }, { 1, 1 } }, JLSTZ_OFFSETS, add_to_blocks);

	case PYRAMID:
		return create_tetromino(QColor(126, 63, 190), { { 0, 0 }, { -1, 0 }, { 1, 0 }, { 0, 1 } }, JLSTZ_OFFSETS, add_to_blocks);

	case STEP_UP_LEFT:
		return create_tetromino(QColor(255, 0, 0), { { 0, 0 }, { -1, 1 }, { 0, 1 }, { 1, 0 } }, JLSTZ_OFFSETS, add_to_blocks);

	default:
		return nullptr;
	}
}

Tetromino* Engine::create_tetromino(const QColor& color, const std::vector<Point>& points_to_add, const std::vector<Offset>& offsets, const bool& add_to_blocks)
{
	int start_x = width_ / 2;
	int start_y = height_ + 1;

	Tetromino* tetromino = new Tetromino(this, offsets);

	for (Point point : points_to_add)
	{
		Block* block = new Block({ { start_x + point.x, start_y + point.y }, color });

		tetromino->blocks_.push_back(block);

		if (add_to_blocks)
		{
			blocks_.push_back(block);
		}
	}

	return tetromino;
}

int Engine::remove_full_lines()
{
	// Create and initialize grid
	std::vector<std::vector<Block*>> grid_lines;
	for (int i = 0; i < height_ + 4; i++)
	{
		grid_lines.push_back({});
	}

	// Add all blocks to their grid line
	for (Block* block : blocks_)
	{
		grid_lines.at(block->position.y).push_back(block);
	}

	int removed_count = 0;

	// Remove full lines
	for (unsigned int i = 0; i < grid_lines.size(); i++)
	{
		if (grid_lines.at(i).size() == (unsigned int)width_)
		{
			removed_count++;

			// Delete blocks
			for (Block* block : grid_lines.at(i))
			{
				blocks_.erase(std::find(blocks_.begin(), blocks_.end(), block));
				delete block;
			}

			// Move lines above this line down
			for (unsigned int j = i + 1; j < grid_lines.size(); j++)
			{
				for (Block* block : grid_lines.at(j))
					block->position.y--;
			}
		}
	}

	return removed_count;
}

bool Engine::check_gameover()
{
	for (Block* block : current_tetromino_->blocks_)
	{
		if (block->position.y > height_)
		{
			return true;
		}
	}

	return false;
}


















