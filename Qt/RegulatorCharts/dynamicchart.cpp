#include "dynamicchart.h"
#include <QPen>

DynamicChart::DynamicChart(QWidget *widget) : QChartView (widget)
{
    // Inits
    chart = new QChart();
    lines = new DCLine();
    points_selected = new QScatterSeries();

    lbl_mouse_pos = nullptr;

    state = MouseState::none;
    mouse_prev_pos = QPointF(0,0);

    CreateActions(); // context menu actions

    // Setups
    lines->setPointsVisible(true);
    lines->pen().setWidth(2);
    chart->setTheme(QChart::ChartThemeBlueCerulean);

    setMouseTracking(true); // track mouse position
}

DynamicChart::~DynamicChart() {
    delete chart;
    delete lines;
    delete points_selected;
}

//:: Graph points
DCLine* DynamicChart::GetDCLine() const {
    return lines;
}

QChart* DynamicChart::GetChart() const {
    return chart;
}

int DynamicChart::GetNearestPointIndexAtPos(QPointF pos) {
    QList<QPointF> points = lines->points();
    int pcount = points.size();
    int nearIndex = -1;
    qreal distMin = 1000000;
    for(int i=0; i<pcount;++i){
        QPointF p = points[i];
        QPointF vec2 = (pos - p);
        qreal dist = qSqrt(vec2.x()*vec2.x() + vec2.y()*vec2.y());
        if(dist < distMin && dist < selection_radius) {
            distMin = dist;
            nearIndex = i;
        }
    }
    qInfo() << "- Near Index: " << nearIndex;
    return nearIndex;
}

void DynamicChart::AppendPoint(qreal x, qreal y) {
    lines->append(x, y);
}

void DynamicChart::InsertPoint(QPointF point) {
    QList<QPointF> points = lines->points();
    int pcount = points.size();
    int nearIndex = 0;
    qreal distMin = 1000000;
    for(int i=0; i<pcount;++i){
        QPointF p = points[i];
        qreal dist = (point - p).x();
        if(dist < distMin && dist > 0) {
            distMin = dist;
            nearIndex = i;
        }
    }
    qInfo() << "Near Index: " << nearIndex << ", point: " << points[nearIndex];
    // :: Insert point
    lines->insert(nearIndex+1, point);
}
void DynamicChart::InsertPoint(qreal x, qreal y) {
    InsertPoint(QPointF(x, y));
}

// :: Selected points
void DynamicChart::AddSelectedPoint(QPointF point) {
    points_selected->append(point);
}
void DynamicChart::AddSelectedPoint(qreal x, qreal y) {
    AddSelectedPoint(QPointF(x,y));
}

void DynamicChart::ClearSelectedPoints() {
    points_selected->clear();
}

void DynamicChart::InitTest() {
    AppendPoint(0, 6);
    AppendPoint(2, 4);
    AppendPoint(3, 8);
    AppendPoint(7, 4);
    AppendPoint(10, 5);
    *lines << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);
    //*points_selected << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    chart->legend()->hide();
    chart->addSeries(lines);
    chart->addSeries(points_selected);
    chart->createDefaultAxes();
    chart->setTitle("Кривая разгона");

    //points_selected->attachAxis(chart->axes()[0]);
    //points_selected->attachAxis(chart->axes()[1]);
    //chart->addSeries(points_selected);

    setChart(chart);
    setRenderHint(QPainter::Antialiasing);
}

// :: Events
void DynamicChart::mousePressEvent(QMouseEvent *event) {
    state = MouseState::pressed;
}

void DynamicChart::mouseMoveEvent(QMouseEvent *event) {
    QPointF point = chart->mapToValue(event->pos());

    // - Move selected
    if(state == MouseState::pressed && (event->buttons() & Qt::LeftButton)) {
        QPointF move = point - mouse_prev_pos;

        // - move selected points
        QList<QPointF> points_sel = points_selected->points();
        QList<QPointF> points = lines->points();
        for(int i=0; i < points_sel.count(); ++i) {
            QPointF p = points_sel[i];
            QPointF np = p + move;
            points_selected->replace(p, np);

            // - move line points
            lines->replace(p, np);
        }
    }
    mouse_prev_pos = point;

    // - Update label
    QString str = "Mouse position: x = " + QString::number(point.x()) + ", y = " + QString::number(point.y());
    lbl_mouse_pos->setText(str);

    // - Draw Foreground
    chart->update();
}

