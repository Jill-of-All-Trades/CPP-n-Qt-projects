#include "mainwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QApplication>
#include <QProcess>



MainWindow::MainWindow(QWidget *par) : QMainWindow (par)
{

    // Текстовое поле
    pDoc = new DocWindow();

    // Создаем меню File
    pMenuFile = new QMenu("&File");

    //pMenuFile->addAction("&New");
    pMenuFile->addAction("&Open...", pDoc, SLOT(slotLoad()),QKeySequence("CTRL+O"));
    pMenuFile->addSeparator();
    pMenuFile->addAction("&Save", pDoc, SLOT(slotSave()), QKeySequence("CTRL+S"));
    pMenuFile->addAction("S&ave as...", pDoc, SLOT(slotSaveAs()));
    pMenuFile->addSeparator();
    //pMenuFile->addAction("Lexer", this, SLOT(LexAnalysis()));
    //pMenuFile->addSeparator();
    pMenuFile->addAction("&Exit", qApp, SLOT(quit()), QKeySequence("CTRL+Q"));

    menuBar()->addMenu(pMenuFile);

    // Меню Compile
    pMenuCompile = new QMenu("Compile");

    pMenuCompile->addAction("&Compile", this, SLOT(Compile()), QKeySequence("CTRL+L"));
    pMenuCompile->addSeparator();
    pMenuCompile->addAction("&Console Clear", this, SLOT(ConsoleClear()), QKeySequence("CTRL+R"));

    menuBar()->addMenu(pMenuCompile);
    // Установка центрального виджета
    setCentralWidget(pDoc);

    // Signal-Slot linking
    connect(pDoc,
            SIGNAL(changeWindowTitle(const QString&)),
            SLOT(slotChangeWindowTitle(const QString&))
        );
    connect(pDoc,
            SIGNAL(showNotification(const QString&)),
            SLOT(slotShowNotification(const QString&))
        );
    //
    statusBar()->showMessage("Ready", 2000);
}

MainWindow::~MainWindow()
{
    delete source_txt;
}

// Compile!
bool MainWindow::Compile()
{
    Lexer* lex = LexAnalysis();
    Parser* parser = ParseAnalysis(lex);
    std::string output = Generate(parser->getTree());

    std::cout << "### OUTPUT PROGRAM ###\n" <<  output << std::endl;

    //Clean up
    delete parser;
    delete lex;

    return true;
}

void MainWindow::ConsoleClear()
{
    system("CLS");
}

// Лексический анализ
Lexer* MainWindow::LexAnalysis()
{
    // Получить текст
    Q_ASSERT(pDoc);
    auto plain_txt = pDoc->toPlainText();
    auto plain_std_str = plain_txt.toStdString();
    auto plain_c_str = plain_std_str.c_str();
    //source_txt = pDoc->toPlainText().toStdString().c_str();
    source_txt = plain_c_str;

    // test
    std::cout << "### Source code: ###" << std::endl;
    std::cout << source_txt <<  "\n###################" << std::endl;

    Lexer* lexer = new Lexer(source_txt);
    lexer->analys();

    slotShowNotification("Lexer analysis is done!");

    return lexer;
}

// Синтаксич. анализ
Parser* MainWindow::ParseAnalysis(Lexer* lex)
{
    Parser* parser = new Parser(lex->tokens());
    parser->analys();

    std::cout << "Parse done!" << std::endl;
    // Show results
    AST* tree = parser->getTree();
    tree->show(tree->getRoot(), 0);

    return parser;
}

std::string MainWindow::Generate(AST *tr)
{
    Generator gen(tr);
    return gen.generate();
}

// OTHER SLOTS
void MainWindow::slotChangeWindowTitle(const QString& str) {
    setWindowTitle(str);
}

void MainWindow::slotShowNotification(const QString &str)
{
    statusBar()->showMessage(str, 4000);
}
