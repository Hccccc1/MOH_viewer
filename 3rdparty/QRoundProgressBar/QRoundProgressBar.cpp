/*
 * QRoundProgressBar - a circular progress bar Qt widget.
 *
 * Sintegrial Technologies (c) 2015-now
 *
 * The software is freeware and is distributed "as is" with the complete source codes.
 * Anybody is free to use it in any software projects, either commercial or non-commercial.
 * Please do not remove this copyright message and remain the name of the author unchanged.
 *
 * It is very appreciated if you produce some feedback to us case you are going to use
 * the software.
 *
 * Please send your questions, suggestions, and information about found issues to the
 *
 * sintegrial@gmail.com
 *
 */


#include "QRoundProgressBar.h"

#include <QtGui/QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QtMath>

#include <QInputDialog>

QRoundProgressBar::QRoundProgressBar(QWidget *parent) :
    QWidget(parent),
    m_min(0), m_max(100),
    m_value(0),
    m_nullPosition(PositionTop),
    m_barStyle(StyleDonut),
    m_outlinePenWidth(1),
    m_dataPenWidth(1),
    m_rebuildBrush(false),
    m_format("%p%"),
    m_decimals(1),
    m_updateFlags(UF_PERCENT)
{
}
#if 0
void QRoundProgressBar::mousePressEvent(QMouseEvent *event)
{
//   qDebug() << event->localPos().x();
    double outer_radius = qMin(this->width(), this->height());
    double inner_radius = outer_radius * 0.75;

    double x = event->x() , y = event->y();

    double point_radius = qSqrt(qPow(x-50, 2)+qPow(y-50, 2));
    double angle = 0;

    if (point_radius > inner_radius/2 && point_radius < outer_radius/2)
    {
        if( x == 50.0 )
        {
            if (y < 50)
                angle = 0;
            else
                angle = 180;
        }
        else if ( y == 50.0 )
        {
            if ( x < 50 )
                angle = 270;
            else
                angle = 90;
        }
        else if (x < 50 && y < 50)
        {
            angle = qAtan((50-y)/(50-x)) / M_PI * 180 + 270;
        }
        else if (x > 50 && y < 50)
        {
            angle = 90 - qAtan((50-y)/(x-50)) / M_PI * 180;
        }
        else if (x > 50 && y > 50)
        {
            angle = qAtan((50-y)/(50-x)) / M_PI * 180 + 90;
        }
        else if (x < 50 && y > 50)
        {
            angle = 270 - qAtan((y-50)/(50-x)) / M_PI * 180;
        }

        setValue(angle/360*100);

        double value = (angle/360*100 - m_min) / (m_max - m_min) * 100;

        emit barValueChanged(value);

        update();
    }
}
#endif

void QRoundProgressBar::setRange(double min, double max)
{
    m_min = min;
    m_max = max;

    if (m_max < m_min)
        qSwap(m_max, m_min);

    if (m_value < m_min)
        m_value = m_min;
    else if (m_value > m_max)
        m_value = m_max;

    if (!m_gradientData.isEmpty())
        m_rebuildBrush = true;

    update();
}

void QRoundProgressBar::setMinimum(double min)
{
    setRange(min, m_max);
}

void QRoundProgressBar::setMaximum(double max)
{
    setRange(m_min, max);
}

void QRoundProgressBar::setValue(double val)
{
    if (m_value != val)
    {
        if (val < m_min)
            m_value = m_min;
        else if (val > m_max)
            m_value = m_max;
        else
            m_value = val;

        update();
    }
}

void QRoundProgressBar::setValue(int val)
{
    setValue(double(val));
}

void QRoundProgressBar::setNullPosition(double position)
{
    if (position != m_nullPosition)
    {
        m_nullPosition = position;

        if (!m_gradientData.isEmpty())
            m_rebuildBrush = true;

        update();
    }
}

