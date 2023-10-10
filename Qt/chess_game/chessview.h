#ifndef CHESSVIEW_H
#define CHESSVIEW_H

#include <QWidget>
#include <QPointer>
#include <QMap>
#include <QIcon>
#include "chessboard.h"


class ChessView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QSize fieldSize READ fieldSize WRITE setFieldSize NOTIFY fieldSizeChanged)
public: //CLASSES

    class Highlight {
    public:
        Highlight() {}
        virtual ~Highlight() {}
        virtual int type() const { return 0;}
    };

    class FieldHighlight : public Highlight {
    private:
        QPoint m_field;
        QColor m_color;
    public:
        enum {Type = 1};
        FieldHighlight(int column, int rank, QColor color) : m_field(column, rank), m_color(color) {}
        inline int column() const { return m_field.x();}
        inline int rank() const { return m_field.y();}
        inline QColor color() const { return m_color;}
        int type() const {return Type;}
    };

private:
    QPointer<ChessBoard> m_board;
    QSize m_fieldSize;

    QMap<char,QIcon> m_pieces;
    QList<Highlight*> m_highlights;

public: //METHODS

    ChessView(QWidget *parent=nullptr);
    //BOARD
    void setBoard(ChessBoard *board);
    ChessBoard* board() const { return m_board;}

    //FIELD
    QRect fieldRect(int column, int rank) const;
    QPoint fieldAt(const QPoint &pt) const;
    QSize fieldSize() const
    {
        return m_fieldSize;
    }

    //PIECE
    void setPiece(char type, const QIcon &icon);
    QIcon piece(char type) const;

    //HIGHLIGHT
    void addHighlight(Highlight *hl);
    void removeHighlight(Highlight *hl);
    inline Highlight* highlight(int index) const { return m_highlights.at(index);}
    inline int highlightCount() const { return m_highlights.size();}
protected:
    QSize sizeHint() const;
    void paintEvent(QPaintEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    //Draw
    virtual void drawRank(QPainter* painter, int rank);
    virtual void drawColumn(QPainter* painter, int column);
    virtual void drawField(QPainter* painter, int column, int rank);
    virtual void drawPiece(QPainter* painter, int column, int rank);
    virtual void drawHighlight(QPainter *painter);
public slots:
    //FOR FieldSize
    void setFieldSize(QSize fieldSize)
    {
        if (m_fieldSize == fieldSize)
            return;

        m_fieldSize = fieldSize;
        emit fieldSizeChanged(m_fieldSize);
        updateGeometry();
    }
signals:
    void fieldSizeChanged(QSize fieldSize);
    void clicked(const QPoint &);
};

#endif // CHESSVIEW_H
