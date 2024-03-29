#include "algorithmcomp.h"
#include "parameditradardialog.h"
#include "ui_parameditradardialog.h"
#include <QDebug>
#include <QFileDialog>
#include <QHeaderView>
#include <QTableWidget>
#include "utils.h"
#include <QApplication>
#include <QDesktopWidget>
//ParamEditRadarDialog::ParamEditRadarDialog(QString fname, QWidget *parent) :
ParamEditRadarDialog::ParamEditRadarDialog(AlgorithmComp ac, QWidget *parent) :
    QDialog(parent),
    ac(ac),
    ui(new Ui::ParamEditRadarDialog)
//    fname(fname),
{
    ui->setupUi(this);
    // 重新设置时间，不然不对
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    // 必须要设置有多少行，在ui中设置也行
//    ui->tableWidget_Param->setRowCount(20);
//    ui->tableWidget_Param->setColumnCount(3);
    ui->tableWidget_Param->setShowGrid(true);
    QTableWidgetItem *columnHeaderItem0 = ui->tableWidget_Param->horizontalHeaderItem(0); //获得水平方向表头的第一个Item对象
    columnHeaderItem0->setBackgroundColor(QColor(0,60,10)); //设置单元格背景颜色
    columnHeaderItem0->setTextColor(QColor(200,111,30)); //设置文字颜色
    ui->tableWidget_Param->setFont(QFont("Helvetica")); //设置字体
    //去掉水平滚动条
//    ui->tableWidget_Param->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //垂直滚动条按项移动
    ui->tableWidget_Param->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    //去掉自动滚动
//    ui->tableWidget_Param->setAutoScroll(false);
    ui->tableWidget_Param->horizontalHeader()->setStretchLastSection(true);
    // 选中单个单元格
    ui->tableWidget_Param->setSelectionBehavior(QAbstractItemView::SelectItems);
//    ui->tableWidget_Param->setSelectionMode(QAbstractItemView::SingleSelection);
//    ui->tableWidget_Param->setSelectionMode(QAbstractItemView::MultiSelection);
    // 正常情况下是单选，按下Ctrl键后，可以多选
    ui->tableWidget_Param->setSelectionMode(QAbstractItemView::ExtendedSelection);
    // 固定宽度先
    ui->pushButton_Path->setFixedWidth(335);
    // 双击可修改
    ui->tableWidget_Param->setEditTriggers(QAbstractItemView::DoubleClicked);

    // 每次重新读取文件不行，应该获取id，从algorithms里面找到ac
//    if(fname.compare("null")){
//        AlgorithmComp ac = Utils::readPluginXmlFile(QDir::currentPath()+"/algoXml/"+fname+".xml");
    if(!ac.getInfo()["ID"].isEmpty()){
        QString id = ac.getInfo()["ID"];
        this->ui->lineEdit_Name->setText(ac.getInfo()["Name"]);
        // WARNING 不能编辑xml文件名字，因为它是组件的名字，修改了的话保存的时候以新名字为准，就会重新生成一个新的组件文件
//        ui->lineEdit_Name->setReadOnly(true);
        ui->lineEdit_ID->setText(id);
        ui->pushButton_Path->setText(ac.getInfo()["Path"]);
        ui->textEdit->setText(ac.getDesc());
//        qDebug() << QDateTime::fromString(QString(ac.getInfo()["Time"]), "M/d/yyyy h:mm AP");
        ui->dateTimeEdit->setDateTime(QDateTime::fromString(QString(ac.getInfo()["Time"]), "M/d/yyyy h:mm AP"));
        QMap<QString, QMap<QString, QString>> paraMap = ac.getParam();
        for (QMap<QString, QMap<QString, QString>>::iterator j =paraMap.begin();j!=paraMap.end();j++) {
            int r = ui->tableWidget_Param->rowCount();//获取表格中当前总行数
            ui->tableWidget_Param->setRowCount(r+1);//添加一行
            row = r;
            QTableWidgetItem *itemName, *itemDesc, *itemValue;
            itemName = new QTableWidgetItem;
            itemDesc = new QTableWidgetItem;
            itemValue = new QTableWidgetItem;
            itemName->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter); //居中
            itemDesc->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            itemValue->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

            QString txt = QString("%1").arg(j.key());
            itemName->setText(txt);
            ui->tableWidget_Param->setItem(row, 0, itemName);
            txt = QString("%1").arg(j.value().value("describe"));
            itemDesc->setText(txt);
            ui->tableWidget_Param->setItem(row, 1, itemDesc);
            txt = QString("%1").arg(j.value().value("value"));
            itemValue->setText(txt);
            ui->tableWidget_Param->setItem(row, 2, itemValue);
            row++;
        }
    }

}

