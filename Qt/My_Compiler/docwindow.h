#ifndef DOCWINDOW_H
#define DOCWINDOW_H

#include <QTextEdit>

/*
 * Класс окна текстового ввода
 *
*/

class DocWindow : public QTextEdit
{
    Q_OBJECT
private:
    QString m_strFileName;
public:
    explicit DocWindow(QWidget* par = nullptr);
    ~DocWindow();

signals:
    void changeWindowTitle(const QString&);
    void showNotification(const QString&);

public slots:
    void slotLoad();
    void slotSave();
    void slotSaveAs();
};

#endif // DOCWINDOW_H
