#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <random>

#include "point.hh"
#include "tetromino.hh"


class Engine
{
public:
	Engine() {}
	Engine(const int& width, const int& height);
	~Engine();

	enum class Game_state { PLAYING, NEW_BLOCK, LINE_REMOVED, GAMEOVER };

	int width_, height_;
	std::vector<Block*> blocks_;

	// Frame time in milliseconds
	int game_speed = 1000;


	/**
	 * Starts a new game
	 */
	void start();

	/**
	 * Advances the game by one tick
	 * Tick speed is defined by the game_speed variable
	 */
	Game_state tick();

	/**
	 * Moves the current tetromino either to the right or left
	 * @param direction should be 1 or -1
	 */
	void move_horizontal(const int& direction);

	/**
	 * Drops the current tetromino to the bottom instantly
	 */
	void drop();

	/**
	 * Rotates the current tetromino in the given direction
	 * @param clockwise which direction the tetromino should rotate
	 * @return true if the block was rotated, otherwise false
	 */
	bool rotate(const bool& clockwise);

	/**
	 * Puts the current tetromino to hold and spawns the hold piece
	 * if there is one
	 */
	void hold();

	/**
	 * Clears the game field
	 */
	void clear();

	/**
	 * Returns the 4 blocks of the current drop position
	 */
	Block* get_drop_position();

	/**
	 * Returns the blocks of the next tetromino
	 */
	std::vector<Block*> get_next_tetromino();

	/**
	 * Returns the blocks of the hold tetromino
	 */
	std::vector<Block*> get_hold_tetromino();

	inline int get_score() { return score; }
	inline int get_tetrises() { return tetrises; }
	inline int get_level() { return level; }

private:
	// Constants for different tetrominos and the number of them
	enum Tetromino_kind {HORIZONTAL,
						 LEFT_CORNER,
						 RIGHT_CORNER,
						 SQUARE,
						 STEP_UP_RIGHT,
						 PYRAMID,
						 STEP_UP_LEFT,
						 NUMBER_OF_TETROMINOS};

	Tetromino* current_tetromino_;
	Tetromino_kind next_tetromino_;

	Tetromino* hold_tetromino_;
	bool can_hold;

	// For randomly selecting the next dropping tetromino
	std::default_random_engine randomEng_;
	std::uniform_int_distribution<int> distr_;

	int score;
	int tetrises;
	int lines_removed;
	int level;


	/**
	 * Spawns the next tetromino
	 */
	void set_new_tetromino();

	/**
	 * Creates and returns a random tetromino
	 * @return the new tetromino
	 */
	Tetromino* random_tetromino();
	/**
	 * Creates a specific tetromino
	 * @param kind - which tetromino to spawn
	 * @param add_to_blocks - should the blocks be added to the blocks vector
	 * @return the new tetromino
	 */
	Tetromino* new_tetromino(const Tetromino_kind& kind, const bool& add_to_blocks = true);
	/**
	 * Creates the tetromino based on the given points
	 * @param color - color of the blocks
	 * @param points_to_add - relative positions of the tetromino blocks
	 * @param offsets - rotation offsets
	 * @param add_to_blocks - should the blocks be added to the blocks vector
	 * @return the new tetromino
	 */
	Tetromino* create_tetromino(const QColor& color, const std::vector<Point>& points_to_add, const std::vector<Offset>& offsets, const bool& add_to_blocks);

	/**
	 * Clear full lines if there are any
	 * @return number of cleared lines
	 */
	int remove_full_lines();

	/**
	 * Checks if the game is over
	 * @return is the game over
	 */
	bool check_gameover();
};

#endif // ENGINE_H
