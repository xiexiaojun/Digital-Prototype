#include "diagramitem.h"
#include "arrow.h"
#include "radarscene.h"
#include<QGraphicsScene>
#include <QPainter>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsBlurEffect>
/**
* @projectName   prototype_v0719
* @brief         这里就是悬浮框中雷达内部组件的图表类。
* @author        Antrn
* @date          2019-08-12
*/

//DiagramItem::DiagramItem(DiagramType diagramType, QMenu *contextMenu,
//             QGraphicsItem *parent): QGraphicsPolygonItem(parent)
//{
//    myDiagramType = diagramType;
DiagramItem::DiagramItem(QString iconName, QMenu *contextMenu, QGraphicsItem *parent)
: QGraphicsPolygonItem(parent), iconName(iconName)
{
    myContextMenu = contextMenu;
    QPainterPath path;

    //这里元素的大小和在场景中是一样的，修改了都会变。
//    switch (myDiagramType) {
        //暂不使用
//        case Comp3:
//            path.moveTo(200, 50);
//            path.arcTo(150, 0, 50, 50, 0, 90);
//            path.arcTo(50, 0, 50, 50, 90, 90);
//            path.arcTo(50, 50, 50, 50, 180, 90);
//            path.arcTo(150, 50, 50, 50, 270, 90);
//            path.lineTo(200, 25);
//            myPolygon = path.toFillPolygon();
        //统一形状
//            myPolygon << QPointF(-50, -50) << QPointF(50, -50)
//                      << QPointF(50, 50) << QPointF(-50, 50)
//                      << QPointF(-50, -50);
//            break;
//        立着正方形
//        case Comp2:
//            myPolygon << QPointF(-50, 0) << QPointF(0, 50)
//                      << QPointF(50, 0) << QPointF(0, -50)
//                      << QPointF(-50, 0);

        //统一形状
//            myPolygon << QPointF(-50, -50) << QPointF(50, -50)
//                      << QPointF(50, 50) << QPointF(-50, 50)
//                      << QPointF(-50, -50);
//            break;
        //正方形
//        case Comp1:
//            path.moveTo(-50, -50);
//            path.addText(QPointF(0,0),QFont("Helvetica", 20),QString("hhh"));
//            path.moveTo(50, -50);
//            path.lineTo(50, -50);
//            path.lineTo(50, 50);
//            path.lineTo(-50, 50);
//            path.lineTo(-50, -50);

        //统一形状
//            myPolygon << QPointF(-50, -50) << QPointF(50, -50)
//                      << QPointF(50, 50) << QPointF(-50, 50)
//                      << QPointF(-50, -50);
//            myPolygon = path.toFillPolygon();
//            break;
        //默认 菱形，IO
//        default:
//            myPolygon << QPointF(-60, -40) << QPointF(-35, 40)
//                      << QPointF(60, 40) << QPointF(35, -40)
//                      << QPointF(-60, -40);
        //统一形状
//            myPolygon << QPointF(-50, -50) << QPointF(50, -50)
//                      << QPointF(50, 50) << QPointF(-50, 50)
//                      << QPointF(-50, -50);
//            break;
//    }

    // 统一形状
    myPolygon << QPointF(-50, -50) << QPointF(50, -50)
          << QPointF(50, 50) << QPointF(-50, 50)
          << QPointF(-50, -50);
    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
    // 坐标变化
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

QRectF DiagramItem::boundingRect()
{
    qreal adjust=0.5;
    return QRectF(-50-adjust,50-adjust,100+adjust,100+adjust);
}

void DiagramItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->drawRect(-50,-50,100,100);

    // NOTE BUG 修改资源图片位置，为什么这里不能加后缀，试了两小时，又变成加不加后缀都可以了。。。。
//    painter->drawPixmap(-49,-49,98,98, QPixmap(":/img/"+iconName));

    QString s = QDir::currentPath()+"/images/"+iconName;
    painter->drawPixmap(-49,-49,98,98, QPixmap(s));
//    qDebug() << "名字！！！" << s;

//    myDiagramType = diagramType();
//    switch (myDiagramType) {
//        case Comp1: painter->drawPixmap(-49,-49,98,98, QPixmap(":/img/FDPC.png"));
//            break;
//        case Comp2: painter->drawPixmap(-49,-49,98,98, QPixmap(":/img/CFAR.png"));
//            break;
//        case Comp4: painter->drawPixmap(-49,-49,98,98, QPixmap(":/img/MTD.png"));
//            break;
//        case Comp3: painter->drawPixmap(-49,-49,98,98, QPixmap(":/img/input.png"));
//            break;
//        case Comp5: painter->drawPixmap(-49,-49,98,98, QPixmap(":/img/output.png"));
//            break;
//    }
}
void DiagramItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "hover 进入";
    setCursor(Qt::OpenHandCursor);
    event->accept();
}

