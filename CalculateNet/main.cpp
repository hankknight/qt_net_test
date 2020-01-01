#include "calculatenet.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CalculateNet w;
	w.show();
	return a.exec();
}
