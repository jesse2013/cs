#include "cc.h"
#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cc w;
    login l;
    if (l.exec() == QDialog::Accepted)
    {
        w.show();
        return a.exec();
    } else
        return 0;
}
