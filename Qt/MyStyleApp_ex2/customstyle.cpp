#include "customstyle.h"

/*CustomStyle::CustomStyle()
{

}*/

void CustomStyle::polish(QWidget* pwgt)
{
    if(qobject_cast<QPushButton*>(pwgt)) {
        pwgt->setAttribute(Qt::WA_Hover, true);
    }
}

void CustomStyle::unpolish(QWidget* pwgt)
{
    if(qobject_cast<QPushButton*>(pwgt)) {
        pwgt->setAttribute(Qt::WA_Hover, false);
    }
}

void CustomStyle::drawPrimitive(PrimitiveElement elem, const QStyleOption *popt, QPainter *ppainter, const QWidget *pwgt) const
{
    switch(elem){
    case PE_PanelButtonCommand:
        {
          const QStyleOptionButton* pOptionButton =
                  qstyleoption_cast<const QStyleOptionButton*>(popt);
          if(pOptionButton) {
              bool bDown = (pOptionButton->state & State_Sunken)
                      || (pOptionButton->state & State_On);

              QPixmap pix = bDown ? QPixmap("D:/Qt_Projects/MyStyleApp_ex2/bmp-icon.png") :
                                    QPixmap("D:/Qt_Projects/MyStyleApp_ex2/bmp-icon2.png");

              ppainter->drawPixmap(pOptionButton->rect, pix);

              bool bHover = (pOptionButton->state & State_Enabled)
                      && (pOptionButton->state & State_MouseOver);

              if(bHover) {
                  ppainter->fillRect(pOptionButton->rect,
                                     QColor(25,97,45,38));
              }
          }
          break;
        }

    default:
        QCommonStyle::drawPrimitive(elem,popt,ppainter,pwgt);
        break;
    }
    return;
}
