#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_chartSettings_released();

    void on_btn_calc_path_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
