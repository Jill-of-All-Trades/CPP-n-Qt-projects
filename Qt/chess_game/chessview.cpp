#include "chessview.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

ChessView::ChessView(QWidget *parent) : QWidget (parent)
{

}

void ChessView::setBoard(ChessBoard *board)
{
    if(m_board == board) return;

    if(m_board) {
        //disconnect all signal-slot connection between m_board &  this
        m_board->disconnect(this);
    }
    m_board = board;
    //connect signals...
    if(board) {
        connect(board, SIGNAL(dataChanged(int,int)),this,SLOT(update()));
        connect(board,SIGNAL(boardReset()),this,SLOT(update()));
    }

    updateGeometry();
}

//Get Field Rect
QRect ChessView::fieldRect(int column, int rank) const
{
    if(!m_board) return QRect();
    const QSize fs = fieldSize();
    QRect fRect = QRect(QPoint((column-1)*fs.width(), (m_board->ranks()-rank)*fs.height()),fs);
    // offset rect by rank symbols
    int offset = fontMetrics().width('M');
    // 'M' is the widest letter
    return fRect.translated(offset+4, 0);
}

//Transform QPoint to Rank & Column (i.e. QPoint(x,y) => QPoint(c,r))
QPoint ChessView::fieldAt(const QPoint &pt) const
{
    if(!m_board) return QPoint();
    const QSize fs = fieldSize();
    int offset = fontMetrics().width('M') + 4;

    if(pt.x() < offset) return QPoint();
    int c = (pt.x() - offset) / fs.width();
    int r = pt.y() / fs.height();
    if(c < 0  || c >= m_board->columns() || r < 0 || r >= m_board->ranks()) return QPoint();

    return QPoint(c+1,m_board->ranks()-r); //max rank - r
}

//SET piece
void ChessView::setPiece(char type, const QIcon &icon)
{
   m_pieces.insert(type, icon);
   update();
}

//GET piece
QIcon ChessView::piece(char type) const
{
    return m_pieces.value(type, QIcon());
}

//HIGHLIGHT
void ChessView::addHighlight(ChessView::Highlight *hl)
{
    m_highlights.append(hl); update();
}

void ChessView::removeHighlight(ChessView::Highlight *hl)
{
    m_highlights.removeOne(hl); update();
}

QSize ChessView::sizeHint() const
{
    if(!m_board) return QSize(100,100);
    QSize boardSize = QSize(fieldSize().width() * m_board->columns()+1,
                            m_fieldSize.height() * m_board->ranks()+1);
    //Offset for rank/col index
    int rankSize = fontMetrics().width('M')+4;
    int columnSize = fontMetrics().height()+4;
    return boardSize+QSize(rankSize, columnSize);
}

//PAINT EVENT
void ChessView::paintEvent(QPaintEvent *event)
{
    if(!m_board) return;
    QPainter painter(this);
    //Draw rank nums
    for(int r = m_board->ranks(); r>0; --r) {
        painter.save();
        drawRank(&painter, r);
        painter.restore();
    }
    //Draw Col nums
    for(int c=1; c <= m_board->columns(); ++c) {
        painter.save();
        drawColumn(&painter, c);
        painter.restore();
    }
    //Draw field background
    for(int r=1; r<=m_board->ranks();++r) {
        for(int c=1; c<=m_board->columns();++c){
            painter.save();
            drawField(&painter, c, r);
            painter.restore();
        }
    }
    //Draw Highlights
    drawHighlight(&painter);

    //Draw Pieces
    for(int r = m_board->ranks(); r>0; --r) {
        for(int c = 1 ; c <= m_board->columns(); ++c) {
            drawPiece(&painter, c, r);
        }
    }
}

//Mouse Release event (clicked)
void ChessView::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint pt = fieldAt(event->pos());
    if(pt.isNull()) return;
    emit clicked(pt);
}

//Draw Rank number
void ChessView::drawRank(QPainter *painter, int rank)
{
    QRect r = fieldRect(1, rank);
    QRect rankRect = QRect(0, r.top(), r.left(), r.height()).adjusted(2, 0, -2, 0);
    QString rankedText = QString::number(rank);
    painter->drawText(rankRect, Qt::AlignVCenter | Qt::AlignRight, rankedText);
}

//Draw Column number
void ChessView::drawColumn(QPainter *painter, int column)
{
    QRect r = fieldRect(column, 1);
    QRect columnRect = QRect(r.left(), r.bottom(), r.width(), height() - r.bottom()).adjusted(0, 2, 0, -2);
    painter->drawText(columnRect, Qt::AlignHCenter | Qt::AlignTop, QChar('a'+column-1));
}

//Draw Field Background
void ChessView::drawField(QPainter *painter, int column, int rank)
{
    QRect rect = fieldRect(column, rank);
    QColor fillColor = (column+rank) % 2 ? palette().color(QPalette::Light) : palette().color(QPalette::Mid);
    painter->setPen(palette().color(QPalette::Dark));
    painter->setBrush(fillColor);
    painter->drawRect(rect);
}

void ChessView::drawPiece(QPainter *painter, int column, int rank)
{
    QRect rect = fieldRect(column, rank);
    char value = m_board->data(column, rank);
    if(value != ' ') {
        QIcon icon = piece(value);
        if(!icon.isNull()) {
            icon.paint(painter, rect, Qt::AlignCenter);
        } else {    //Error!
            qDebug() << "Error! QIcon at (" << rank <<","<<column<<") is invalid!";
        }
    } else {    //Error - value == ' '
            //qDebug() << "Error! value at (" << rank <<","<<column<<") is invalid/empty!";
    }
}

void ChessView::drawHighlight(QPainter *painter)
{
    for(int idx=0; idx < highlightCount(); ++idx) {
        Highlight *hl = highlight(idx);
        if(hl->type() == FieldHighlight::Type) {
            FieldHighlight *fhl = static_cast<FieldHighlight*>(hl);
            QRect rect = fieldRect(fhl->column(), fhl->rank());
            painter->fillRect(rect, fhl->color());
        }
    }
}
