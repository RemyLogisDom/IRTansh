#ifndef IRTRANSPLUGIN_H
#define IRTRANSPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <QTabWidget>
#include <QTcpSocket>
#include <QThread>
#include "ui_IRTrans.h"
#include "../interface.h"


struct Dev_Data
{
    QString RomID;
    double value = 0;
};


class tcpSocket : public QThread
{
public:
QString ip, command, log;
quint16 port;
    tcpSocket()
    {
    }

    void run()
    {
        log.clear();
        QTcpSocket tcp;
        tcp.connectToHost(ip, port);
        tcp.waitForConnected();
        if (tcp.state() == QAbstractSocket::ConnectedState) {
            log.append("Connected to " + ip + "\n");
            command.append("\n");
            QByteArray data(command.toLatin1());
            tcp.write(data);
            log.append("Command sent " + command );
            if (tcp.waitForBytesWritten(1000))
            {
                if (tcp.waitForReadyRead(1000))
                {
                    log.append(tcp.readAll());
                }
            }
            tcp.disconnect();
        }
        else {
            log.append("Could not connect to " + ip + "\n");
        }
    }
};

class IRTransPlugin : public QWidget, LogisDomInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "logisdom.network.LogisDomInterface/1.0" FILE "IRTransPlugin.json")
    Q_INTERFACES(LogisDomInterface)
public:
    IRTransPlugin( QWidget *parent = nullptr );
    ~IRTransPlugin() override;
    QObject* getObject() override;
    QWidget *widgetUi() override;
    QWidget *getDevWidget(QString RomID) override;
    void setConfigFileName(const QString) override;
    void readDevice(const QString &device) override;
    QString getDeviceCommands(const QString &device) override;
    void saveConfig() override;
    void readConfig() override;
    void setLockedState(bool) override;
    QString getDeviceConfig(QString) override;
    void setDeviceConfig(const QString &, const QString &) override;
    QString getName() override;
    void setStatus(const QString) override;
    bool acceptCommand(const QString) override;
    bool isDimmable(const QString) override;
    bool isManual(const QString) override;
    double getMaxValue(const QString) override;
signals:
    void newDeviceValue(QString, QString) override;
    void newDevice(LogisDomInterface*, QString) override;
    void deviceSelected(QString) override;
    void updateInterfaceName(LogisDomInterface*, QString) override;
    void connectionStatus(QAbstractSocket::SocketState) override;
private:
    QTabWidget *interfaces;
    QWidget *ui;
    QString configFileName;
    void saveSetup();
    void loadSetup();
    void setipaddress(const QString &adr);
    QString ipaddress;
    void setport(const QString &adr);
    quint16 port = 80;
    Ui::IRTransUI *mui;
    QString lastStatus;
    void log(const QString);
    QString logStr;
    tcpSocket socket;
private slots:
    void logEnable(int);
    void IPEdited(QString);
    void PortEdited(QString);
    void showLog();
    void On();
    void Off();
};


#endif

