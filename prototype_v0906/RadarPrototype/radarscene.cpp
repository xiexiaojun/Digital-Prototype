#include "radarscene.h"
#include "arrow.h"
#include <QDebug>
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QMetaEnum>
#include <runcompconf.h>
#include <QTimer>
#include "diagramitem.h"
#include "mainwindow_radar.h"
#include "radarcompdraglistwidget.h"
/**
* @projectName   prototype_v0719
* @brief         编辑雷达页面的自定义场景类，上面绘制设计雷达的组件链接。
* @author        Antrn
* @date          2019-08-12
*/
//Q_DECLARE_METATYPE(DiagramItem)
RadarScene::RadarScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
//    myItemType = DiagramItem::Comp1;

    line = nullptr;
    textItem = nullptr;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;

    //添加处理指令即xml头部说明和场景的属性
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    QDomElement root = doc.createElement("components");
    doc.appendChild(root);
    QDomAttr scene_w = doc.createAttribute("width");
    QDomAttr scene_h = doc.createAttribute("height");
    scene_w.setValue(QString::number(width()));
    scene_h.setValue(QString::number(height()));
    root.setAttributeNode(scene_w);
    root.setAttributeNode(scene_h);
    Arrs = doc.createElement("Arrs");
    root.appendChild(Arrs);
    Items = doc.createElement("Items");
    root.appendChild(Items);
}


void RadarScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    //只过滤箭头设置颜色
    if (isItemChange(Arrow::Type)) {
        //一次只能设置一个
        Arrow *item = qgraphicsitem_cast<Arrow *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}

void RadarScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if (isItemChange(DiagramTextItem::Type)) {
        //一次只能设置一个
        DiagramTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
    }
}

void RadarScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    if (isItemChange(DiagramItem::Type)) {
        DiagramItem *item = qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
        // 填充
        item->setBrush(myItemColor);
        // 描边
//        item->setPen(QPen(myItemColor));
    }
}

void RadarScene::setFont(const QFont &font)
{
    myFont = font;

    if (isItemChange(DiagramTextItem::Type)) {
        QGraphicsTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(myFont);
    }
}


void RadarScene::modifyXmlItems(QPointF pos, DiagramItem *item)
{
    QDomElement comp;
//    QMetaObject mo = DiagramItem::staticMetaObject;
//    int index = mo.indexOfEnumerator("DiagramType");
//    QMetaEnum metaEnum = mo.enumerator(index);
//    comp = doc.createElement(metaEnum.valueToKey(item->diagramType()));
    // TODO　以iconName命名节点名称？
    comp = doc.createElement(item->getIconName());
//    switch (item->diagramType()) {
//        case DiagramItem::DiagramType::Comp1:
//            comp = doc.createElement("Comp1");
//            break;
//        case DiagramItem::DiagramType::Comp2:
//            comp = doc.createElement("Comp2");
//            break;
//        case DiagramItem::DiagramType::Comp4:
//            comp = doc.createElement("Comp4");
//            break;
//        case DiagramItem::DiagramType::Comp3:
//            comp = doc.createElement("Comp3");
//            break;
//        case DiagramItem::DiagramType::Comp5:
//            comp = doc.createElement("Comp5");
//            break;
//    }

    QDomElement color = doc.createElement("color");
    QDomAttr posx = doc.createAttribute("pos_x");
    QDomAttr posy = doc.createAttribute("pos_y");
    QDomAttr id = doc.createAttribute("id");
    posx.setValue(QString::number(pos.x()));
    posy.setValue(QString::number(pos.y()));
    id.setValue(QString::number(item->itemId));
    comp.setAttributeNode(posx);
    comp.setAttributeNode(posy);
    comp.setAttributeNode(id);
    QDomText c = doc.createTextNode(itemColor().name());
    color.appendChild(c);
    comp.appendChild(color);
    Items.appendChild(comp);
}

void RadarScene::updateXmlItemsPos(QPointF pos, DiagramItem *item)
{
    QDomNode node = doc.elementsByTagName("Items").at(0).firstChild();
    QDomElement elem;
    // 遍历items标签找到
    while(!node.isNull()){
        if(node.isElement()){
             elem = node.toElement();
             if(elem.attribute("id").compare(QString::number(item->itemId))==0){
                 elem.setAttribute("pos_x", pos.x());
                 elem.setAttribute("pos_y", pos.y());
                 MainWindow_Radar::isSave = false;
//                 qDebug() << "xml由于位置改变而被修改";
                 emit isSave2False(0);
                 return;
             }
        }
        node = node.nextSibling();
    }
    qDebug() << "[warning] 未找到匹配的id=" << item->itemId;
}

void RadarScene::modifyXmlArrows(Arrow *arrow, DiagramItem *startItem, DiagramItem *endItem)
{
    QDomElement arr = doc.createElement("arrow");
    QDomElement color = doc.createElement("color");
    QDomAttr start = doc.createAttribute("start_item_id");
    QDomAttr end = doc.createAttribute("end_item_id");
    QDomAttr id = doc.createAttribute("id");
    start.setValue(QString::number(startItem->itemId));
    end.setValue(QString::number(endItem->itemId));
    id.setValue(QString::number(arrow->itemId));
    arr.setAttributeNode(start);
    arr.setAttributeNode(end);
    arr.setAttributeNode(id);
    QDomText c = doc.createTextNode(myLineColor.name());
    color.appendChild(c);
    arr.appendChild(color);
    Arrs.appendChild(arr);
}


