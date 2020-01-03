/* Mainwindow for hanoi tower.
 * Jere Aho, jere.aho@tuni.fi
*/
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <map>
#include <deque>
#include <QTimer>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_newGameButton_clicked();

    void on_ABButton_clicked();

    void on_ACButton_clicked();

    void on_BAButton_clicked();

    void on_BCButton_clicked();

    void on_CAButton_clicked();

    void on_CBButton_clicked();

    void changeTime();

    void keyPressEvent( QKeyEvent* event );

private:
    Ui::MainWindow* ui_;
    QGraphicsScene* scene_;
    QTimer* timer_;

    // Disks in different taps.
    std::map<const std::string, std::vector<QGraphicsRectItem*>> disks_;

    int amount_of_disks_ = 3;

    // Coordinates of the game board.
    const int BORDER_UP = 0;
    const int BORDER_DOWN = 500;
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = 750;

    // Taps first disk y coordinate.
    const int BOTTOM_OF_TAP = BORDER_DOWN - 40;
    std::map<const std::string, const int>
    TAPS_x_coords = {{"first" , BORDER_RIGHT/6},
                     {"second" , BORDER_RIGHT/2},
                     {"third" , BORDER_RIGHT*5/6}};

    const int BIGGEST_DISK_WIDTH = 220;
    const int SMALLEST_DISK_WIDTH = 20;
    const int NORMAL_DISK_HEIGHT_ = 20;
    // Calculated in new_game method. Is NORMAL_DISK_HEIGHT_ if there is not
    // too many disks.
    int disk_height_ = NORMAL_DISK_HEIGHT_;

    std::pair<int, int> current_time_ = {0, 0};
    std::pair<int, int> best_time_ = {0, 0};

    // Initializes the new game.
    void new_game();
    // Clears the disks.
    void clear_game();
    // Moves one disk from one tap to another.
    bool move_disk(std::string from, std::string to);
    // Checks if player has won.
    void check_win();
};

#endif // MAINWINDOW_HH
