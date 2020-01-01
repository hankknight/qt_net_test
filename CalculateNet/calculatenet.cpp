#include "calculatenet.h"

CalculateNet::CalculateNet(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowTitle("���糬ʱ���Թ���");

	ui.outBox->setValue(outTime);
	ui.conBtn->setShortcut(Qt::Key_Enter);
	setFixedSize(this->width(), this->height());

	socket = new QTcpSocket(this);
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_socketErr_slot()));
}

void CalculateNet::closeEvent(QCloseEvent *e)
{
	Q_UNUSED(e);
	socket->close();
	qDebug() << "delete socket";
}

void CalculateNet::socketErr_slot()
{
	QString msg = ui.errEdit->toPlainText();
	msg += "[" + QTime::currentTime().toString("HH:mm:ss.zz") + "] " + socket->errorString() + "\n";
	ui.errEdit->setText(msg);
}

void CalculateNet::on_conBtn_clicked()
{
	QString ip = ui.ipEdit->text(),
		port = ui.portEdit->text(),
		msg = ui.errEdit->toPlainText();

	quint16 remote_port = port.toUShort();
	QHostAddress remote_ip = QHostAddress(ip);

	socket->connectToHost(remote_ip, remote_port);

	if (socket->waitForConnected())
	{
		msg += "[" + QTime::currentTime().toString("HH:mm:ss.zz") + "] ���ӷ������ɹ�\n";
		ui.errEdit->setText(msg);
	}
	else {
		msg += "[" + QTime::currentTime().toString("HH:mm:ss.zz") + "] ���ӷ�����ʧ��\n";
		ui.errEdit->setText(msg);
	}
}

void CalculateNet::on_disBtn_clicked()
{
	socket->abort();

	QString msg = ui.errEdit->toPlainText();
	msg += "[" + QTime::currentTime().toString("HH:mm:ss.zz") + "] �ѶϿ�������\n";
	ui.errEdit->setText(msg);
}

void CalculateNet::on_clsBtn_clicked()
{
	ui.errEdit->setText("");
	ui.startEdit->setText("");
	ui.endEdit->setText("");
}

void CalculateNet::on_sendBtn_clicked()
{
	QElapsedTimer t;
	qint64 recv_buffer = 0,
		temp_buffer = 0;
	QByteArray frame = gen_test_frame();

	// ����4G��ѯָ��
	socket->write(frame);

	QString msg1 = ui.errEdit->toPlainText();
	msg1 += "[" + QTime::currentTime().toString("HH:mm:ss.zz") + "] �ѷ�������\n";
	ui.errEdit->setText(msg1);

	QTime start_time = QTime::currentTime();
	// ��ʼʱ��
	ui.startEdit->setText(start_time.toString("HH:mm:ss.zz"));
	t.start();

	while (1)
	{
		qApp->processEvents();

		if (t.elapsed() > outTime * 1000)
		{
			QString msg2 = ui.errEdit->toPlainText();
			msg2 += "[" + QTime::currentTime().toString("HH:mm:ss.zz") + "] �Զ���ʱ�ѶϿ�\n";
			ui.errEdit->setText(msg2);
			break;
		}

		temp_buffer = socket->bytesAvailable();

		if ((temp_buffer != 0) && (recv_buffer == temp_buffer)) { break; }

		recv_buffer = temp_buffer;
	}

	QTime end_time = QTime::currentTime();
	// ����ʱ��
	ui.endEdit->setText(end_time.toString("HH:mm:ss.zz"));

	// ʱ������
	QString msg3 = ui.errEdit->toPlainText();
	msg3 += "[" + QTime::currentTime().toString("HH:mm:ss.zz") + "] ���յ����ݵ�ʱ���ֵΪ��" + QString::number(start_time.msecsTo(end_time)) + "ms\n";
	ui.errEdit->setText(msg3);
}

void CalculateNet::on_outBtn_clicked()
{
	outTime = ui.outBox->value();
	QString msg = ui.errEdit->toPlainText();
	msg += "[" + QTime::currentTime().toString("HH:mm:ss.zz") + "] ���ó�ʱʱ��Ϊ��" + QString::number(outTime) + "s�ɹ�\n";
	ui.errEdit->setText(msg);
}

QByteArray CalculateNet::gen_test_frame()
{
	QByteArray bytes;
	bytes.resize(6);
	bytes.clear();

	uchar frame[6] = { 0x55,0xAA,0x00,0x00,0x60,0x9F };

	for (int i = 0; i < 6; i++) { bytes.append(frame[i]); }

	return bytes;
}