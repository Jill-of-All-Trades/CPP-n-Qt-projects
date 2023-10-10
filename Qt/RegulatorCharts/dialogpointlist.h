#ifndef DIALOGPOINTLIST_H
#define DIALOGPOINTLIST_H

#include <QDialog>
#include <QTableView>

#include "pointlistdelegate.h"
#include "dynamicchart.h"

namespace Ui {
class DialogPointList;
}



class DialogPointList : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPointList(QWidget *parent = nullptr);
    ~DialogPointList();

    // Init
    void ChartSet(QChart *chart);

    void LineSet(DCLine *line);
    DCLine* LineGet() const;

    // Point manip
    QPointF PointGet(int row) const;
    void PointAdd(double x, double y);
    void PointSet(int row, double x, double y);
    void PointRemove(int row);

    // Load point from line
    void PointSetFromLine(const DCLine *line = nullptr);

    // Clear
    void ClearTable();

private:
    Ui::DialogPointList *ui;

    DCLine* points_line;
    QChart* chart;

    QTableWidget *table_points;
    PointListDelegate *pl_delegate;


public slots:
    void onAccepted();
    void onCellChanged();

private slots:
    void on_btn_point_add_clicked();
    void on_btn_point_delete_clicked();
    void on_btn_point_load_clicked();
    void on_btn_point_save_clicked();
};

//class PointsModel : public QAbstractTableModel {
//    Q_OBJECT

//public:
//    PointsModel(QObject *parent = nullptr);

//};


#endif // DIALOGPOINTLIST_H
