#include "pointlistdelegate.h"

#include "dialogpointlist.h"

// https://www.bogotobogo.com/Qt/Qt5_QTableView_QItemDelegate_ModelView_MVC.php
//

#include <QLineEdit>

PointListDelegate::PointListDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    validator = new QDoubleValidator();

    // Signals
    DialogPointList* dpl = static_cast<DialogPointList*>(this->parent());
    connect(this, &PointListDelegate::closeEditor, dpl, &DialogPointList::onCellChanged);
}

// TableView need to crate an Editor
// Craete Editor when we construct this
// and return the Editor
QWidget* PointListDelegate::createEditor(QWidget *parent,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    QLineEdit *editor = new QLineEdit(parent);
    editor->setPlaceholderText("<пусто>");
    editor->setValidator(validator);

    return editor;
}

// Then, we set the Editor
// Gets the data from Model and feeeds the data to Editor
void PointListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    // Get the value via index of the model
    QString value = index.model()->data(index, Qt::EditRole).toString();

    // Put the value into the line editor
    QLineEdit *line_edit = static_cast<QLineEdit*>(editor);
    line_edit->setText(value);
}

// When we modify data, this model reflect the change
void PointListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QLineEdit *line_editor = static_cast<QLineEdit*>(editor);
    model->setData(index, line_editor->text().toDouble());
}

// Give the info on size and location
void PointListDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {

}
