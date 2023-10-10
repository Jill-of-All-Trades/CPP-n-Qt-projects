#ifndef CUSTOMSTYLE_H
#define CUSTOMSTYLE_H

#include <QtWidgets>


class QPainter;

class CustomStyle : public QCommonStyle
{
public:
    //CustomStyle();
    virtual void polish(QWidget* pwgt); //Изменение кнопки при Hover накрытии мыши
    virtual void unpolish(QWidget* pwgt);

    virtual void drawPrimitive( PrimitiveElement elem,
                                const QStyleOption* popt,
                                QPainter* ppainter,
                                const QWidget* pwgt =0) const;
};

#endif // CUSTOMSTYLE_H
