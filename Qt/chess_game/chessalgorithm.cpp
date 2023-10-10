#include "chessalgorithm.h"
#include <QDebug>

ChessAlgorithm::ChessAlgorithm(QObject* parent) :QObject (parent)
{
    m_board = nullptr;
    m_currentPlayer = NoPlayer;
    m_result = NoResult;
}

void ChessAlgorithm::newGame()
{
    setupBoard();
    board()->setFen(
                "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    //Debug - Fen
    qDebug() << "Fen is: " << board()->fen();
}

bool ChessAlgorithm::move(int colFrom, int rankFrom, int colTo, int rankTo)
{
    Q_UNUSED(colFrom)
    Q_UNUSED(rankFrom)
    Q_UNUSED(colTo)
    Q_UNUSED(rankTo)
    return false;
}

bool ChessAlgorithm::move(const QPoint &from, const QPoint &to)
{
    return move(from.x(), from.y(), to.x(), to.y());
}

//Setup game board (create)
void ChessAlgorithm::setupBoard()
{
    setBoard(new ChessBoard(8, 8, this));
}

//Set new/other game board
void ChessAlgorithm::setBoard(ChessBoard *board)
{
    if(board == m_board) return;
    if(m_board) delete m_board;
    m_board = board;
    emit boardChanged(m_board);
}

void ChessAlgorithm::setResult(ChessAlgorithm::Result value)
{
    if(result() == value) return;
    if(result() == NoResult) {
        m_result = value;
        emit gameOver(m_result);
    } else { m_result = value; }
}

void ChessAlgorithm::setCurrentPlayer(ChessAlgorithm::Player value)
{
    if(currentPlayer() == value) return;
    m_currentPlayer = value;
    emit currentPlayerChanged(m_currentPlayer);
}



//FOX & HOUNDS
FoxAndHounds::FoxAndHounds(QObject *parent) : ChessAlgorithm (parent)
{

}

void FoxAndHounds::newGame()
{
    setupBoard();
    board()->setFen("3p4/8/8/8/8/8/8/P1P1P1P1 w"); // w - white to move
    m_fox = QPoint(5,8);
    setResult(NoResult);
    setCurrentPlayer(Player1);
}

bool FoxAndHounds::move(int colFrom, int rankFrom, int colTo, int rankTo)
{
    if(currentPlayer() == NoPlayer) return false;
    //is there a piece of right color?
    char source = board()->data(colFrom,rankFrom);
    if(currentPlayer() == Player1 && source != 'P') return false;
    if(currentPlayer() == Player2 && source != 'p') return false;

    //both can only move one column right or left
    if(colTo != colFrom+1 && colTo != colFrom-1) return false;

    //do we move within the board?
    if(colTo < 1 || colTo > board()->columns()) return false;
    if(rankTo < 1 || rankTo > board()->ranks()) return false;

    //is the destination field black?
    if((colTo +rankTo)%2) return false;

    //is the destination field empty?
    char destination = board()->data(colTo, rankTo);
    if(destination != ' ') return false;

    //is white advancing?
    if(currentPlayer() == Player1 && rankTo <= rankFrom) return false;

    board()->movePiece(colFrom, rankFrom, colTo, rankTo);
    //make move
    if(currentPlayer() == Player2) {
        m_fox = QPoint(colTo, rankTo); //cache fox position
    }

    //check win condition
    if(currentPlayer() == Player2 && rankTo == 1) {
        setResult(Player2Wins); //fox has escaped
    } else if (currentPlayer() == Player1 && !foxCanMove()) {
        setResult(Player1Wins); //fox can't move
    } else {
        //the other player makes move now
        setCurrentPlayer(currentPlayer() == Player1 ? Player2 : Player1);
    }
    return true;
}

bool FoxAndHounds::foxCanMove() const
{
    if(emptyByOffset(-1, -1) || emptyByOffset(-1, 1)
            || emptyByOffset(1, -1) || emptyByOffset(1,1)) return true;
    else return false;
}

bool FoxAndHounds::emptyByOffset(int x, int y) const
{
    const int destCol = m_fox.x() + x;
    const int destRank = m_fox.y() + y;
    if(destCol < 1 || destRank < 1 || destCol > board()->columns() || destRank > board()->ranks()) return false;
    return (board()->data(destCol, destRank) == ' ');
}
