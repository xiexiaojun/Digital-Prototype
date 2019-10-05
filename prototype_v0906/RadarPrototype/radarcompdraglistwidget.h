#ifndef RADARCOMPDRAGLISTWIDGET_H
#define RADARCOMPDRAGLISTWIDGET_H
#include <QObject>
#include <QListWidget>
#include <algorithmcomp.h>

// This is available in all editors.
/**
* @projectName   prototype_v0906
* @brief         简介:将组件列表改为ListWidget形式，可以拖动
* @author        Antrn
* @date          2019-09-27
*/
class RadarCompDraglistWidget : public QListWidget
{
    Q_OBJECT
public:
    RadarCompDraglistWidget(QWidget *parent = nullptr);
    void addDragItem(QListWidgetItem*item);
    void addDragItem(const QString &label);
    static QString puzzleMimeType() {
        return QStringLiteral("image/x-Comp-piece");
    }
    QListWidgetItem *addCompButton;
    QMap<QString, AlgorithmComp> algorithms;
    QList<QString> nameList;// 存放每个导入的组件的名字
    void createNewComp();
public slots:
    void onCurrentTextChanged(QListWidgetItem *item);
    void onCurrentDoubleClicked(QListWidgetItem *item);
    void deleteItemSlot();
    void editItemParamSlot();
    void createItemParamSlot();
    void contextMenuEvent ( QContextMenuEvent * event );

signals:
    void add_one_Comp(AlgorithmComp algo);
    void setComp_typeandMode(int id);
    void toRefreshCompList();

protected:
    //开始拖拽
    void startDrag(Qt::DropActions supportedActions);
   //从该部件中拖拽出去的操作.
    void mousePressEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
//    //接受外来拖拽的操作.
//    void dragEnterEvent(QDragEnterEvent *event);
//    void dragMoveEvent(QDragMoveEvent *event);
//    void dropEvent(QDropEvent *event);

private:
    //记录拖拽的起点.
    QPoint m_dragPoint;
    //记录被拖拽的项.
    QListWidgetItem *m_dragItem;
    QString oldName;
};

#endif // RADARCOMPDRAGLISTWIDGET_H
