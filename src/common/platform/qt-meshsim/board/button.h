#pragma once
#include <QtWidgets/QWidget>
#include "interfaces/ibutton.h"
#include "interfaces/ibuttonobserver.h"

class Button : public QWidget, public IButton
{
    Q_OBJECT

public:
    enum Color {
        Black,
        Blue
    };

    explicit Button(Color color = Black, int x = 0, int y = 0, int width = 30, int height = 30, QWidget *parent = nullptr);
    ~Button();

    // IButton implementation
    bool initialize() override;
    bool setObserver(IButtonObserver *observer) override;
    void start() override;
    ButtonState state() const override { return _currentState; }

    // Get the button color
    Color color() const { return _color; }

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Color _color;
    ButtonState _currentState;
    int _x, _y, _width, _height;
    IButtonObserver *_observer;

    QString createSvgContent() const;
    QColor getButtonColor() const;
    QColor getBackgroundColor() const;

    void updateState(ButtonState newState);
};
