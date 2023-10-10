#include "dialogpointlist.h"
#include "mainwindow.h"
#include "ui_dialogpointlist.h"

#include <QStandardItemModel>


DialogPointList::DialogPointList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPointList)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    // Delegate
    pl_delegate = new PointListDelegate(this);

    // Setup table
    table_points = ui->tableWidget;

    table_points->setColumnCount(2);
    table_points->setShowGrid(true);

    table_points->setSelectionMode(QAbstractItemView::SingleSelection);
    table_points->setSelectionBehavior(QAbstractItemView::SelectRows);


    table_points->setHorizontalHeaderLabels(
      QStringList() << tr("time") << tr("value")
    );
    table_points->horizontalHeader()->setStretchLastSection(true);

    table_points->showColumn(0);

    // Delegate
    table_points->setItemDelegate(pl_delegate);

    // Range Validators
    QDoubleValidator* validator = new QDoubleValidator();
    ui->le_x_min->setValidator(validator);
    ui->le_x_max->setValidator(validator);
    ui->le_y_min->setValidator(validator);
    ui->le_y_max->setValidator(validator);

    // Signals <-> Slots
    connect(this, &DialogPointList::accepted, this, &DialogPointList::onAccepted);
}

// Points
QPointF DialogPointList::PointGet(int row) const {
    QTableWidgetItem* item_time = table_points->item(row, 0);
    qreal x = item_time->data(Qt::DisplayRole).toReal();

    QTableWidgetItem* item_value = table_points->item(row, 1);
    qreal y = item_value->data(Qt::DisplayRole).toReal();

    return QPointF(x, y);
}

void DialogPointList::PointAdd(double x, double y) {
    int rows = table_points->rowCount();
    table_points->insertRow(rows);

    QTableWidgetItem *item = new QTableWidgetItem();
    item->setData(Qt::EditRole, x);
    table_points->setItem(rows, 0, item);

    item = new QTableWidgetItem();
    item->setData(Qt::EditRole, y);
    table_points->setItem(rows, 1, item);

    // Sort
    onCellChanged();
}

void DialogPointList::PointSet(int row, double x, double y) {
    QTableWidgetItem* item_time = table_points->item(row, 0);
    item_time->setData(Qt::EditRole, x);

    QTableWidgetItem* item_value = table_points->item(row, 1);
    item_value->setData(Qt::EditRole, y);

    // Sort
    onCellChanged();
}

void DialogPointList::PointRemove(int row) {
    table_points->removeRow(row);
}

void DialogPointList::PointSetFromLine(const DCLine *line) {
    if(line == nullptr) line = points_line;
    QList<QPointF> points = line->points();

    ClearTable();


    foreach(QPointF pp, points) {
        PointAdd(pp.x(), pp.y());
    }
}

// Chart
void  DialogPointList::ChartSet(QChart *chart) {
    this->chart = chart;

    // Setup Plot Range line edits
    QValueAxis *axis_x = static_cast<QValueAxis*>(chart->axes(Qt::Horizontal)[0]);
    QValueAxis *axis_y = static_cast<QValueAxis*>(chart->axes(Qt::Vertical)[0]);

    ui->le_x_min->setText(QString::number(axis_x->min()));
    ui->le_x_max->setText(QString::number(axis_x->max()));
    ui->le_y_min->setText(QString::number(axis_y->min()));
    ui->le_y_max->setText(QString::number(axis_y->max()));
}

// Lines
void DialogPointList::LineSet(DCLine *line) {
    points_line = line;
}

DCLine* DialogPointList::LineGet() const {
    return points_line;
}

// Clear
void DialogPointList::ClearTable() {
    table_points->clearContents();
    int rows = table_points->rowCount();

    for (int i = rows-1; i >= 0; --i) {
        table_points->removeRow(i);
    }
}

// Destructor
DialogPointList::~DialogPointList()
{
    delete ui;
}

//
// SLOTS
//

void DialogPointList::onAccepted() {
    // - update points
    points_line->clear();

    int row_cnt = table_points->rowCount();
    for(int i=0; i < row_cnt; ++i) {
        points_line->append(PointGet(i));
    }

    // update axis ranges
    QAbstractAxis *axis_x = (chart->axes(Qt::Horizontal)[0]);
    QAbstractAxis *axis_y = (chart->axes(Qt::Vertical)[0]);
    axis_x->setRange(ui->le_x_min->text().toDouble(), ui->le_x_max->text().toDouble());
    axis_y->setRange(ui->le_y_min->text().toDouble(), ui->le_y_max->text().toDouble());
}

void DialogPointList::onCellChanged() {
    qDebug() << QTime::currentTime().toString() << " - onCellChanged!";
    // Sort
    if(table_points->rowCount() > 1)
        table_points->sortItems(0, Qt::AscendingOrder);
}

void DialogPointList::on_btn_point_add_clicked()
{
    int rows = table_points->rowCount();
    QPointF point = PointGet(rows-1);
    PointAdd(point.x()+1, 0);
}

void DialogPointList::on_btn_point_delete_clicked()
{
    if(table_points->rowCount() <= 2) {
        QMessageBox::information(this, "Невозможно удалить точку", "Для построения линии необходимо минимум 2 точки.");
        return;
    }

    QList<QTableWidgetItem*> items_selected = table_points->selectedItems();
    QSet<int> rows_selected = QSet<int>();

    foreach(QTableWidgetItem* it, items_selected) {
        rows_selected.insert(table_points->row(it));
    }

    foreach(int row, rows_selected) {
        PointRemove(row);
    }
}

void DialogPointList::on_btn_point_load_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Открыть файл данных"), "",
        tr("Comma-Separated Values (*.csv);;All Files (*)"));

    if(filename.isEmpty())
        return;
    else {
        QFile file(filename);

        if(!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Ошибка при открытии файла"), file.errorString());
            return;
        }

        // Parse CSV
        QList<QPointF> list_points;
        while(!file.atEnd()) {
            QByteArray line = file.readLine();
            QList<QByteArray> line_splitted = line.split(',');
            qDebug() << line_splitted;
            double time = line_splitted[0].toDouble();
            double value = line_splitted[1].toDouble();
            list_points.append(QPointF(time, value));
        }

        // Load into table
        ClearTable();

        foreach(QPointF pp, list_points) {
            PointAdd(pp.x(), pp.y());
        }
    }
}

void DialogPointList::on_btn_point_save_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Сохранить файл данных"), "",
        tr("Comma-Separated Values (*.csv);;All Files (*)"));

    if(filename.isEmpty())
        return;
    else {
        QFile file(filename);

        if(!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Ошибка при открытии файла"), file.errorString());
            return;
        }

        // Save to file
        QTextStream out(&file);

        int rows = table_points->rowCount();
        for(int i = 0; i < rows; ++i) {
            QPointF point = PointGet(i);
            out << QString::number(point.x()) << ',' << QString::number(point.y());

            if(i < rows-1) out << '\r' << '\n';
        }
    }
}
