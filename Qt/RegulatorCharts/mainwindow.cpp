#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialogpointlist.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Binding
    ui->graphicsView->lbl_mouse_pos = ui->lbl_mousePos;

    // Test
     ui->graphicsView->InitTest();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_chartSettings_released()
{
    DialogPointList *dialog = new DialogPointList(this);
    dialog->ChartSet(ui->graphicsView->GetChart());
    dialog->LineSet(ui->graphicsView->GetDCLine());
    dialog->PointSetFromLine(); // ui->graphicsView->GetDCLine()

    dialog->exec();
}

//
// TEST
//
// https://www.bragitoff.com/2015/09/c-program-for-polynomial-fit-least-squares/
#include "simousolver.h"
void MainWindow::on_btn_calc_path_clicked()
{
    DCLine *line = ui->graphicsView->GetDCLine();
    QList<QPointF> points = line->points();

    SimouSolver solver(&points, 4);
    solver.SetRegressionPointsAmount(25);
    solver.CalcDiscreteFrequency();
    double *koefs = solver.Solve();

    ui->le_a1->setText(QString::number(koefs[0]));
    ui->le_a2->setText(QString::number(koefs[1]));
    ui->le_a3->setText(QString::number(koefs[2]));
    ui->le_a4->setText(QString::number(koefs[3]));

    delete koefs;
}
