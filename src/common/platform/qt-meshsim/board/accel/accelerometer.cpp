#include <cassert>
#include <algorithm>
#include <QtCore/QRect>
#include <QtWidgets/QApplication>
#include <QScreen>
#include <QtWidgets/QWidget>
#include <QtDebug>
#include "accelerometer.h"

namespace board {

QWidget * Accelerometer::_pMainWindow(nullptr);
QPoint Accelerometer::lastSeenMousePosition;

Accelerometer::Accelerometer()
{

}

bool Accelerometer::initialize()
{
    if (_pMainWindow == nullptr)
    {
        QWidgetList widgets = QApplication::allWidgets();

        for (const auto widget : widgets)
        {
            // Find the 'main' widget
            if (!widget->parent())
            {
                _pMainWindow = widget;
                break;
            }
        }
    }
    return (_pMainWindow) ? true : false;
}

// static
void Accelerometer::getAccelerationValues(int16_t & x, int16_t & y, int16_t & z)
{
    //
    // The accelerometer minimum value is -2048 and the maximum value is +2048
    //
    if (!QGuiApplication::screens().empty())
    {
        QRect desktopRect = QGuiApplication::screens().constFirst()->availableGeometry();   // Desktop rectangle
        QRect widgetRect = _pMainWindow->geometry();                                        // Main window rectangle
        QPoint topLeft = widgetRect.topLeft();

        if (topLeft.isNull())
        {
            // Typically the case on Linux and macOS
            topLeft = lastSeenMousePosition;
        }

        // Point for calculations is the center point of the windows title
        QPoint pointOfInterest(topLeft.x() + widgetRect.width()/2,
                               topLeft.y());

        const float slopeX = 4096.0 / desktopRect.size().width();
        const float slopeY = 4096.0 / desktopRect.size().height();

        x = slopeX * abs(pointOfInterest.x()) - 2048;		// Calculate x
        y = slopeY * abs(pointOfInterest.y()) - 2048;		// Calculate y

        // Bring back to valid range
        x = std::max((int32_t)x, -2048); x = std::min((int32_t)x, 2048);
        y = std::max((int32_t)y, -2048); y = std::min((int32_t)y, 2048);

        // z is a mix of x and y
        z = (x + y) / 2;

        // qDebug() << "x:" << x << "y:" << y << "z:" << z;
    }
    else
    {
        x = y = z = 0;
    }
}

void Accelerometer::onLastSeenMousePosition(const QPoint & pos)
{
    lastSeenMousePosition = pos;
}

} // namespace board
