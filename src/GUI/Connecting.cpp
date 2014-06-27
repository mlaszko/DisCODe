#include "Connecting.hpp"
#include <QProcess>
#include <iostream>

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

Connecting::Connecting(QWidget *parent) :
	QWidget(parent), m_client(NULL), m_task(NULL), state(0), tries(0)
{
	ui.setupUi(this);

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(proceed()));
	connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(abort()));

	QMovie* movie = new QMovie(":/preloader/cube");

	// Make sure the GIF was loaded correctly
	if (!movie->isValid())
	{
	    // Something went wrong :(
		ui.preloader->setText("Loading");
	} else {
		// Play GIF
		ui.preloader->setMovie(movie);
		movie->start();
	}

	m_try_limit = 6;

	QFont fnt(ui.btnCancel->font());
    fnt.setFamily("Ubuntu Light");
    fnt.setPointSize(16);
    ui.btnCancel->setFont(fnt);
}

Connecting::~Connecting()
{

}

void Connecting::setTextRoller(TextRoller * roller) {
	m_roller = roller;
}

void Connecting::setup(DisCODe::Client* client, QString host, QString port) {
	m_client = client;
	m_host = host;
	m_port = port;

	m_roller->setText("Waiting for DisCODe server");

	m_timer.start(500);
	
	tries = 0;
	state = 0;
}

void Connecting::proceed() {
	tries++;

	// waiting for server reply
	if (state == 0) {
		m_try_limit = 6;
		if (m_client->connect(m_host.toStdString(), m_port.toStdString())) {
			state = 1;

			m_roller->setText("Waiting for task to start");

			m_task = new DisCODe::TaskProxy(m_client);

			tries = 0;
		}
	}

	// waiting for task to start
	if (state == 1) {
		m_try_limit = 20;
		if (m_task->state() != DisCODe::TaskProxy::Initializing) {
			state = 2;
			tries = 0;
		}
	}
	
	if (state == 2) {
		m_timer.stop();
		emit connected();
	}

	if (tries > m_try_limit) {
		m_timer.stop();
		emit failed();
	}
}

void Connecting::abort() {
	m_timer.stop();
	tries = 0;
	state = 0;
    emit aborted();
}

void Connecting::closeEvent(QCloseEvent *e)
{
	abort();
    e->accept();
}
