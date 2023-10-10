#include "chessboard.h"
#include <QDebug>

ChessBoard::ChessBoard(int r, int c, QObject *parent) : QObject(parent)
{
    m_ranks = r;
    m_columns = c;
    initBoard();
}

//Read data
char ChessBoard::data(int column, int rank) const
{
    return m_boardData.at((rank-1)*columns()+(column-1));
}

//Set Data
void ChessBoard::setData(int column, int rank, char value)
{
    if(setDataInternal(column,rank,value))
        emit dataChanged(column, rank);
}

void ChessBoard::movePiece(int fromColumn, int fromRank, int toColumn, int toRank)
{
    setData(toColumn, toRank, data(fromColumn, fromRank));
    setData(fromColumn, fromRank, ' ');
}

void ChessBoard::setFen(const QString &fen)
{
    int index = 0;
    int skip = 0;
    const int columnCount = columns();
    QChar ch;
    for (int rank = ranks(); rank > 0; --rank) {
        for(int column = 1; column <= columnCount; ++column) {
            //Set skip ' '
            if(skip > 0) {
                ch = ' ';
                skip--;
            } else {
                ch = fen.at(index++);           //get char
                if(ch.isDigit()) {              //IF its digit...
                    skip = ch.toLatin1()-'0';   //set skip
                    ch = ' ';
                    skip--;
                }
            }
            setDataInternal(column, rank, ch.toLatin1());
        }
        QChar next = fen.at(index++);
        if(next != '/' && next != ' ') {
             qDebug() << "Found: "<< next << " at " << index-1 << "Not found / or ' -> breaked";
             initBoard();
             return; //fail on return
        }
    }
    emit boardReset();
}

//Get Fen
QString ChessBoard::fen() const
{
    QString str;
    for(int r = ranks(); r>0; --r) {
        for(int c = 1 ; c <= columns(); ++c) {
            str += data(c, r);
        }
    }
    return str;
}

void ChessBoard::setRanks(int NewRanks)
{
    if(ranks() == NewRanks) return;
    m_ranks = NewRanks;
    initBoard();
    emit ranksChanged(m_ranks);
}

void ChessBoard::setColumns(int newColumns)
{
    if(columns() == newColumns) return;
    m_columns = newColumns;
    initBoard();
    emit columnsChanged(m_columns);
}

//Init Board (set all cells to ' ')
void ChessBoard::initBoard()
{
    m_boardData.fill(' ', ranks()*columns());
    emit boardReset();
}

//Change data (if it is new value) & return success(T/F)
bool ChessBoard::setDataInternal(int column, int rank, char value)
{
    int index = (rank-1)*columns()+(column-1);
    if(m_boardData.at(index) == value) return false;
    m_boardData[index] = value;
    return true;
}