void QRoundProgressBar::setBarStyle(QRoundProgressBar::BarStyle style)
{
    if (style != m_barStyle)
    {
        m_barStyle = style;

        update();
    }
}

void QRoundProgressBar::setOutlinePenWidth(double penWidth)
{
    if (penWidth != m_outlinePenWidth)
    {
        m_outlinePenWidth = penWidth;

        update();
    }
}

void QRoundProgressBar::setDataPenWidth(double penWidth)
{
    if (penWidth != m_dataPenWidth)
    {
        m_dataPenWidth = penWidth;

        update();
    }
}

void QRoundProgressBar::setDataColors(const QGradientStops &stopPoints)
{
    if (stopPoints != m_gradientData)
    {
        m_gradientData = stopPoints;
        m_rebuildBrush = true;

        update();
    }
}

void QRoundProgressBar::setFormat(const QString &format)
{
    if (format != m_format)
    {
        m_format = format;

        valueFormatChanged();
    }
}

void QRoundProgressBar::resetFormat()
{
    m_format = QString();

    valueFormatChanged();
}

void QRoundProgressBar::setDecimals(int count)
{
    if (count >= 0 && count != m_decimals)
    {
        m_decimals = count;

        valueFormatChanged();
    }
}

void QRoundProgressBar::paintEvent(QPaintEvent* /*event*/)
{
    double outerRadius = qMin(width(), height());
    QRectF baseRect(1, 1, outerRadius-2, outerRadius-2);

    QImage buffer(outerRadius, outerRadius, QImage::Format_ARGB32_Premultiplied);

    QPainter p(&buffer);
    p.setRenderHint(QPainter::Antialiasing);

    // data brush
    rebuildDataBrushIfNeeded();

    // background
    drawBackground(p, buffer.rect());

    // base circle
    drawBase(p, baseRect);

    // data circle
    double arcStep = 360.0 / (m_max - m_min) * m_value;
    drawValue(p, baseRect, m_value, arcStep);

    // center circle
    double innerRadius(0);
    QRectF innerRect;
    calculateInnerRect(baseRect, outerRadius, innerRect, innerRadius);
    drawInnerBackground(p, innerRect);

    // text
    drawText(p, innerRect, innerRadius, m_value);

    // finally draw the bar
    p.end();

    QPainter painter(this);
    painter.fillRect(baseRect, palette().window());
    painter.drawImage(0,0, buffer);
}

void QRoundProgressBar::drawBackground(QPainter &p, const QRectF &baseRect)
{
    p.fillRect(baseRect, palette().window());
}

void QRoundProgressBar::drawBase(QPainter &p, const QRectF &baseRect)
{
    switch (m_barStyle)
    {
    case StyleDonut:
        p.setPen(QPen(palette().shadow().color(), m_outlinePenWidth));
        p.setBrush(palette().base());
        p.drawEllipse(baseRect);
        break;

    case StylePie:
        p.setPen(QPen(palette().base().color(), m_outlinePenWidth));
        p.setBrush(palette().base());
        p.drawEllipse(baseRect);
        break;

    case StyleLine:
        p.setPen(QPen(palette().base().color(), m_outlinePenWidth));
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(baseRect.adjusted(m_outlinePenWidth/2, m_outlinePenWidth/2, -m_outlinePenWidth/2, -m_outlinePenWidth/2));
        break;

    default:;
    }

}

void QRoundProgressBar::drawValue(QPainter &p, const QRectF &baseRect, double value, double arcLength)
{
    // nothing to draw
    if (value == m_min)
        return;

    // for Line style
    if (m_barStyle == StyleLine)
    {
        p.setPen(QPen(palette().highlight().color(), m_dataPenWidth));
        p.setBrush(Qt::NoBrush);
        p.drawArc(baseRect.adjusted(m_outlinePenWidth/2, m_outlinePenWidth/2, -m_outlinePenWidth/2, -m_outlinePenWidth/2),
                  m_nullPosition * 16,
                  -arcLength * 16);
        return;
    }

    // for Pie and Donut styles
    QPainterPath dataPath;
    dataPath.setFillRule(Qt::WindingFill);

    // pie segment outer
    dataPath.moveTo(baseRect.center());
    dataPath.arcTo(baseRect, m_nullPosition, -arcLength);
    dataPath.lineTo(baseRect.center());

    p.setBrush(palette().highlight());
    p.setPen(QPen(palette().shadow().color(), m_dataPenWidth));
    p.drawPath(dataPath);
}

