#ifndef POINTLISTDELEGATE_H
#define POINTLISTDELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QDoubleValidator>

class PointListDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit PointListDelegate(QObject *parent = nullptr);

    // Create Editor when we construct
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    // Then, we set the Editor
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    // When we modify data, thos model reflect the change
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QDoubleValidator *validator;

signals:

public slots:

};

#endif // POINTLISTDELEGATE_H
