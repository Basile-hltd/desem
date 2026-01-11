#include "led.h"
#include <QPainter>
#include <QPaintEvent>
#include <QSvgRenderer>
#include <QPixmap>

Led::Led(Color color, int x, int y, int width, int height, QWidget *parent)
    : QWidget(parent)
    , _color(color)
    , _isOn(false)
    , _x(x)
    , _y(y)
    , _width(width)
    , _height(height)
{
    setFixedSize(_width, _height);
    move(_x, _y);
    setAutoFillBackground(false); // We'll paint everything ourselves
}

Led::~Led()
{
}

void Led::setState(bool on)
{
    if (_isOn != on) {
        _isOn = on;
        update(); // Trigger repaint
    }
}

void Led::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Create SVG content based on current state
    QString svgContent = createSvgContent();

    // Render SVG
    QSvgRenderer renderer(svgContent.toUtf8());
    renderer.render(&painter, rect());
}

QString Led::createSvgContent() const
{
    QColor ledColor = getLedColor();
    QColor glowColor = getGlowColor();

    // Create SVG for rectangular LED
    QString svg = QString(R"(
        <svg width="%1" height="%2" viewBox="0 0 %1 %2" xmlns="http://www.w3.org/2000/svg">
            <!-- LED body -->
            <rect x="2" y="2" width="%3" height="%4" rx="3" ry="3"
                  fill="%5" stroke="#333" stroke-width="1"/>
    )").arg(_width).arg(_height).arg(_width - 4).arg(_height - 4).arg(ledColor.name());

    if (_isOn) {
        // Add glow effect for ON state
        svg += QString(R"(
            <!-- Glow effect -->
            <rect x="1" y="1" width="%1" height="%2" rx="4" ry="4"
                  fill="none" stroke="%3" stroke-width="2" opacity="0.6"/>
            <!-- Inner highlight -->
            <rect x="4" y="4" width="%4" height="%5" rx="2" ry="2"
                  fill="%6" opacity="0.4"/>
        )").arg(_width - 2).arg(_height - 2).arg(glowColor.name())
           .arg(_width - 8).arg(_height - 8).arg(ledColor.lighter(150).name());
    } else {
        // Dim appearance for OFF state
        svg += QString(R"(
            <!-- Dim effect for OFF state -->
            <rect x="2" y="2" width="%1" height="%2" rx="3" ry="3"
                  fill="#444" opacity="0.3"/>
        )").arg(_width - 4).arg(_height - 4);
    }

    svg += "\n        </svg>";
    return svg;
}

QColor Led::getLedColor() const
{
    if (!_isOn) {
        // Return dim version for OFF state
        switch (_color) {
            case Red: return QColor(0x44, 0x11, 0x11);
            case Green: return QColor(0x11, 0x44, 0x11);
            case Blue: return QColor(0x11, 0x11, 0x44);
            case Yellow: return QColor(0x44, 0x44, 0x11);
        }
    } else {
        // Return bright version for ON state
        switch (_color) {
            case Red: return QColor(0xff, 0x00, 0x00);
            case Green: return QColor(0x00, 0xff, 0x00);
            case Blue: return QColor(0x00, 0x00, 0xff);
            case Yellow: return QColor(0xff, 0xff, 0x00);
        }
    }
    return QColor(0x66, 0x66, 0x66);
}

QColor Led::getGlowColor() const
{
    switch (_color) {
        case Red: return QColor(0xff, 0x66, 0x66);
        case Green: return QColor(0x66, 0xff, 0x66);
        case Blue: return QColor(0x66, 0x66, 0xff);
        case Yellow: return QColor(0xff, 0xff, 0x66);
    }
    return QColor(0xaa, 0xaa, 0xaa);
}
