#include <QPushButton>
#include <QGridLayout>
#include <QSignalMapper>
#include "tictactoewidget.h"

TicTacToeWidget::TicTacToeWidget(QWidget *parent)
    : QWidget(parent)
{
    setupBoard();
}

TicTacToeWidget::~TicTacToeWidget()
{

}

void TicTacToeWidget::initNewGame()
{
    for(int i =0;i<9;++i) board.at(i)->setText(" ");
    m_currentPlayer = Player::Player1;
}

void TicTacToeWidget::handleButtonClick(int index)
{
    if(index < 0 || index >= board.size()) return;
    //out of bounds check
    QPushButton *button = board.at(index);
    if(button->text() != " ") return;
    //invalid move
    button->setText(currentPlayer() == Player1 ? "X" : "O");
    Player winner = checkWinCondition();
    if(winner == Player::Invalid) {
        setCurrentPlayer(currentPlayer() == Player1 ? Player2 : Player1);
        return;
    } else {
        emit gameOver(winner);
    }
}

//Check for a line(for 3)
TicTacToeWidget::Player TicTacToeWidget::checkWinConditionForLine(
        int index1, int index2, int index3) const {
    QString text1 = board.at(index1)->text();
    if(text1 == " ") return Player::Invalid;

    QString text2 = board.at(index2)->text();
    QString text3 = board.at(index3)->text();
    if(text1 == text2 && text1 == text3) {
        return text1 == "X" ? Player::Player1 : Player::Player2;
    }
    return Player::Invalid;
}

//Check Win condition
TicTacToeWidget::Player TicTacToeWidget::checkWinCondition() const
{
    Player result = Player::Invalid;
    //check horizontal
    for(int row =0; row < 3; ++row) {
        result = checkWinConditionForLine(row*3,
                                          row*3+1,
                                          row*3+2);
        if(result != Player::Invalid) {
            return result;
        }
    }
    //Check vertical
    for(int col=0; col < 3; ++col) {
        result = checkWinConditionForLine(col,
                                          col +3,
                                          col+3*2);
        if(result != Player::Invalid) {
            return result;
        }
    }
    //Check diagonals
    result = checkWinConditionForLine(0, 4, 8);
    if(result != Player::Invalid) return result;

    result = checkWinConditionForLine(2, 4, 6);
    if(result != Player::Invalid) return result;
    //
    for(QPushButton *button : board) {
        if(button->text() == " ")
            return Player::Invalid;
    }
    //Draw
    return Player::Draw;
}

void TicTacToeWidget::setupBoard()
{
    QGridLayout * gridLayout = new QGridLayout;
    QSignalMapper* mapper = new QSignalMapper(this);

    for(int row = 0; row < 3; ++row){
        for(int column = 0; column < 3; ++column) {
            QPushButton * button = new QPushButton;
            button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
            button->setText(" ");
            gridLayout->addWidget(button, row, column);
            board.append(button);
            mapper->setMapping(button, board.count() - 1);
            connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
        }
    }
    connect(mapper, SIGNAL(mapped(int)),this,SLOT(handleButtonClick(int)));
    setLayout(gridLayout);
}