void RadarScene::setMode(Mode mode)
{
    myMode = mode;
}

void RadarScene::setMyItemIconName(QString value)
{
    myItemIconName = value;
}

//void RadarScene::setItemType(DiagramItem::DiagramType type)
//{
//    myItemType = type;
//}
//文本框
void RadarScene::editorLostFocus(DiagramTextItem *item)
{
    //获取光标
    QTextCursor cursor = item->textCursor();
    //取消选中
    cursor.clearSelection();
    item->setTextCursor(cursor);
    //如果里面没有内容，就自动删除吧
    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        //计划删除此对象
        item->deleteLater();
    }
}


// This is available in all editors.
/**
* @projectName   prototype_v0906
* @brief         执行指令
* @author        Antrn
* @date          2019-09-10
*/
void RadarScene::startRunCode()
{
    if(MainWindow_Radar::isSave == false){
//        tr("检测到场景还未保存，是否保存后执行?")
        emit isSave2False("检测到场景还未保存，是否保存后执行?");
    }
    emit startRun();
    // TODO 执行代码程序
    // 先展示出配置窗口
    RunCompConf *run = new RunCompConf();
    run->exec();
    sendRate(100);
    emit overRun();
}

void RadarScene::sendRate(float rate)
{
    emit rateSignal(rate);
}

//鼠标事件
void RadarScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //左键
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    DiagramItem *item;
    switch (myMode) {
        case InsertItem:{
//            item = new DiagramItem(myItemType, myItemMenu);
            item = new DiagramItem(myItemIconName, myItemMenu);
//            item->(myItemColor);
            item->setBrush(myItemColor);
            item->itemId = generateUniqueid();
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            emit itemInserted(item);
            modifyXmlItems(mouseEvent->scenePos(), item);
            break;
        }
        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            addItem(line);
            break;
        case InsertText:
            textItem = new DiagramTextItem();
            textItem->setFont(myFont);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            textItem->setZValue(1000.0);
            connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)),
                    this, SLOT(editorLostFocus(DiagramTextItem*)));
            connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                    this, SIGNAL(itemSelected(QGraphicsItem*)));
            addItem(textItem);
            textItem->setDefaultTextColor(myTextColor);

            textItem->setPos(mouseEvent->scenePos());
            emit textInserted(textItem);
            break;
    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void RadarScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    emit signal_xy(mouseEvent->scenePos().x(), mouseEvent->scenePos().y());
    if (myMode == InsertLine && line != nullptr) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
        update();
    }
}

void RadarScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != nullptr && myMode == InsertLine) {
        //找到线段的开始点p1处叠加的所有Items
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        //已经存在line在最上层
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        //防止点线段
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();
        //删除press时新建的线段
        removeItem(line);
        delete line;

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == DiagramItem::Type &&
            endItems.first()->type() == DiagramItem::Type &&
            startItems.first() != endItems.first()) {
            DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
            DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.first());
            Arrow *arrow = new Arrow(startItem, endItem);
            arrow->setColor(myLineColor);
            arrow->itemId = generateUniqueid();
            qDebug() << "新箭头的ID: " << arrow->itemId << "; " << idList;
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();

            modifyXmlArrows(arrow, startItem, endItem);
        }
    }
    line = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

//void RadarScene::focusInEvent(QFocusEvent *)
//{
//    qDebug() << "scene focus in";
//    // 暂时无用
//}

void RadarScene::focusOutEvent(QFocusEvent *)
{
    qDebug() << "scene focus out";
    if(myItemMenu->isEnabled() && isSelected){
//        myItemMenu->setEnabled(true);
//        qDebug() << "myItemMenu设置为true" << myItemMenu->isEnabled();
        isSelected = false;
    }else{
        myItemMenu->setEnabled(false);
        qDebug() << "myItemMenu设置为false" << myItemMenu->isEnabled();
    }
}

void RadarScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat(RadarCompDraglistWidget::puzzleMimeType()))
        event->accept();
    else
        event->ignore();
    qDebug() << "组件被托入到场景中";
}

void RadarScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat(RadarCompDraglistWidget::puzzleMimeType())){
        event->acceptProposedAction();

//        DiagramItem *item = new DiagramItem(myItemType, myItemMenu);
        DiagramItem *item = new DiagramItem(myItemIconName, myItemMenu);
        item->setBrush(myItemColor);
        item->itemId = generateUniqueid();
        addItem(item);
        item->setPos(event->scenePos());
        emit itemInserted(item);
        modifyXmlItems(event->scenePos(), item);


        QByteArray comData = event->mimeData()->data(RadarCompDraglistWidget::puzzleMimeType());
        QDataStream dataStream(&comData, QIODevice::ReadOnly);
        QPixmap pixmap;
        QString str;
        dataStream >> pixmap >> str;
        qDebug() << pixmap << "; " << str;
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else {
        event->ignore();
    }
}

void RadarScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat(RadarCompDraglistWidget::puzzleMimeType())) {
        //设置为移动而不是复制
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

//是否有选中的类型
bool RadarScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}

QDomElement *RadarScene::getItems()
{
    return &Items;
}

void RadarScene::setItems(const QDomElement &value)
{
    Items = value;
}

QDomElement *RadarScene::getArrs()
{
    return &Arrs;
}

void RadarScene::setArrs(const QDomElement &value)
{
    Arrs = value;
}

void RadarScene::setDoc(const QDomDocument &value)
{
    doc = value;
}

QString RadarScene::getMyItemIconName() const
{
    return myItemIconName;
}