void DynamicChart::mouseReleaseEvent(QMouseEvent *event) {
    qInfo() << "Mouse Release Event - triggered";

    if(event->button() & Qt::RightButton) {
        // Add point
        //qInfo() << "Mouse Release Event - Left button at " << event->x() << ", " << event->y();
        QPointF point = chart->mapToValue(event->pos());
        qInfo() << "Mouse Release Event - Left button at " << point;
        //InsertPoint(point);
        CreateContextMenu();

    }
    else if(event->button() & Qt::LeftButton) {
        // Select point
        QPointF mousePos = chart->mapToValue(event->pos());
        // -- found nearest point to mouse
        int nearIndex = GetNearestPointIndexAtPos(mousePos);

        if(nearIndex > -1) { // Point was found
            QList<QPointF> points_sel = points_selected->points();
            QList<QPointF> points = lines->points();
            QPointF &ps = points[qBound(nearIndex, 0, points.size()-1)];
            int already_in_list_inx = points_sel.indexOf(ps);
            qInfo() << "Same point in list at " << already_in_list_inx;
            if(already_in_list_inx > -1) {
                // remove selected
                points_selected->remove(already_in_list_inx);
                qInfo() << "deleting point at " << already_in_list_inx;
            } else {
                // add as selected
                points_selected->append(ps);
                qInfo() << "adding point: " << ps;
            }
        }
    }

    // State - none
    state = MouseState::none;
}

void DynamicChart::keyPressEvent(QKeyEvent *event) {
    if(hasFocus())
    switch(event->key()) {
        case Qt::Key_Delete:
        // - Delete selected points
        slotDeleteSelectedPoints();
        break;
    }
}


// CONTEXT MENU
void DynamicChart::CreateActions() {
    // - Add Point Action
    addPointAct = new QAction(tr("Вставить точку"), this);
    addPointAct->setStatusTip(tr("Добавление новой точки на график"));
    connect(addPointAct, &QAction::triggered, this, &DynamicChart::slotInsertPoint);

    // - Delete Point Action
    deletePointAct = new QAction(tr("Удалить выделенные точки"), this);
    deletePointAct->setStatusTip(tr("Удалить выделенные точки с графика"));
    connect(deletePointAct, &QAction::triggered, this, &DynamicChart::slotDeleteSelectedPoints);

    // - Reset chart Action
    resetChartAct = new QAction(tr("Сбросить график"), this);
    resetChartAct->setStatusTip(tr("Вернуть график в исходный вид"));
    connect(resetChartAct, &QAction::triggered, this, &DynamicChart::slotResetChart);

    // - Reset chart Action
    clearSelectionAct = new QAction(tr("Снять выделение"), this);
    clearSelectionAct->setStatusTip(tr("Убрать выделение для всех точек"));
    connect(clearSelectionAct, &QAction::triggered, this, &DynamicChart::slotClearSelection);
}

void DynamicChart::CreateContextMenu() {
    context_menu = new QMenu(this);
    context_menu->addAction(addPointAct);
    context_menu->addAction(deletePointAct);
    context_menu->addAction(clearSelectionAct);
    context_menu->addSeparator();
    context_menu->addAction(resetChartAct);
    context_menu->popup(QCursor::pos());
}

void DynamicChart::slotInsertPoint() {
    InsertPoint(mouse_prev_pos);
}

void DynamicChart::slotDeleteSelectedPoints() {
    // - Delete selected points
    QList<QPointF> points_sel = points_selected->points();
    QList<QPointF> points = lines->points();
    for(int i = 0; i < points_sel.count(); ++i) {
        QPointF p = points_sel[i];
        // - Seek point index in line
        int p_inx = points.indexOf(p);
        if(p_inx > 0 && p_inx < points.count()-1) { // its not 1st & not last point
            lines->remove(p_inx);
        }
    }
    // - Also clear all selected
    ClearSelectedPoints();
}

void DynamicChart::slotResetChart() {
    ClearSelectedPoints();
    lines->clear();


    AppendPoint(0, 0);
    AppendPoint(2, 2.75);
    AppendPoint(8, 5.3);
    AppendPoint(20, 6);
}

void DynamicChart::slotClearSelection() {
    ClearSelectedPoints();
}
