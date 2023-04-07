/*
 * Copyright 2023 Collabora, Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <QGuiApplication>
#include <QCommandLineParser>
#include <QtCore/QUrlQuery>
#include <QtCore/QObject>
#include <QObject>
#include <QQmlEngine>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlComponent>
#include <QtQml/qqml.h>
#include <QQuickWindow>
#include <QTimer>

#include "AglShellGrpcClient.h"

const QString myname = QString("window-management-client-rpc");

enum mode {
	NONE = 0,
	FLOAT = 1,
	FULLSCREEN = 2,
	FULLSCREEN_QT = 3,	//  rather than use rpc, use Qt API
	ON_OTHER_OUTPUTS = 4,
};

static QWindow *
create_window(QQmlComponent *comp)
{
	QObject *obj = comp->create();
	obj->setParent(nullptr);

	QWindow *win = qobject_cast<QWindow *>(obj);

	return win;
}

int main(int argc, char *argv[])
{
	enum mode mmode = NONE;

	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;

	// necessary to identify correctly by app_id
	app.setDesktopFileName(myname);

	QQmlComponent main_comp(&engine, QUrl("qrc:/Main.qml"));

	if (argc >= 2) {
		const char *output_name = nullptr;
		if (strcmp(argv[1], "float") == 0)
			mmode = FLOAT;
		else if (strcmp(argv[1], "full") == 0)
			mmode = FULLSCREEN;
		else if (strcmp(argv[1], "on_output") == 0)
			mmode = ON_OTHER_OUTPUTS;
		else if (strcmp(argv[1], "full_qt") == 0)
			mmode = FULLSCREEN_QT;
		else
			assert(!"Invalid mode");

		if (mmode != FLOAT && mmode != FULLSCREEN && mmode != ON_OTHER_OUTPUTS) {
			fprintf(stderr, "Will not use rpc\n");
			goto skip;
		}

		if (mmode == ON_OTHER_OUTPUTS)
		       output_name = argv[2];

		// start grpc connection
		GrpcClient *client = new GrpcClient();

		// note that these are setting up the window state by using
		// another communication channel (rpc), and need to happen
		// before the client does the initial commit (without a buffer
		// attached)
		switch (mmode) {
		case FLOAT:
			fprintf(stderr, "Setting the application as float\n");
			client->SetAppFloat(myname.toStdString(), 40, 300);
			break;
		case FULLSCREEN:
			fprintf(stderr, "Setting the application as fullscreen\n");
			client->SetAppFullscreen(myname.toStdString());
			break;
		case ON_OTHER_OUTPUTS:
			fprintf(stderr, "Setting application '%s' on output '%s'\n", 
					myname.toStdString().c_str(), output_name);
			if (!output_name) {
				fprintf(stderr, "Output name is not set!\n");
				exit(EXIT_FAILURE);
			}
			client->SetAppOnOutput(myname.toStdString(),
					       std::string(output_name));
			break;
		default:
			break;
		}
	}

skip:
	// this would allow call any of the QWindow methods
	QWindow *win = create_window(&main_comp);

	// alternatively do the load directly
	//engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

	return app.exec();
}
