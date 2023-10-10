#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <QDialog>

namespace Ui {
class ConfigurationDialog;
}

class ConfigurationDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QString player1Name READ
               player1Name WRITE setPlayer1Name)
    Q_PROPERTY(QString player2Name READ
               player2Name WRITE setPlayer2Name)

public:
    explicit ConfigurationDialog(QWidget *parent = nullptr);
    ~ConfigurationDialog();

    void setPlayer1Name(const QString &pl1name);
    void setPlayer2Name(const QString &pl2name);
    QString player1Name() const;
    QString player2Name() const;
public slots:
    void updateOKButtonState();
private:
    Ui::ConfigurationDialog *ui;
};

#endif // CONFIGURATIONDIALOG_H
