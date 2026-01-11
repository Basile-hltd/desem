#include "button.h"
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QSvgRenderer>
#include <QPixmap>

Button::Button(Color color, int x, int y, int width, int height, QWidget *parent)
    : QWidget(parent)
    , _color(color)
    , _currentState(ButtonReleased)
    , _x(x)
    , _y(y)
    , _width(width)
    , _height(height)
    , _observer(nullptr)
{
    setFixedSize(_width, _height);
    move(_x, _y);
    setAutoFillBackground(false); // We'll paint everything ourselves
    setMouseTracking(true); // Enable mouse tracking for hover events
}

Button::~Button()
{
}

bool Button::initialize()
{
    // For Qt implementation, initialization is done in constructor
    return true;
}

bool Button::setObserver(IButtonObserver *observer)
{
    _observer = observer;
    return true;
}

void Button::start()
{
    // Nothing to start for Qt implementation
}

void Button::updateState(ButtonState newState)
{
    if (_currentState != newState) {
        _currentState = newState;
        update(); // Trigger repaint

        // Notify observer
        if (_observer) {
            _observer->onButtonStateChange(_currentState);
        }
    }
}

void Button::paintEvent(QPaintEvent *event)
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

void Button::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event)
    if (_currentState == ButtonReleased) {
        updateState(ButtonHover);
    }
}

void Button::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    if (_currentState == ButtonHover) {
        updateState(ButtonReleased);
    }
}

void Button::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        updateState(ButtonPressed);
    }
}

void Button::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (rect().contains(event->pos())) {
            updateState(ButtonHover); // Go back to hover if still over the button
        } else {
            updateState(ButtonReleased); // Go back to released if moved outside
        }
    }
}

QString Button::createSvgContent() const
{
    QColor buttonColor = getButtonColor();
    QColor backgroundColor = getBackgroundColor();

    // Create SVG for circular button with square background
    QString svg = QString(R"(
        <svg width="%1" height="%2" viewBox="0 0 %1 %2" xmlns="http://www.w3.org/2000/svg">
            <!-- Square background (PCB) -->
            <rect x="0" y="0" width="%1" height="%2" rx="2" ry="2"
                  fill="%3" stroke="#666" stroke-width="1"/>
            <!-- PCB fixing holes -->
            <circle cx="3" cy="3" r="1.5" fill="#000"/>
            <circle cx="%4" cy="3" r="1.5" fill="#000"/>
            <circle cx="3" cy="%5" r="1.5" fill="#000"/>
            <circle cx="%4" cy="%5" r="1.5" fill="#000"/>
    )").arg(_width).arg(_height).arg(backgroundColor.name()).arg(_width - 3).arg(_height - 3);

    // Add circular button
    int buttonRadius = qMin(_width, _height) * 0.35; // Button is 70% of the smaller dimension
    int centerX = _width / 2;
    int centerY = _height / 2;

    svg += QString(R"(
            <!-- Circular button -->
            <circle cx="%1" cy="%2" r="%3"
                    fill="%4" stroke="#333" stroke-width="1"/>
    )").arg(centerX).arg(centerY).arg(buttonRadius).arg(buttonColor.name());

    // Add state-specific effects
    if (_currentState == ButtonPressed) {
        // Pressed effect: darker and slightly smaller
        svg += QString(R"(
            <!-- Pressed effect -->
            <circle cx="%1" cy="%2" r="%3"
                    fill="%4" opacity="0.7"/>
        )").arg(centerX).arg(centerY).arg(buttonRadius * 0.9).arg(buttonColor.darker(150).name());
    } else if (_currentState == ButtonHover) {
        if (_color == Black) {
            // Black button hover: turn grey
            svg += QString(R"(
                <!-- Black button hover: grey effect -->
                <circle cx="%1" cy="%2" r="%3"
                        fill="#808080" opacity="0.8"/>
            )").arg(centerX).arg(centerY).arg(buttonRadius);
        } else {
            // Blue button hover: prominent highlight
            svg += QString(R"(
                <!-- Blue button hover effect -->
                <circle cx="%1" cy="%2" r="%3"
                        fill="%4" opacity="0.6"/>
                <!-- Additional glow ring -->
                <circle cx="%1" cy="%2" r="%5"
                        fill="none" stroke="%4" stroke-width="2" opacity="0.4"/>
            )").arg(centerX).arg(centerY).arg(buttonRadius).arg(buttonColor.lighter(150).name()).arg(buttonRadius + 3);
        }
    }

    svg += "\n        </svg>";
    return svg;
}

QColor Button::getButtonColor() const
{
    switch (_color) {
        case Black: return QColor(0x22, 0x22, 0x22); // Dark gray for black button
        case Blue: return QColor(0x17, 0x91, 0xfc);  // Bright blue
    }
    return QColor(0x66, 0x66, 0x66);
}

QColor Button::getBackgroundColor() const
{
    return QColor(201, 213, 235); // Blueish metal
}
