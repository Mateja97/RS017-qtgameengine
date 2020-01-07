#include "gamestart.h"
#include "ui_gamestart.h"

GameStart::GameStart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameStart),
    timer(new QTimer())
{
    ui->setupUi(this);

}
GameStart::~GameStart()
{
    delete ui;
}

QGraphicsView *GameStart::getScene() const
{
    return scene;
}

void GameStart::setScene(QGraphicsView *value)
{
    scene = value;
}
void GameStart::start(){
  this->setFixedSize(1600,1000);
  QRectF exactRect(0, 0, 1600, 1000);
  qreal scaleX;
  scaleX = this->size().width()*1.0/this->scene->size().width()*1.0;
  qreal scaleY;
  scaleY = this->size().height()*1.0/this->scene->size().height()*1.0;
  ui->graphicsView->setSceneRect(exactRect);
  QGraphicsScene* new_scene= new QGraphicsScene(ui->graphicsView);
  for(QGraphicsItem* item:scene->scene()->items())
  {
      qreal oldX;
      qreal oldY;

      if(GameComponent* r = qgraphicsitem_cast<GameComponent*>(item)){
          oldX = r->getX();
          oldY = r->getY();
          r->setX(oldX);
          r->setY(oldY*scaleY);
          r->setPos(oldX,oldY*scaleY);
          new_scene->addItem(r);
      }
      if(MapBuilder* r = qgraphicsitem_cast<MapBuilder*>(item)){
          oldX = r->getX();
          oldY = r->getY();
          r->setX(oldX);
          r->setY(oldY*scaleY);
          r->setPos(oldX,oldY*scaleY);
          new_scene->addItem(r);
     }
  }
  if(gameON->getPlayer() != nullptr){
    connect(&(*timer), SIGNAL(timeout()),this, SLOT(update()));
  }
  timer->start(30);
  QPixmap bkgnd;
  bkgnd.load(getFName());
  bkgnd = bkgnd.scaled(QSize(1600,1000));
  QPalette palette;
  palette.setBrush(QPalette::Base, bkgnd);
   ui->graphicsView->setScene(&(*new_scene));
   ui->graphicsView->setPalette(palette);
   ui->graphicsView->horizontalScrollBar()->hide();
   ui->graphicsView->verticalScrollBar()->hide();

}
void GameStart::closeEvent(QCloseEvent *event)
{
    qreal scaleX;
    scaleX = this->size().width()*1.0/this->scene->size().width()*1.0;
    qreal scaleY;
    scaleY = this->size().height()*1.0/this->scene->size().height()*1.0;
    for(QGraphicsItem* item:ui->graphicsView->scene()->items())
    {
        qreal oldX;
        qreal oldY;
        if(GameComponent* r = qgraphicsitem_cast<GameComponent*>(item)){
            oldX = r->getX();
            oldY = r->getY();
            r->setX(oldX);
            r->setY(oldY/scaleY);
            r->setPos(oldX,oldY/scaleY);
            scene->scene()->addItem(r);
        }
        if(MapBuilder* r = qgraphicsitem_cast<MapBuilder*>(item)){
            oldX = r->getX();
            oldY = r->getY();
            r->setX(oldX);
            r->setY(oldY/scaleY);
            r->setPos(oldX,oldY/scaleY);
            scene->scene()->addItem(r);
        }
    }
    timer->stop();
    event->accept();
}

QString GameStart::getFName() const
{
    return fName;
}

void GameStart::setFName(const QString &value)
{
    fName = value;
}


void GameStart::update()
{
    GameBuilder* gameON = this->getGameON();
    Player* player = this->getGameON()->getPlayer();
    QList<Enemy*> lstEnemy = this->getGameON()->getLstEnemy();
    //player->collidingItems();
    //player->pos();
    foreach(Enemy* e, lstEnemy){
        //qDebug() << e->getRange();
        if(e->getRange() > 0)
            e->move();
    }
    //W-0 A-1 S-2 D-3
    qreal speed = 4;
    if(player->movementArray[4])
        speed *= 2;

    if(gameON->playerCanMove(0,-speed) && player->movementArray[0]){
        player->move(0,-speed);
    }

    else if(gameON->playerCanMove(-speed,0) && player->movementArray[1]){
        player->move(-speed,0);
    }

    else if(gameON->playerCanMove(0,speed) && player->movementArray[2]){
        player->move(0,speed);
    }

    else if(gameON->playerCanMove(speed,0) && player->movementArray[3]){
        player->move(speed,0);
    }
    if(player->pos().ry()+player->getHeight() < this->ui->graphicsView->size().height())
        player->gravity(true);
}
void GameStart::keyReleaseEvent(QKeyEvent *event)
{
    Player* player = this->getGameON()->getPlayer();
    if(event->key() == Qt::Key_D){
        player->movementArray.setBit(3,false);
    }
    if(event->key() == Qt::Key_W){
        player->movementArray.setBit(0,false);
    }
    if(event->key() == Qt::Key_A){
        player->movementArray.setBit(1,false);
    }
    if(event->key() == Qt::Key_S){
        player->movementArray.setBit(2,false);
    }

//    qDebug() << player->movementArray[0] << " " << player->movementArray[1] << " " << player->movementArray[2] << " " << player->movementArray[3];

}

void GameStart::keyPressEvent(QKeyEvent *event)
{

   Player* player = this->getGameON()->getPlayer();

    if(event->key() == Qt::Key_D){
        player->setFocus();
        player->movementArray.setBit(3,true);
        player->setCurrentLook(true);
    }
    if(event->key() == Qt::Key_W){
        player->setFocus();
        player->movementArray.setBit(0,true);
    }
    if(event->key() == Qt::Key_A){
        player->setFocus();
        player->movementArray.setBit(1,true);
        player->setCurrentLook(false);

    }
    if(event->key() == Qt::Key_S){
        player->setFocus();
        player->movementArray.setBit(2,true);
    }

}

GameBuilder *GameStart::getGameON() const
{
    return gameON;
}

void GameStart::setGameON(GameBuilder *value)
{
    gameON = value;
}
