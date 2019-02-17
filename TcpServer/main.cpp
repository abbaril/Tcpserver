#include "dialog.h"
#include <QApplication>
#include "test.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Test test;
    //test.doTest();


    Dialog w;
    w.show();

    return a.exec();
}
