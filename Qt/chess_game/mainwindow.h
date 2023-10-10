#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chessview.h"
#include "chessalgorithm.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    ChessView* m_view;
    ChessView::FieldHighlight *m_selectedField;
    ChessAlgorithm* m_algorithm;
    QPoint m_clickPoint;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void viewClicked(const QPoint &field);
};

#endif // MAINWINDOW_H
