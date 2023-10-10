#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "docwindow.h"
#include "lexer.h"
#include "parser.h"
#include "generator.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:

    DocWindow* pDoc = nullptr;
    QMenu* pMenuFile = nullptr;
    QMenu* pMenuCompile = nullptr;

    const char* source_txt;

public:
    explicit MainWindow(QWidget* par = nullptr);
    ~MainWindow();

public slots:
    Lexer*  LexAnalysis();
    Parser* ParseAnalysis(Lexer* lex);
    std::string Generate(AST* tr);

    bool Compile();
    void ConsoleClear();

    void slotChangeWindowTitle(const QString& str);
    void slotShowNotification(const QString& str);
};

#endif // MAINWINDOW_H
