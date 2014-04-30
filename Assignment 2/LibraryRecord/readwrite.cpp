#include "readwrite.h"
#include <QFile>
#include <QTextStream>

ReadWrite::ReadWrite()
{

}

void ReadWrite::writeReport(){
    QFile outputFile("output.html");
    outputFile.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream out(&outputFile);
    //out<<data;
    outputFile.close();
}
