#ifndef CONNECTION_H
#define CONNECTION_H

#include <QDialog>

#include "../SerialPort/serial_port.h"

namespace Ui {
class Connection;
}

class Connection : public QDialog
{
    Q_OBJECT

public:
    explicit Connection(QWidget *parent = 0);
    ~Connection();

private slots:
    void on_serial_port_clicked();
    //void on_serial_port_is_open(bool serial_com_is_open);
private:
    Ui::Connection *ui;
    SerialPort *serial_port_;
};

#endif // CONNECTION_H