void QRoundProgressBar::calculateInnerRect(const QRectF &/*baseRect*/, double outerRadius, QRectF &innerRect, double &innerRadius)
{
    // for Line style
    if (m_barStyle == StyleLine)
    {
        innerRadius = outerRadius - m_outlinePenWidth;
    }
    else    // for Pie and Donut styles
    {
        innerRadius = outerRadius * 0.75;
    }

    double delta = (outerRadius - innerRadius) / 2;
    innerRect = QRectF(delta, delta, innerRadius, innerRadius);
}

void QRoundProgressBar::drawInnerBackground(QPainter &p, const QRectF &innerRect)
{
    if (m_barStyle == StyleDonut)
    {
        p.setBrush(palette().alternateBase());
        p.drawEllipse(innerRect);
    }
}

void QRoundProgressBar::drawText(QPainter &p, const QRectF &innerRect, double innerRadius, double value)
{
    if (m_format.isEmpty())
        return;

    // !!! to revise
    QFont f(font());
    f.setPixelSize(innerRadius * qMax(0.05, (0.35 - (double)m_decimals * 0.08)));
    p.setFont(f);

    QRectF textRect(innerRect);
    p.setPen(palette().text().color());
    p.drawText(textRect, Qt::AlignCenter, valueToText(value));
}

QString QRoundProgressBar::valueToText(double value) const
{
    QString textToDraw(m_format);

    if (m_updateFlags & UF_VALUE)
        textToDraw.replace("%v", QString::number(value, 'f', m_decimals));

    if (m_updateFlags & UF_PERCENT)
    {
        double procent = (value - m_min) / (m_max - m_min) * 100.0;
        textToDraw.replace("%p", QString::number(procent, 'f', m_decimals));
    }

    if (m_updateFlags & UF_MAX)
        textToDraw.replace("%m", QString::number(m_max - m_min + 1, 'f', m_decimals));

    return textToDraw;
}

void QRoundProgressBar::valueFormatChanged()
{
    m_updateFlags = 0;

    if (m_format.contains("%v"))
        m_updateFlags |= UF_VALUE;

    if (m_format.contains("%p"))
        m_updateFlags |= UF_PERCENT;

    if (m_format.contains("%m"))
        m_updateFlags |= UF_MAX;

    update();
}

void QRoundProgressBar::rebuildDataBrushIfNeeded()
{
    if (m_rebuildBrush)
    {
        m_rebuildBrush = false;

        QConicalGradient dataBrush;
        dataBrush.setCenter(0.5,0.5);
        dataBrush.setCoordinateMode(QGradient::StretchToDeviceMode);

        // invert colors
        for (int i = 0; i < m_gradientData.count(); i++)
        {
            dataBrush.setColorAt(1.0 - m_gradientData.at(i).first, m_gradientData.at(i).second);
        }

        // angle
        dataBrush.setAngle(m_nullPosition);

        QPalette p(palette());
        p.setBrush(QPalette::Highlight, dataBrush);
        setPalette(p);
    }
}

void QRoundProgressBar::mouseDoubleClickEvent(QMouseEvent *)
{
//    qobject_cast<QRoundProgressBar>
    bool ok;
    double value = QInputDialog::getDouble(this, "输入", "需要修改的值：", this->value(), 0.0, 100.0, 1, &ok, Qt::Dialog, 0.1);

    setValue(value);

//    qDebug() << __FILE__ << __LINE__ << value;

    emit barValueChanged(value);

    update();
}
