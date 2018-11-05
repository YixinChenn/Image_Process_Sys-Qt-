#ifndef COLORDISPLAY_H
#define COLORDISPLAY_H

#include <QWidget>

namespace Ui {
class colorDisplay;
}

class colorDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit colorDisplay(QWidget *parent = nullptr);
    colorDisplay(int R, int G, int B);
    ~colorDisplay();

    void setInfo(QColor color);

private:
    Ui::colorDisplay *ui;
};

#endif // COLORDISPLAY_H
