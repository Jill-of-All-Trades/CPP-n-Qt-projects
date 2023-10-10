#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QObject>

class ChessBoard : public QObject
{
    Q_OBJECT
    //Ranks& columns props
    Q_PROPERTY(int ranks READ ranks NOTIFY ranksChanged)
    Q_PROPERTY(int columns READ columns NOTIFY columnsChanged)
private:
    int m_ranks;
    int m_columns;
    QVector<char> m_boardData;
public:
    ChessBoard(int r=8, int c=8, QObject* parent = nullptr);

    //Read/Set cell's data
    char data(int column, int rank) const;
    void setData(int column, int rank, char value);

    //Move Figure
    void movePiece(int fromColumn, int fromRank, int toColumn, int toRank);

    //Forsyth-Edwards Notation (fill the board)
    void setFen(const QString &fen);
    QString fen() const;

    //For QPROPERTIES
    int ranks() const { return m_ranks;}
    int columns() const { return m_columns;}
    //
protected:
    void setRanks(int NewRanks);
    void setColumns(int newColumns);

    void initBoard();

    //~setData
    bool setDataInternal(int column, int rank, char value);
signals:
    void ranksChanged(int);
    void columnsChanged(int);

    void boardReset();

    void dataChanged(int c, int r);
};

#endif // CHESSBOARD_H
