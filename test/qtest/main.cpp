
#include <QApplication>
#include <QDebug>

#include "base/io/log.h"

#include "window.h"

int main(int argc, char *argv[])
{
	try {
		IO::Log::set(
		    [](const std::string &msg)
		    {
			    qWarning() << QString::fromStdString(msg);
		    });

		QApplication app(argc, argv);
		Window w;
		w.show();
		return QApplication::exec();
	}
	catch (std::exception &e) {
		qWarning() << QString::fromStdString(e.what());
	}
}
