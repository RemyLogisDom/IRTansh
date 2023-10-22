#include <QtWidgets>

#include "IRTransPlugin.h"
#include "../common.h"



IRTransPlugin::IRTransPlugin(QWidget *parent) : QWidget(parent)
{
    ui = new QWidget();
    QGridLayout *layout = new QGridLayout(ui);
    //QTabWidget *interfaces = new QTabWidget();
    //interfaces->setTabPosition(QTabWidget::West);
    //layout->addWidget(interfaces);
    //QWidget *firstTab = new QWidget();
    QWidget *w = new QWidget();
    mui = new Ui::IRTransUI;
    mui->setupUi(w);
    layout->addWidget(w);
    mui->logTxt->hide();
    //interfaces->addTab(firstTab, "toto");
    connect(mui->checkBoxLog, SIGNAL(stateChanged(int)), this, SLOT(logEnable(int)));
    connect(mui->editIP, SIGNAL(textChanged(QString)), this, SLOT(IPEdited(QString)));
    connect(mui->editPort, SIGNAL(textChanged(QString)), this, SLOT(PortEdited(QString)));
    connect(mui->On, SIGNAL(clicked()), this, SLOT(On()));
    connect(mui->Off, SIGNAL(clicked()), this, SLOT(Off()));
    connect(&socket, SIGNAL(finished()), this, SLOT(showLog()));
}


IRTransPlugin::~IRTransPlugin()
{
}


QObject *IRTransPlugin::getObject()
{
    return this;
}


QWidget *IRTransPlugin::widgetUi()
{
    return ui;
}

QWidget *IRTransPlugin::getDevWidget(QString RomID)
{
    return nullptr;
}

void IRTransPlugin::setConfigFileName(const QString fileName)
{
    configFileName = fileName;
}

void IRTransPlugin::readDevice(const QString &device)
{
}


QString IRTransPlugin::getDeviceCommands(const QString &device)
{
    return "on=" + tr("On") + "|off=" + tr("Off");
}


void IRTransPlugin::saveConfig()
{
    QFile file("IRTRans.cfg");
    QByteArray configdata;
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out.setGenerateByteOrderMark(true);
        QString str;
        QDateTime now = QDateTime::currentDateTime();
        str += "Configuration file " + now.toString() + "\n";
        str += saveformat("IPadress", ipaddress);
        str += saveformat("Port", QString("%1").arg(port));
        str += saveformat("ONCommand", mui->On_Command->text());
        str += saveformat("OFFCommand", mui->Off_Command->text());
        out << str;
        file.close();
    }
}


void IRTransPlugin::readConfig()
{
    QFile file("IRTRans.cfg");
    QByteArray configdata;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString configData;
        configData.append(in.readAll());
        file.close();
        setipaddress(getvalue("IPadress", configData));
        setport(getvalue("Port", configData));
        mui->On_Command->setText(getvalue("ONCommand", configData));
        mui->Off_Command->setText(getvalue("OFFCommand", configData));
    }
    emit(newDevice(this, "IRTRANS"));
    Off();
}


void IRTransPlugin::setLockedState(bool state)
{

}


QString IRTransPlugin::getDeviceConfig(QString RomID)
{
    return "";
}


void IRTransPlugin::setDeviceConfig(const QString &RomID, const QString &strsearch)
{
}


QString IRTransPlugin::getName()
{
    return "IRTrans";
}



void IRTransPlugin::logEnable(int state)
{
    if (state) {
        mui->logTxt->show();
    }
    else {
        mui->logTxt->hide();
    }
}


double IRTransPlugin::getMaxValue(const QString)
{
    return 1;
}


bool IRTransPlugin::isManual(const QString)
{
    return false;
}


bool IRTransPlugin::isDimmable(const QString)
{
    return false;
}


bool IRTransPlugin::acceptCommand(const QString)
{
    return true;
}


void IRTransPlugin::setStatus(const QString status)
{
    QStringList split = status.split("=");
    QString RomID = split.first();
    QString command = split.last();
    if (command == "on") On();
    if (command == "off") Off();
}


void IRTransPlugin::setipaddress(const QString &adr)
{
    if (!adr.isEmpty()) mui->editIP->setText(adr);		// affichage de l'adresse IP
}


void IRTransPlugin::setport(const QString &p)
{
    if (!p.isEmpty()) {
        mui->editPort->setText(p);		// affichage de l'adresse IP
        port = quint16(p.toInt()); }
    else {
        mui->editPort->setText("21000");		// affichage de l'adresse IP
        port = quint16(21000); }
}


void IRTransPlugin::IPEdited(QString ip)
{
    ipaddress = ip;
}


void IRTransPlugin::PortEdited(QString p)
{
    port = quint16(p.toInt());
}


void IRTransPlugin::log(const QString str)
{
    if (mui->checkBoxLog->isChecked())
    {
        if (logStr.length() > 100000) logStr = logStr.mid(90000);
        mui->logTxt->append(str);
        mui->logTxt->moveCursor(QTextCursor::End);
    }
}


void IRTransPlugin::showLog()
{
    log(socket.log);
}


void IRTransPlugin::On()
{
    socket.ip = ipaddress;
    socket.port = port;
    socket.command = mui->On_Command->text();
    socket.start();
    emit(newDeviceValue("IRTRANS", "1"));
}


void IRTransPlugin::Off()
{
    socket.ip = ipaddress;
    socket.port = port;
    socket.command = mui->Off_Command->text();
    socket.start();
    emit(newDeviceValue("IRTRANS", "0"));
}
