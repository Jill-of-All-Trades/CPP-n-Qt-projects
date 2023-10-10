#ifndef TICTACTOEWIDGET_H
#define TICTACTOEWIDGET_H

#include <QWidget>
class QPushButton;



class TicTacToeWidget : public QWidget
{
    Q_OBJECT
    Q_ENUMS(Player)
    Q_PROPERTY(Player currentPlayer READ currentPlayer
               WRITE setCurrentPlayer
               NOTIFY currentPlayerChanged)

public:
    enum Player {
        Invalid, Player1, Player2, Draw
    };

    TicTacToeWidget(QWidget *parent = 0);
    ~TicTacToeWidget();

    void initNewGame();
    //Player manip

    Player currentPlayer() const {return m_currentPlayer;}
    void setCurrentPlayer(Player p) {
        if(m_currentPlayer == p) return;
        m_currentPlayer = p;
        emit currentPlayerChanged(p);
    }
    //Win condition
    TicTacToeWidget::Player checkWinCondition() const;

private:
    QList<QPushButton*> board;
    void setupBoard();

    Player m_currentPlayer;
    TicTacToeWidget::Player checkWinConditionForLine(int index1, int index2, int index3) const;

public slots:
    void handleButtonClick(int);
signals:
    void currentPlayerChanged(Player);
    void gameOver(TicTacToeWidget::Player);

};

#endif // TICTACTOEWIDGET_H
