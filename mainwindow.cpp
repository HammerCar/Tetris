#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // We need a graphics scene in which to draw rectangles.
    scene_ = new QGraphicsScene(this);
	scene_next_ = new QGraphicsScene(this);
	scene_hold_ = new QGraphicsScene(this);

    // The graphicsView object is placed on the window
    // at the following coordinates, but if you want
    // different placement, you can change their values.
    int left_margin = 100; // x coordinate
    int top_margin = 150; // y coordinate

    // The width of the graphicsView is BORDER_RIGHT added by 2,
    // since the borders take one pixel on each side
    // (1 on the left, and 1 on the right).
    // Similarly, the height of the graphicsView is BORDER_DOWN added by 2.
	ui->graphicsView->setGeometry(left_margin, top_margin,
                                  BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui->graphicsView->setScene(scene_);

	ui->nextTetrominoView->setScene(scene_next_);
	ui->holdTetrominoView->setScene(scene_hold_);

    // The width of the scene_ is BORDER_RIGHT decreased by 1 and
    // the height of it is BORDER_DOWN decreased by 1, because
    // each square of a tetromino is considered to be inside the sceneRect,
    // if its upper left corner is inside the sceneRect.
	scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);
	scene_next_->setSceneRect(0, 0, ui->nextTetrominoView->width() - 2, ui->nextTetrominoView->height() - 2);
	scene_hold_->setSceneRect(0, 0, ui->holdTetrominoView->width() - 2, ui->holdTetrominoView->height() - 2);


	ui->labelState->setText("");
	ui->labelScore->setText("");
	ui->labelTetrisCount->setText("");
	ui->labelLevel->setText("");

	ui->graphicsView->setBackgroundBrush(QBrush(QColor(220, 220, 220)));

	// Initialize button
	connect(ui->playButton, SIGNAL(clicked()), this, SLOT(play()));
	connect(ui->pauseButton, SIGNAL(clicked()), this, SLOT(pause()));
	connect(ui->closseButton, SIGNAL(clicked()), this, SLOT(close()));

	engine_ = Engine(COLUMNS, ROWS);

	timer_ = new QTimer();
	connect(timer_, SIGNAL(timeout()), this, SLOT(tick()));
	timer_->setInterval(engine_.game_speed);

	game_time_timer_ = new QTimer();
	connect(game_time_timer_, SIGNAL(timeout()), this, SLOT(time_tick()));
	game_time_timer_->setInterval(1000);
}

MainWindow::~MainWindow()
{
	delete ui;
	delete timer_;
}

void MainWindow::play()
{
	playing_ = true;
	paused_ = false;

	engine_.start();

	timer_->start();

	game_time_elapsed_ = 0;
	game_time_timer_->start();
	update_game_time();

	scene_hold_->clear();

	render();
	render_next();
}

void MainWindow::pause()
{
	if (playing_ == false)
	{
		return;
	}

	paused_ = !paused_;
	if (paused_)
	{
		timer_->stop();
		game_time_timer_->stop();

		ui->pauseButton->setText("Resume");
	}
	else
	{
		timer_->start();
		game_time_timer_->start();

		ui->pauseButton->setText("Pause");

		tick();
	}
}

void MainWindow::tick()
{
	Engine::Game_state state = engine_.tick();

	if (state == Engine::Game_state::GAMEOVER)
	{
		// Game over
		playing_ = false;
		timer_->stop();
		game_time_timer_->stop();

		update_game_time();
	}
	else if (state == Engine::Game_state::NEW_BLOCK)
	{
		render_next();
	}
	else if (state == Engine::Game_state::LINE_REMOVED)
	{
		render_next();
	}

	render();
}

void MainWindow::time_tick()
{
	game_time_elapsed_++;
	update_game_time();
}

