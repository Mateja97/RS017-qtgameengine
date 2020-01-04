#include "player.h"
#include <QDebug>

#define BOOST (10)

Player::Player(qreal x, qreal y, qreal width, qreal height, QString look,
               QToolBox* componentInfo,QList<QLineEdit*> playerInfo, QPushButton* playerUpdate)
    : GameComponent(x, y, width, height)
    ,lookRight(look)
    ,componentInfo(componentInfo)
    ,playerInfo(playerInfo)
    ,playerUpdate(playerUpdate)
    ,movementArray(5)
    ,jumpEnabled(true)
    ,gravityEnabled(true)
    ,moveUpDownEnabled(true)
{
    setFlags(ItemIsMovable|ItemIsFocusable);
    lookLeft = lookRight.transformed(QTransform().scale(-1,1));
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    playerInfo.at(0)->setPlaceholderText(QString::number(this->pos().rx()));
    playerInfo.at(1)->setPlaceholderText(QString::number(this->pos().ry()));
    painter->drawPixmap(0, 0, width, height, this->isRight ? lookRight : lookLeft);
}


void Player::move(qreal delta_x, qreal delta_y)
{
    if(movementArray[0] && moveUpDownEnabled){
        if(movementArray[4])
            y -= BOOST;
        else
            y -= 4;
    }

    if(movementArray[1]){
        if(movementArray[4])
            x -= BOOST;
        else
            x -= 4;
    }

    if(movementArray[2] && moveUpDownEnabled){
        if(movementArray[4])
            y += BOOST;
        else
            y += 4;
    }

    if(movementArray[3]){
        if(movementArray[4])
            x += BOOST;
        else
            x += 4;
    }

    this->setPos(x, y);
    update();

//    x+=delta_x;
//    y+=delta_y;
//    this->setPos(x, y);
//    qDebug()<<this->pos();
//    update();
}

void Player::setMoveUpDownEnabled(bool checked)
{
    this->moveUpDownEnabled = checked;
}

void Player::setGravityEnabled(bool checked)
{
    this->gravityEnabled = checked;
}


qreal Player::getHeight()
{
    return this->height;
}

qreal Player::getWidth()
{
    return this->width;
}

void Player::setCurrentLook(bool right)
{
    this->isRight = right;
}

void Player::pbApply()
{
    this->x = (qreal)playerInfo.at(0)->text().toFloat();
    this->y = (qreal)playerInfo.at(1)->text().toFloat();
    this->setPos((qreal)playerInfo.at(0)->text().toFloat(), (qreal)playerInfo.at(1)->text().toFloat());
    this->width = (qreal)playerInfo.at(2)->text().toFloat();
    this->height = (qreal)playerInfo.at(3)->text().toFloat();
    update();
}
