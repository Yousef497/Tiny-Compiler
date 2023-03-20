#ifndef PARSETREE_H
#define PARSETREE_H

#include <QWidget>
#include "QGraphicsScene"
#include "QGraphicsRectItem"
#include "QGraphicsEllipseItem"
#include "QGraphicsLineItem"
#include "QGraphicsItem"
class ParseTree : public QWidget
{
    Q_OBJECT
private:
    QVector<QGraphicsItem*>Items;
    QGraphicsScene* scene;
    QGraphicsView* view;
public:

    explicit ParseTree(QWidget *parent = nullptr);
    QGraphicsRectItem* addRectangleNode(int x,int y,QString text);
    QGraphicsEllipseItem* addEllipseNode(int x,int y,QString text);
    QGraphicsLineItem *addEdgeChild(int x1,int y1,int x2,int y2);
    QGraphicsLineItem *addEdgeFriend(int x1,int y1,int x2,int y2);
    void clearTree(void);

signals:

};

#endif // PARSETREE_H
