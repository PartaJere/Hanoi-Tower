// Jere Aho, jere.aho@tuni.fi
#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

    // We need a graphics scene in which to draw a circle
    scene_ = new QGraphicsScene(this);

    // The graphicsView object is placed on the window
    // at the following coordinates:
    int left_margin = 20; // x coordinate
    int top_margin = 60; // y coordinate
    // The width of the graphicsView is BORDER_RIGHT added by 2,
    // since the borders take one pixel on each side
    // (1 on the left, and 1 on the right).
    // Similarly, the height of the graphicsView is BORDER_DOWN added by 2.
    ui_->graphicsView->setGeometry(left_margin, top_margin,
                                   BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui_->graphicsView->setScene(scene_);
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    // Sets limits to amount of disk.
    ui_->diskAmountSpinBox->setMaximum(99);
    ui_->diskAmountSpinBox->setMinimum(3);

    // Insert taps to map.
    disks_.insert({"first", {}});
    disks_.insert({"second", {}});
    disks_.insert({"third", {}});

    // Creates timer
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(changeTime()));

    // Starts new game.
    new_game();
}

MainWindow::~MainWindow()
{
    delete ui_;
    delete scene_;
    delete timer_;


}

void MainWindow::new_game()
{

    // Sets all buttons enabled.
    ui_->ABButton->setEnabled(true);
    ui_->ACButton->setEnabled(true);
    ui_->BAButton->setEnabled(true);
    ui_->BCButton->setEnabled(true);
    ui_->CAButton->setEnabled(true);
    ui_->CBButton->setEnabled(true);

    // If disk amount is changed, resets the best time.
    if ( amount_of_disks_ != ui_->diskAmountSpinBox->value()){
        best_time_ = {0, 0};
        ui_->bestMinLcdNumber->display(best_time_.first);
        ui_->bestSecLcdNumber->display(best_time_.second);
    }
    amount_of_disks_ = ui_->diskAmountSpinBox->value();
    // Calculates how big a difference is in the width of two closest disk.
    int width_difference = (BIGGEST_DISK_WIDTH-SMALLEST_DISK_WIDTH)
                            / amount_of_disks_;

    // Sets disk_height_ back to normal
    disk_height_ = NORMAL_DISK_HEIGHT_;
    // If disks do not fit to scene, adjusts disk height until they do.
    while ( amount_of_disks_ * disk_height_ > BOTTOM_OF_TAP ){
       disk_height_ -= 1;
    }

    // Initializes disks.
    for ( int i = 0 ; i < amount_of_disks_ ; i++ ){
        QGraphicsRectItem* disk;
        int offset;
        int width = BIGGEST_DISK_WIDTH - i*width_difference;
        offset = width/2;
        disk = scene_->addRect(0, 0, width, disk_height_);
        disk->moveBy(TAPS_x_coords.at("first") - offset,
                     BOTTOM_OF_TAP - i * disk_height_);
        disks_.at("first").push_back(disk);
    }
    timer_->start(1000);
}

void MainWindow::clear_game()
{
    for ( auto& pair : disks_ ){
        for ( QGraphicsRectItem* disk : pair.second ){
            delete disk;
        }
        pair.second.clear();
    }
    current_time_ = {0, 0};
    // Clears labels and sets current time back to zero.
    ui_->currentMinLcdNumber->display(current_time_.first);
    ui_->currentSecLcdNumber->display(current_time_.second);
    ui_->winLabel->clear();
    ui_->gameLogTextBrowser->clear();
    ui_->illegalLabel->clear();
}

