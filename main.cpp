#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <QDebug>
#include <vector>
#include <QSplitter>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QByteArray>
#include <boost/scoped_ptr.hpp>

//#include "MessageBus/MessageBus.hpp"
#include "Connection/connection.h"
#include "MSProtocol/msp_protocol_structs.h"
#include "MSProtocol/msp_protocol_process_interface.h"
#include "MSProtocol/attitude_protocol_process.h"
#include "MSProtocol/msp_protocol_convert_to_real_data.h"
#include "Meters/meters.h"
#include "HUD/hud.h"

#include "3D/qt_osg_widget.h"

// OSGWidget 功能较多，较为稳定
#include "3D/QtOSG/OSGWidget.h"

// 全局消息总线，暂时不使用
//MessageBus g_bus;

void SplitterWindow(MainWindow *w);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
/*
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
*/
    QSurfaceFormat format;
    format.setVersion(2, 1);
    format.setProfile( QSurfaceFormat::CompatibilityProfile);

    QSurfaceFormat::setDefaultFormat(format);

    MainWindow w;
    SplitterWindow(&w);
    w.showMaximized();

    return a.exec();
}


void SplitterWindow(MainWindow *w)
{
    QWidget *wnd = new QWidget;

    QSplitter *splitter1 = new QSplitter(Qt::Vertical);
    //QTextEdit *editor1 = new QTextEdit(QObject::tr("3D"));
    QtOSGWidget *widget_3d = new QtOSGWidget(1, 1);
    //OSGWidget *widget_3d = new OSGWidget();

    QSplitter *splitter2 = new QSplitter(Qt::Horizontal);
    //QTextEdit *editor2 = new QTextEdit(QObject::tr("Map"));
    Meters *meters = new Meters;

    QSplitter *splitter3 = new QSplitter(Qt::Vertical);
    QTextEdit *editor3 = new QTextEdit(QObject::tr("Progress"));
    //Meters *meters = new Meters;

    QTextEdit *editor4 = new QTextEdit(QObject::tr("DataBase"));
    splitter3->addWidget(editor3);
    //splitter3->addWidget(meters);
    splitter3->addWidget(editor4);

    //splitter2->addWidget(editor2);
    splitter2->addWidget(meters);
    splitter2->addWidget(splitter3);

    //splitter1->addWidget(editor1);
    splitter1->addWidget(widget_3d);
    splitter1->addWidget(splitter2);
    splitter1->setStretchFactor(0, 2);
    splitter1->setStretchFactor(1, 1);

    QSplitter *splitter4 = new QSplitter(Qt::Vertical);
    //QTextEdit *editor5 = new QTextEdit(QObject::tr("Video & HUD"));
    HUD *hud = new HUD;

    QTextEdit *editor6 = new QTextEdit(QObject::tr("Flight Control"));
    //QTextEdit *editor7 = new QTextEdit(QObject::tr("Connect"));
    Connection *connection = new Connection;
    //splitter4->addWidget(editor5);
    splitter4->addWidget(hud);

    splitter4->addWidget(editor6);
    splitter4->addWidget(connection);
    splitter4->setStretchFactor(0, 2);
    splitter4->setStretchFactor(1, 1);
    splitter4->setStretchFactor(2, 1);

    QSplitter *splitter_main = new QSplitter(Qt::Horizontal);
    splitter_main->addWidget(splitter1);
    splitter_main->addWidget(splitter4);
    splitter_main->setStretchFactor(0, 3);
    splitter_main->setStretchFactor(1, 2);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(splitter_main);

    // 加上 Widget 有边界
    wnd->setLayout(layout);
    w->setCentralWidget(wnd);

    // 直接添加没有边界
    //main_window.setCentralWidget(splitter_main);

    // 拖拽分割条时，是否实时更新
    //splitter_right->setOpaqueResize(true);


    // must be public slot!
    //QObject::connect(connection, &Connection::UpdateAttitude, meters, &Meters::UpdateMeters);
    QObject::connect(connection, &Connection::UpdateAttitude, hud, &HUD::UpdateHUD);
    QObject::connect(connection, &Connection::UpdateAttitude, widget_3d, &QtOSGWidget::Update3D);
    //QObject::connect(connection, &Connection::UpdateAttitude, widget_3d, &OSGWidget::Update3D);
    QObject::connect(connection, &Connection::UpdateMotor, meters, &Meters::UpdateMotor);

}
