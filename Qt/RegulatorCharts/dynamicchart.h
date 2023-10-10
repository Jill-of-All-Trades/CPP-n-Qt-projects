#ifndef DYNAMICCHART_H
#define DYNAMICCHART_H

#include <QLineSeries>
#include <QtCharts>

// Note 1: Give up tracing, this shit doesn't implemented there
// Note 2: Dialog->Tab2 - axis ranges has no range check yet

typedef QSplineSeries DCLine; // if u wanna change the type of line
                              // just change it once here

class DynamicChart : public QChartView
{
    enum MouseState {
        none,
        pressed
    };

private:
    QChart *chart;
    DCLine *lines;

    // :: selection
    QScatterSeries *points_selected;
    QPointF mouse_prev_pos;
    MouseState state;

    const qreal selection_radius = 1.;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *k);

public:
    DynamicChart(QWidget *widget);
    ~DynamicChart();

    void InitTest();

    DCLine* GetDCLine() const;
    QChart* GetChart() const;

    void AppendPoint(qreal x, qreal y);
    void InsertPoint(QPointF point);
    void InsertPoint(qreal x, qreal y);

    // - Selected points
    int GetNearestPointIndexAtPos(QPointF pos);

    void AddSelectedPoint(QPointF point);
    void AddSelectedPoint(qreal x, qreal y);
    void ClearSelectedPoints();

public:
    // :: External
    QLabel *lbl_mouse_pos;

    // Context menu
private:
    void CreateActions();
    void CreateContextMenu();
    bool is_cmenu_open = false;

    QMenu *context_menu;

    QAction *addPointAct;
    QAction *deletePointAct;
    QAction *resetChartAct;
    QAction *clearSelectionAct;

private slots:
    void slotInsertPoint();
    void slotDeleteSelectedPoints();
    void slotResetChart();
    void slotClearSelection();

};

#endif // DYNAMICCHART_H
