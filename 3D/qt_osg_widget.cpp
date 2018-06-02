#include "qt_osg_widget.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QDesktopWidget>
#include <QScreen>
#include <QtGlobal>
#include <QWindow>
#include <QTimer>

#include <osg/Camera>
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/Material>
#include <osgGA/EventQueue>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>

// suggest using callback
#define USING_ROTATE_CALLBACK 1


QtOSGWidget::QtOSGWidget(qreal scaleX, qreal scaleY, QWidget* parent)
  : QOpenGLWidget(parent)
  , _mGraphicsWindow(new osgViewer::GraphicsWindowEmbedded( this->x(), this->y(),
                                                            this->width(), this->height() ) )
  , _mViewer(new osgViewer::Viewer)
  , m_scaleX(scaleX)
  , m_scaleY(scaleY)
  , model_(osgDB::readNodeFile("Drone.obj"))
  , pat_(new osg::PositionAttitudeTransform)
  , timer_(new QTimer(this))
  , rotate_callback_(new RotateCallback)
{
  // 创建摄像机
  osg::Camera* camera = new osg::Camera;
  camera->setViewport( 0, 0, this->width(), this->height() );
  //camera->setClearColor( osg::Vec4( 0.9f, 0.9f, 1.f, 1.f ) );
  float aspectRatio = static_cast<float>( this->width()) / static_cast<float>( this->height() );
  camera->setProjectionMatrixAsPerspective( 30.f, aspectRatio, 1.f, 1000.f );
  camera->setGraphicsContext( _mGraphicsWindow );
  camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));

  // 绑定摄像机
  _mViewer->setCamera(camera);

  pat_->addChild(model_);
#if USING_ROTATE_CALLBACK
  pat_->setUpdateCallback(rotate_callback_);
#endif
  _mViewer->setSceneData(pat_.get());

  osgGA::TrackballManipulator* manipulator = new osgGA::TrackballManipulator;
  manipulator->setAllowThrow( false );
  this->setMouseTracking(true);
  _mViewer->setCameraManipulator(manipulator);
  _mViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
  _mViewer->realize();

  connect(timer_, &QTimer::timeout, this, &QtOSGWidget::UpdateTimer);
#if !USING_ROTATE_CALLBACK
  timer_->start(30);
#endif
}


QtOSGWidget::~QtOSGWidget() {

}

void QtOSGWidget::setScale(qreal X, qreal Y) {
  m_scaleX = X;
  m_scaleY = Y;
  this->resizeGL(this->width(), this->height());
}



void QtOSGWidget::paintGL() {
  _mViewer->frame();
}


void QtOSGWidget::resizeGL( int width, int height ) {

  this->getEventQueue()->windowResize(this->x()*m_scaleX, this->y() * m_scaleY, width*m_scaleX, height*m_scaleY);
  _mGraphicsWindow->resized(this->x()*m_scaleX, this->y() * m_scaleY, width*m_scaleX, height*m_scaleY);
  // 获取摄像机，重新设置视口
  osg::Camera* camera = _mViewer->getCamera();
  camera->setViewport(0, 0, this->width()*m_scaleX, this->height()* m_scaleY);
}

void QtOSGWidget::initializeGL() {
  // 初始化圆筒节点状态几何
/*
  osg::Geode* geode = dynamic_cast<osg::Geode*>(_mViewer->getSceneData());
  osg::StateSet* stateSet = geode->getOrCreateStateSet();
  osg::Material* material = new osg::Material;
  material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
  stateSet->setAttributeAndModes( material, osg::StateAttribute::ON );
  stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
*/
}

void QtOSGWidget::mouseMoveEvent(QMouseEvent* event) {
  //this->getEventQueue()->mouseMotion(event->x()*m_scaleX, event->y()*m_scaleY);
}

void QtOSGWidget::mousePressEvent(QMouseEvent* event) {
  unsigned int button = 0;
  switch (event->button()){
    case Qt::LeftButton:
      button = 1;
      break;
    case Qt::MiddleButton:
      button = 2;
      break;
    case Qt::RightButton:
      button = 3;
      break;
    default:
      button = 0;
      break;
  }

  //this->getEventQueue()->mouseButtonPress(event->x()*m_scaleX, event->y()*m_scaleY, button);
}

void QtOSGWidget::mouseReleaseEvent(QMouseEvent* event) {
  unsigned int button = 0;
  switch (event->button()){
    case Qt::LeftButton:
      button = 2;
      break;
    case Qt::MiddleButton:
      button = 2;
      break;
    case Qt::RightButton:
      button = 3;
      break;
    default:
      button = 0;
      break;
  }

  //this->getEventQueue()->mouseButtonRelease(event->x()*m_scaleX, event->y()*m_scaleY, button);
}

void QtOSGWidget::wheelEvent(QWheelEvent* event) {
  int delta = event->delta();
  osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?
        osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN;
  this->getEventQueue()->mouseScroll(motion);
}

bool QtOSGWidget::event(QEvent* event) {
  bool handled = QOpenGLWidget::event(event);
  this->update();
  return handled;
}


osgGA::EventQueue* QtOSGWidget::getEventQueue() const {
  osgGA::EventQueue* eventQueue = _mGraphicsWindow->getEventQueue();
  return eventQueue;
}


/**
 * @brief Update 3D
 * @param attitude pack
 * @note This slot fun connect Connection::UpdateAttitude signal
 */
void QtOSGWidget::Update3D(const mavlink_attitude_t &attitude) {
#if USING_ROTATE_CALLBACK
  rotate_callback_->SetRotateValue(attitude.roll, attitude.pitch, attitude.yaw);
#else
  pat_ = dynamic_cast<osg::PositionAttitudeTransform*>(_mViewer->getSceneData());
  // x -> roll, y -> pitch, z -> yaw
  pat_->setAttitude(osg::Quat(attitude.roll, osg::X_AXIS, attitude.pitch, osg::Y_AXIS, attitude.yaw, osg::Z_AXIS));
#endif
}


void QtOSGWidget::UpdateTimer(){
  // update 3D model
  update();
}
