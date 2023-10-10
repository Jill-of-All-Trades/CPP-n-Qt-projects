#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tictactoewidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void startNewGame();
    void handleGameOver(TicTacToeWidget::Player winner);
private slots:
    void updateNameLabels();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
