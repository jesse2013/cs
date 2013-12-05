#ifndef CC_H
#define CC_H

#include <QWidget>

namespace Ui {
class cc;
}

class cc : public QWidget
{
    Q_OBJECT
    
public:
    explicit cc(QWidget *parent = 0);
    ~cc();
    
private:
    Ui::cc *ui;
};

#endif // CC_H
