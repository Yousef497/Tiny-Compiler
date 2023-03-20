#include "parsetree.h"
#include "QHBoxLayout"
#include "QGraphicsView"
#include "QGraphicsTextItem"
#include "QLabel"
#include "QGraphicsProxyWidget"
#include "QLineF"
ParseTree::ParseTree(QWidget *parent) : QWidget(parent)
{

    scene = new QGraphicsScene();
//    addEllipseNode(0,0,"Jimmy");
//    addRectangleNode(200,200,"Anssray");
//    addEdge(0,0,200,200);
    QHBoxLayout* hori = new QHBoxLayout;
    view=new QGraphicsView(scene);
    hori->addWidget(view);
    this->setLayout(hori);
}

QGraphicsRectItem* ParseTree::addRectangleNode(int x, int y,QString text)
{
     QGraphicsRectItem* item = new QGraphicsRectItem(x,y,100,50);
     scene->addItem(item);
     Items.append(item);
     QGraphicsProxyWidget *pMyProxy = new QGraphicsProxyWidget(item);
     QLabel *label = new QLabel();
     label->setText(text);
     label->setStyleSheet("background-color: #ffffff");
     pMyProxy->setWidget(label);
     pMyProxy->setPos(item->boundingRect().center()-label->rect().center());
     return item;
}
QGraphicsEllipseItem* ParseTree::addEllipseNode(int x, int y, QString text)
{
    QGraphicsEllipseItem* item = new QGraphicsEllipseItem(x,y,100,50);
    item->setBrush(QBrush(Qt::white));
    scene->addItem(item);
    Items.append(item);
    QGraphicsProxyWidget *pMyProxy = new QGraphicsProxyWidget(item);
    QLabel *label = new QLabel();
    label->setText(text);
    label->setStyleSheet("background-color: #ffffff");
    pMyProxy->setWidget(label);
    pMyProxy->setPos(item->boundingRect().center()-label->rect().center());
    return item;

}

QGraphicsLineItem *ParseTree::addEdgeChild(int x1,int y1,int x2,int y2)
{

    QGraphicsLineItem* line = new QGraphicsLineItem( QLineF( x1+50,y1+50,x2+50,y2) );
    scene->addItem(line);
    Items.append(line);
    return line;

}
QGraphicsLineItem *ParseTree::addEdgeFriend(int x1, int y1, int x2, int y2)
{

    QGraphicsLineItem* line = new QGraphicsLineItem( QLineF( x1+100,y1+25,x2,y2+25) );
    scene->addItem(line);
    Items.append(line);
    return line;

}

void ParseTree::clearTree()
{
//    QGraphicsRectItem* item = new QGraphicsRectItem(-100, -100,100000,100000);
//    item->setBrush(QBrush(Qt::white));
//    scene->addItem(item);

//  scene->clear();
//    qDeleteAll(scene->items());
//    scene->update();
   // scene->setBackgroundBrush(QBrush(Qt::black));
    QList <QGraphicsItem*> itemList = scene->items();
        while(!itemList.isEmpty()){
            delete itemList.first();
            itemList = scene->items();
        }

//    while( ! Items.empty() ) {
//       scene->removeItem( Items.first() );
//       delete Items.first();
//       Items.removeFirst();
//    }
 view->viewport()->update();
}