ParamEditRadarDialog::~ParamEditRadarDialog()
{
    delete ui;
}

void ParamEditRadarDialog::on_pushButton_OK_clicked()
{
    // TODO 这里要判断一下id是否合法，但还没想好，目前必须是数字才行
    if(ui->lineEdit_ID->text() == nullptr || ui->lineEdit_ID->text() == ""){
        Utils::alert(QApplication::desktop()->screen()->rect(), "拒绝添加！id不能为空");
        reject();
    }else{
        ac.setDesc(ui->textEdit->toPlainText());
        mp.insert("ID", ui->lineEdit_ID->text());

        QString nm = ui->lineEdit_Name->text();
        mp.insert("Name", nm);

        QString p = QDir::currentPath()+"/images/base.png";
        QImage image = QPixmap(p).toImage();
        QPainter painter(&image); //为这个QImage构造一个QPainter
        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        //设置画刷的组合模式CompositionMode_SourceOut这个模式为目标图像在上。
        //改变画笔和字体
        QPen pen = painter.pen();
        pen.setColor(Qt::red);
        QFont font = painter.font();
        font.setBold(true);//加粗
        font.setPixelSize(10);//改变字体大小
        painter.setPen(pen);
        painter.setFont(font);
        painter.drawText(image.rect(),Qt::AlignCenter, nm);
        //将Hello写在Image的中心
        int n = 100;//这个为图片的压缩度。0/100
        image.save(QDir::currentPath()+"/images/"+nm+".ico", "ico", n);
        //将画好的图片保存起来。

        // 只有第一次初始化用这个，后面的编辑就不改变了
        if(ac.getFileName().isEmpty() || ac.getFileName().isNull()){
            ac.setFileName(ui->lineEdit_Name->text());
        }
        mp.insert("Time", ui->dateTimeEdit->dateTime().toString());
        mp.insert("Path", ui->pushButton_Path->text());
        ac.setInfo(mp);
        QMap<QString, QMap<QString, QString> >pmap;
        QMap<QString, QString> m;
        qDebug() << "参数总行数row: " << row;
        for (int i=0;i<row;i++) {
            // WARNING 这里填写的内容一定不能为纯数字！！！，xue的教训，会直接造成写入成功，读取失败!
            QString  name = ui->tableWidget_Param->item(i, 0)->text();
            QString desc = ui->tableWidget_Param->item(i, 1)->text();
            QString value = ui->tableWidget_Param->item(i, 2)->text();
    //        qDebug() << name <<": " << desc <<"; " << value;
            m.clear();
            m.insert("describe", desc);
            m.insert("value", value);
            pmap.insert(name, m);
        }
//        qDebug() << "参数列表的大小: " << pmap.size();
        ac.clearParam();
        ac.setParam(pmap);
        accept();
    }
}

void ParamEditRadarDialog::on_pushButton_Cancel_clicked()
{
    close();
}

void ParamEditRadarDialog::on_pushButton_Path_clicked()
{
    QString dirpath = QDir::currentPath();
    QString path = QFileDialog::getExistingDirectory(this, "选择文件夹", QString(dirpath), QFileDialog::ShowDirsOnly);
    ui->pushButton_Path->setText(path);
}

void ParamEditRadarDialog::on_pushButton_Add_clicked()
{
    int r = ui->tableWidget_Param->rowCount();//获取表格中当前总行数
    ui->tableWidget_Param->setRowCount(r+1);//添加一行
    row = r;
    QTableWidgetItem *itemName, *itemDesc, *itemValue;
    itemName = new QTableWidgetItem;
    itemDesc = new QTableWidgetItem;
    itemValue = new QTableWidgetItem;
    itemName->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter); //居中
    itemDesc->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    itemValue->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    QString txt = QString("%1").arg("参数名");
    itemName->setText(txt);
    ui->tableWidget_Param->setItem(row, 0, itemName);
    txt = QString("%1").arg("参数描述");
    itemDesc->setText(txt);
    ui->tableWidget_Param->setItem(row, 1, itemDesc);
    txt = QString("%1").arg("参数值");
    itemValue->setText(txt);
    ui->tableWidget_Param->setItem(row, 2, itemValue);
    row++;
}

void ParamEditRadarDialog::on_pushButton_Del_clicked()
{
    QList<QTableWidgetItem*> items = ui->tableWidget_Param->selectedItems();
    int curRow;
    for (int i=0;i<items.count();i++) {
        curRow = ui->tableWidget_Param->row(items.at(i));//获取选中的行
        ui->tableWidget_Param->removeRow(curRow);
    }
    row-=items.count();
}
