#ifndef RIGHTWINDOW_PROPERTY_H
#define RIGHTWINDOW_PROPERTY_H

#include <QWidget>

namespace Ui {
class RightWindow_Property;
}

class RightWindow_Property : public QWidget
{
    Q_OBJECT

public:
    explicit RightWindow_Property(QWidget *parent = nullptr);
    ~RightWindow_Property();

private:
    Ui::RightWindow_Property *ui;
};

#endif // RIGHTWINDOW_PROPERTY_H