void MainWindow::render()
{
	scene_->clear();

	ui->labelScore->setText("Score: " + QString::number(engine_.get_score()));
	ui->labelTetrisCount->setText("Tetris count: " + QString::number(engine_.get_tetrises()));
	ui->labelLevel->setText("Level: " + QString::number(engine_.get_level() + 1));

	Block* drop_blocks = engine_.get_drop_position();
	for (int i = 0; i < 4; i++)
	{
		render_block(&drop_blocks[i]);
	}

	for (Block* block : engine_.blocks_)
	{
		render_block(block);
	}
}

void MainWindow::render_block(Block* block)
{
	QRectF rect(block->position.x * SQUARE_SIDE,
				BORDER_DOWN - (block->position.y + 1) * SQUARE_SIDE,
				SQUARE_SIDE,
				SQUARE_SIDE);

	QPen pen(QColor(0, 0, 0));
	QBrush brush(block->color);

	scene_->addRect(rect, pen, brush);
}

void MainWindow::render_next()
{
	std::vector<Block*> blocks = engine_.get_next_tetromino();

	double average_x = 0;
	double average_y = 0;
	for (Block* block : blocks)
	{
		average_x += block->position.x;
		average_y += block->position.y;
	}
	average_x /= blocks.size();
	average_y /= blocks.size();

	scene_next_->clear();

	QPen pen(QColor(0, 0, 0));
	for (Block* block : blocks)
	{
		QRectF rect(NEXT_BORDER_RIGHT / 2 + (block->position.x - average_x - 0.5) * SQUARE_SIDE,
					NEXT_BORDER_DOWN / 2 - (block->position.y - average_y + 0.5) * SQUARE_SIDE,
					SQUARE_SIDE,
					SQUARE_SIDE);

		QBrush brush(block->color);

		scene_next_->addRect(rect, pen, brush);

		// The blocks of the next tetromino aren't kept after this point
		// so we should delete them
		delete block;
	}
}

void MainWindow::render_hold()
{
	std::vector<Block*> blocks = engine_.get_hold_tetromino();

	double average_x = 0;
	double average_y = 0;
	for (Block* block : blocks)
	{
		average_x += block->position.x;
		average_y += block->position.y;
	}
	average_x /= blocks.size();
	average_y /= blocks.size();

	scene_hold_->clear();

	QPen pen(QColor(0, 0, 0));
	for (Block* block : blocks)
	{
		QRectF rect(NEXT_BORDER_RIGHT / 2 + (block->position.x - average_x - 0.5) * SQUARE_SIDE,
					NEXT_BORDER_DOWN / 2 - (block->position.y - average_y + 0.5) * SQUARE_SIDE,
					SQUARE_SIDE,
					SQUARE_SIDE);

		QBrush brush(block->color);

		scene_hold_->addRect(rect, pen, brush);
	}
}

void MainWindow::update_game_time()
{
	ui->labelState->setText(QString(playing_ ? "Time" : "Game over") + ": " + QString::number(game_time_elapsed_) + " seconds");
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	if (!playing_ || paused_)
	{
		return;
	}

	switch (event->key())
	{
	case Qt::Key_D:
		engine_.move_horizontal(1);
		render();
		break;
	case Qt::Key_A:
		engine_.move_horizontal(-1);
		render();
		break;
	}

	if (event->isAutoRepeat())
	{
		return;
	}

	switch (event->key())
	{
	case Qt::Key_E:
		engine_.rotate(true);
		render();
		break;
	case Qt::Key_Q:
		engine_.rotate(false);
		render();
		break;

	case Qt::Key_Shift:
		engine_.drop();
		tick();
		break;
	case Qt::Key_S:
		timer_->setInterval(engine_.game_speed / 10);
		tick();
		break;

	case Qt::Key_W:
		engine_.hold();
		render_hold();
		render();
		break;
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
	if (!playing_ || paused_ || event->isAutoRepeat())
		return;

	switch (event->key())
	{
	case Qt::Key_S:
		timer_->setInterval(engine_.game_speed);
		break;
	}
}










