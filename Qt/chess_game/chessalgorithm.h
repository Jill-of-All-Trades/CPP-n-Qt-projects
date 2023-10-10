#ifndef CHESSALGORITHM_H
#define CHESSALGORITHM_H

#include <QObject>
#include <QPoint>
#include "chessboard.h"

class ChessAlgorithm : public QObject
{
    Q_OBJECT
    Q_ENUMS(Result Player) //Чтобы можно было использовать в Q_PROPERTY и арг. сигналов
    Q_PROPERTY(Result result READ result)
    Q_PROPERTY(Player currentPlayer READ currentPlayer NOTIFY currentPlayerChanged)
public:
    //ENUMS
    enum Result { NoResult, Player1Wins, Draw, Player2Wins};
    enum Player { NoPlayer, Player1, Player2 };

    ChessAlgorithm(QObject* parent = nullptr);
    //Get board handler
    ChessBoard* board() const { return m_board;}

    inline Result result() const {return m_result;}
    inline Player currentPlayer() const { return m_currentPlayer;}
public slots:
    virtual void newGame();
    virtual bool move(int colFrom, int rankFrom, int colTo, int rankTo);
    bool move(const QPoint &from, const QPoint &to);
signals:
    void boardChanged(ChessBoard*);
    void gameOver(Result);
    void currentPlayerChanged(Player);
protected:
    virtual void setupBoard();
    void setBoard(ChessBoard * board);
    void setResult(Result);
    void setCurrentPlayer(Player);
private:
    ChessBoard *m_board;
    Result m_result;
    Player m_currentPlayer;
};

//FOX & HOUNDS GAME
class FoxAndHounds : public ChessAlgorithm
{
private:
    QPoint m_fox;
public:
    FoxAndHounds(QObject* parent = nullptr);
    void newGame();
    bool move(int colFrom, int rankFrom, int colTo, int rankTo);
protected:
    bool foxCanMove() const;
    bool emptyByOffset(int x, int y) const;
};

#endif // CHESSALGORITHM_H
