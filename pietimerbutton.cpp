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

#include "pietimerbutton.h"

PieTimerButton::PieTimerButton(QColor backgroundColor, QColor loadingColor, int duration, QWidget *parent)
    : QPushButton(parent), m_backgroundColor(backgroundColor), m_loadingColor(loadingColor),
      m_originalLoadingColor(loadingColor), m_loadingAngle(0), m_animationDuration(duration), m_fadingOut(false){

    m_transitionAngle = new QPropertyAnimation(this, "m_loadingAngle");
    m_fadeColor = new QPropertyAnimation(this, "m_loadingColor");

    connect(this,              &QPushButton::clicked,             this, &PieTimerButton::sweepAnimation, Qt::DirectConnection);
    connect(m_transitionAngle, &QPropertyAnimation::valueChanged, this, [this](){ update(); },           Qt::DirectConnection);
    connect(m_fadeColor,       &QPropertyAnimation::valueChanged, this, [this](){ update(); },           Qt::DirectConnection);
    connect(m_fadeColor,       &QPropertyAnimation::finished,     this, &PieTimerButton::fadeFinishied,  Qt::DirectConnection);
}

int PieTimerButton::getLoadingAngle(){
    return m_loadingAngle;
}

void PieTimerButton::setLoadingAngle(int angle){
    m_loadingAngle = angle;
}

QColor PieTimerButton::getLoadingColor(){
    return m_loadingColor;
}

void PieTimerButton::setLoadingColor(QColor color){
    m_loadingColor = color;
}

void PieTimerButton::setBackgroundColor(QColor color){
    m_backgroundColor = color;
}

void PieTimerButton::setAnimationDuration(int msecDuration){
    if(msecDuration <= 0){
        throw std::invalid_argument("Error. PieTimerButton::setAnimationDuration called with a msecDuration <= , must be > 0");
    }

    m_animationDuration = msecDuration;
}

//Hook for user to stop the button and reset to its defaults
void PieTimerButton::reset(){
    m_transitionAngle->stop();
    m_fadeColor->stop();

    m_fadingOut = false;

    m_loadingAngle = 0;

    m_loadingColor = m_originalLoadingColor;
}

//Does the sweeping animation for 90% of the duration
void PieTimerButton::sweepAnimation(){
    m_fadingOut = false;

    if(m_transitionAngle->state() == QPropertyAnimation::State::Running){
        m_transitionAngle->stop();
    }

    m_transitionAngle->setStartValue(0);

    //We stop at 360 degrees
    m_transitionAngle->setEndValue(360);

    //We keep the duration as set, but give it a nice fade out to make it visually appealing and not jarring
    m_transitionAngle->setDuration(m_animationDuration * 0.9);

    m_transitionAngle->start();
}

//Fades the widget out for the final 10% of the duration
void PieTimerButton::fadeOut(){
    m_fadingOut = true;

    if(m_fadeColor->state() == QPropertyAnimation::State::Running){
        m_fadeColor->stop();
    }

    m_fadeColor->setStartValue(m_loadingColor);
    //We make it go a bit longer to look more correct
    m_fadeColor->setEndValue(QColor(m_loadingColor.red(), m_loadingColor.green(), m_loadingColor.blue(), 0));
    m_fadeColor->setDuration(0.1 * m_animationDuration);

    m_fadeColor->start();
}

//Cleans up the end of the animation for the next iteration of the iteration to begin
void PieTimerButton::fadeFinishied(){
    m_loadingColor = m_originalLoadingColor;
    m_loadingAngle = 0;
    m_fadingOut = false;
    update();
}

//Where all the magic happens - uses the registered Q_PROPERTY members to animate the paint state
void PieTimerButton::paintEvent(QPaintEvent* e){
    QPainter painter(this);

    QRectF size = QRectF(0, 0, width(), height());

    //This seems to be a good size centered in the middle of the button and sweeps around such that the corners get covered
    QRectF pieSize = QRectF((-width() * 1.50)/6, (-height() * 1.50)/6, width() * 1.5, height() * 1.5);

    painter.setBrush(m_backgroundColor);
    painter.drawRect(size);

    if(m_transitionAngle->state() == QPropertyAnimation::State::Running && m_loadingAngle <= 359){
        painter.setBrush(m_loadingColor);
        //This will draw start from the 9 O-clock position and wrap around all 360 degrees in a clockwise fashion
        painter.drawPie(pieSize, 180 * 16, -(m_loadingAngle * 16));
    }else if(m_loadingAngle == 360){
        painter.setBrush(m_loadingColor);
        painter.drawEllipse(pieSize);
        if(!m_fadingOut){
            fadeOut();
        }
    }else{
        QPushButton::paintEvent(e);
        return;
    }

    e->accept();
}
