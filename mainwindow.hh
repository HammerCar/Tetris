#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QTime>

#include "engine.hh"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	void render();

private slots:
	void play();
	void pause();

	void tick();
	void time_tick();

private:
	Ui::MainWindow* ui;

    QGraphicsScene* scene_;
	QGraphicsScene* scene_next_;
	QGraphicsScene* scene_hold_;

	Engine engine_;
	QTimer* timer_;

	QTimer* game_time_timer_;
	int game_time_elapsed_;

	bool playing_ = false;
	bool paused_ = false;

    // Constants describing scene coordinates
    // Copied from moving circle example and modified a bit
    const int BORDER_UP = 0;
    const int BORDER_DOWN = 480; // 260; (in moving circle)
    const int BORDER_LEFT = 0;
	const int BORDER_RIGHT = 240; // 680; (in moving circle)

	const int NEXT_BORDER_UP = 0;
	const int NEXT_BORDER_DOWN = 100;
	const int NEXT_BORDER_LEFT = 0;
	const int NEXT_BORDER_RIGHT = 110;

    // Size of a tetromino component
    const int SQUARE_SIDE = 20;
    // Number of horizontal cells (places for tetromino components)
    const int COLUMNS = BORDER_RIGHT / SQUARE_SIDE;
    // Number of vertical cells (places for tetromino components)
	const int ROWS = BORDER_DOWN / SQUARE_SIDE;


	void render_block(Block* block);
	void render_next();
	void render_hold();

	void update_game_time();

	void keyPressEvent(QKeyEvent* event);
	void keyReleaseEvent(QKeyEvent* event);
};

#endif // MAINWINDOW_HH
