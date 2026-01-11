#pragma once
#include <QtWidgets/QWidget>

class Led : public QWidget
{
    Q_OBJECT

public:
    enum Color {
        Red,
        Green,
        Blue,
        Yellow
    };

    explicit Led(Color color = Red, int x = 0, int y = 0, int width = 20, int height = 20, QWidget *parent = nullptr);
    ~Led();

    // Set the LED state
    void setState(bool on);
    bool state() const { return _isOn; }

    // Get the LED color
    Color color() const { return _color; }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Color _color;
    bool _isOn;
    int _x, _y, _width, _height;

    QString createSvgContent() const;
    QColor getLedColor() const;
    QColor getGlowColor() const;
};