void DiagramItem::removeArrow(Arrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

void DiagramItem::removeArrows()
{
    foreach (Arrow *arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        //RadarScene *sce = dynamic_cast<RadarScene*>(this->scene());
        //sce->idList.removeOne(arrow->itemId);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void DiagramItem::addArrow(Arrow *arrow)
{
    arrows.append(arrow);
}

// This is an auto-generated comment.
/**
* @projectName   prototype_v0906
* @brief         简介 早期版本代码，用于列表中展示
* @author        Antrn
* @date          2019-11-08
*/
//QPixmap DiagramItem::image() const
//{
//    QPixmap pixmap(250, 250);
////    pixmap.fill(Qt::transparent);
////    QString iconName;
////    switch (diagramType()) {
////        case DiagramItem::DiagramType::Comp1 :
////            iconName = "FDPC";
////            break;
////        case DiagramItem::DiagramType::Comp2 :
////            iconName = "CFAR";
////            break;
////        case DiagramItem::DiagramType::Comp4 :
////            iconName = "MTD";
////            break;
////        case DiagramItem::DiagramType::Comp3 :
////            iconName = "INPUT";
////            break;
////        case DiagramItem::DiagramType::Comp5 :
////            iconName = "OUTPUT";
////            break;
////    }
//    QString itemIcon = ":/img/" +iconName+".ico";
//    pixmap.convertFromImage(QImage(itemIcon));
//    QPainter painter(&pixmap);
//    painter.setPen(QPen(Qt::black, 8));
//    painter.translate(125, 125);
////    painter.drawPolyline(myPolygon);
//    painter.setRenderHint(QPainter::Antialiasing, true);
//    return pixmap;
//}

QString DiagramItem::getIconName() const
{
    return iconName;
}

void DiagramItem::setIconName(const QString &value)
{
    iconName = value;
}

//void DiagramItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
//{
////    clearFocus();
//    setCursor(Qt::OpenHandCursor);
//    QGraphicsSceneMouseEvent();
//}

//void DiagramItem::mousePressEvent(QGraphicsSceneMouseEvent *e)
//{
//    clearFocus();
//    setFocus();
//    // 当移动的时候改变光标
//    setCursor(Qt::ClosedHandCursor);
//    QGraphicsSceneMouseEvent();
//}

//void DiagramItem::KeyPressEvent(QKeyEvent *event)
//{
//    if(event->key() == Qt::Key_Down){
//        moveBy(0, 10);
          //？为什么不是想象中的移动图形项，而是scene
//    }
//    switch (event->key()) {
//        case Qt::Key_1:{
//            QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect;
//            blurEffect->setBlurHints(QGraphicsBlurEffect::QualityHint);
//            blurEffect->setBlurRadius(8);
//            setGraphicsEffect(blurEffect);
//            break;
//        }
//        case Qt::Key_5:{
//            graphicsEffect()->setEnabled(false);
//            break;
//        }
//    }
//}

//void DiagramItem::hoverEnterEvent(QGraphicsSceneHoverEvent *)
//{
//    setCursor(Qt::OpenHandCursor);
//    setToolTip("component");
//}


//右键菜单，保持一致
void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    // 菜单可用
    myContextMenu->setEnabled(true);
    qDebug() << "myContextMenu设置为true" << myContextMenu->isEnabled();
    // 鼠标是不是在场景里
    bool is = dynamic_cast<RadarScene*>(this->scene())->isSelected;
    if(!is){
        dynamic_cast<RadarScene*>(this->scene())->setIsSelected(true);
        qDebug() << "isSelected设置为true";
    };
    myContextMenu->exec(event->screenPos());
}

// This is an auto-generated comment.
/**
* @projectName   prototype_v0906
* @brief         简介 位置变化
* @author        Antrn
* @date          2019-10-07
*/
QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    //检测位置发生变化
    if (change == QGraphicsItem::ItemPositionChange) {
        if(this->init_pos_set != 0){
//            qDebug() << "现在的位置： " << this->pos();
            // 获取最新位置，更新doc
            RadarScene *scene = dynamic_cast<RadarScene *>(this->scene());
            scene->updateXmlItemsPos(this->pos(), this);
        }else{
            // 初始化的时候，不用更新位置
            init_pos_set = 1;
        }
        foreach (Arrow *arrow, arrows) {
            arrow->updatePosition();
        }
    }
//    update();
    return value;
}

void DiagramItem::focusInEvent(QFocusEvent *)
{
    qDebug() << "item focus in";
    bool is = dynamic_cast<RadarScene*>(this->scene())->isSelected;
    if(!is){
        dynamic_cast<RadarScene*>(this->scene())->setIsSelected(true);
        qDebug() << "isSelected设置为true";
    };
    myContextMenu->setEnabled(true);
}

void DiagramItem::focusOutEvent(QFocusEvent *)
{
    qDebug() << "item focus out";
    dynamic_cast<RadarScene*>(this->scene())->setIsSelected(false);
    qDebug() << "isSelected设置为false";
    myContextMenu->setEnabled(false);
}