bool MainWindow::move_disk(std::string from, std::string to)
{
    ui_->illegalLabel->clear();
    if ( not disks_.at(from).empty() ){
        QGraphicsRectItem* disk = disks_.at(from).back();
        if ( not disks_.at(to).empty() ){
            // Disks x coordinate minus its taps coordinate is half of the discs
            // coordinate, hence this checks if move is illegal.
            if ( ( TAPS_x_coords.at(from) - disk->x() ) >
                 ( TAPS_x_coords.at(to) - disks_.at(to).back()->x()) ){
                ui_->illegalLabel->setText("Illegal move!\nBigger disk on top "
                                          "of smaller.");
                return false;
            }
        }
        // Calculates how much disk needs to be moved
        int disks_at_from = disks_.at(from).size();
        int disks_at_to = disks_.at(to).size();
        int delta_x = TAPS_x_coords.at(to) - TAPS_x_coords.at(from);
        int delta_y = (disks_at_from - disks_at_to - 1) * disk_height_;
        disk->moveBy(delta_x, delta_y);
        // Moves disk from from-tap deque to to-tap deque.
        disks_.at(from).pop_back();
        disks_.at(to).push_back(disk);
        // Checks if player has won.
        check_win();
        return true;
        }
    else{
        ui_->illegalLabel->setText("No disk to move.");
        return false;
    }

}

void MainWindow::check_win()
{
    if ( disks_.at("first").empty() ){
        ui_->winLabel->setText("You have won!");
        // Sets all move buttons disabled.
        ui_->ABButton->setEnabled(false);
        ui_->ACButton->setEnabled(false);
        ui_->BAButton->setEnabled(false);
        ui_->BCButton->setEnabled(false);
        ui_->CAButton->setEnabled(false);
        ui_->CBButton->setEnabled(false);

        timer_->stop();
        if ( best_time_.first == 0 and best_time_.second == 0 ){
            best_time_ = current_time_;
        }
        else if ( best_time_.first > current_time_.first){
            best_time_ = current_time_;
        }
        else if ( best_time_.first == current_time_.first and
                  current_time_.second < best_time_.second ){
            best_time_ = current_time_;
        }
        ui_->bestMinLcdNumber->display(best_time_.first);
        ui_->bestSecLcdNumber->display(best_time_.second);
    }
}

void MainWindow::on_newGameButton_clicked()
{
    clear_game();
    new_game();
}

void MainWindow::on_ABButton_clicked()
{
    if ( move_disk("first", "second") ){
        ui_->gameLogTextBrowser->append("A -> B");
    }
}

void MainWindow::on_ACButton_clicked()
{
    if ( move_disk("first", "third") ){
        ui_->gameLogTextBrowser->append("A -> C");
    }
}

void MainWindow::on_BAButton_clicked()
{
    if ( move_disk("second", "first") ){
        ui_->gameLogTextBrowser->append("B -> A");
    }
}

void MainWindow::on_BCButton_clicked()
{
    if ( move_disk("second", "third") ){
        ui_->gameLogTextBrowser->append("B -> C");
    }
}

void MainWindow::on_CAButton_clicked()
{
    if ( move_disk("third", "first") ){
        ui_->gameLogTextBrowser->append("C -> A");
    }
}

void MainWindow::on_CBButton_clicked()
{
    if ( move_disk("third", "second") ){
        ui_->gameLogTextBrowser->append("C -> B");
    }
}

void MainWindow::changeTime(){
    ++current_time_.second;
    if ( current_time_.second == 60 ){
        ++current_time_.first;
        current_time_.second = 0;
    }
    ui_->currentMinLcdNumber->display(current_time_.first);
    ui_->currentSecLcdNumber->display(current_time_.second);

}

void MainWindow::keyPressEvent( QKeyEvent* event )
{
    if ( event->key() == Qt::Key_1 ){
        on_ABButton_clicked();
    }
    if ( event->key() == Qt::Key_2 ){
        on_ACButton_clicked();
    }
    if ( event->key() == Qt::Key_3 ){
        on_BAButton_clicked();
    }
    if ( event->key() == Qt::Key_4 ){
        on_BCButton_clicked();
    }
    if ( event->key() == Qt::Key_5 ){
        on_CAButton_clicked();
    }
    if ( event->key() == Qt::Key_6 ){
        on_CBButton_clicked();
    }
}
