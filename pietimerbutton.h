/**
MIT License

Copyright (c) 2019 Brett Sackstein

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef PIETIMERBUTTON_H
#define PIETIMERBUTTON_H

#include <QPainter>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QPaintEvent>

class PieTimerButton : public QPushButton{
    Q_OBJECT

    Q_PROPERTY(QColor m_loadingColor  READ getLoadingColor  WRITE setLoadingColor)
    Q_PROPERTY(int    m_loadingAngle  READ getLoadingAngle  WRITE setLoadingAngle)
public:
    PieTimerButton(QColor backgroundColor, QColor loadingColor, int duration = 5000, QWidget* parent = nullptr);

    void setLoadingColor(QColor color);

    void setBackgroundColor(QColor color);

    void setAnimationDuration(int msecDuration);

public slots:
    void reset();

private:
    int getLoadingAngle();
    void setLoadingAngle(int angle);
    void sweepAnimation();

    QColor getLoadingColor();

    void paintEvent(QPaintEvent* e);
    void fadeOut();

private slots:
    void fadeFinishied();

private:
    QColor m_backgroundColor;
    QColor m_loadingColor;
    QColor m_originalLoadingColor;

    int m_loadingAngle;
    int m_animationDuration;

    QPropertyAnimation* m_transitionAngle;
    QPropertyAnimation* m_fadeColor;

    bool m_fadingOut;
};

#endif // PIETIMERBUTTON_H
