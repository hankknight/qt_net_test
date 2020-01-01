#pragma once

#include <QtWidgets/QWidget>
#include "ui_calculatenet.h"
#include <QTime>
#include <QCloseEvent>
#include <qtcpsocket.h>
#include <qhostaddress.h>
#include <qelapsedtimer.h>

#pragma execution_character_set("utf-8")

class CalculateNet : public QWidget
{
	Q_OBJECT

public:
	CalculateNet(QWidget *parent = Q_NULLPTR);
	QByteArray gen_test_frame();

private:
	Ui::CalculateNetClass ui;
	QTcpSocket *socket;
	qint32 outTime = 60;

protected:
	void closeEvent(QCloseEvent *e);

private slots:
	void on_conBtn_clicked();
	void on_disBtn_clicked();
	void on_clsBtn_clicked();
	void on_sendBtn_clicked();
	void on_outBtn_clicked();
	void socketErr_slot();
};
