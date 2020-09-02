#include "HttpDemo.h"
#include <QtWidgets/QApplication>
#include "HttpClient.h"

int main(int argc, char *argv[])
{

	HttpClient::initNet();

    QApplication a(argc, argv);
    HttpDemo w;
    w.show();
    int k = a.exec();

	HttpClient::releaseNet();

	return k;
}
