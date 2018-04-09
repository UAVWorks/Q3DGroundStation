#ifndef QT_OSG_WIDGET_H
#define QT_OSG_WIDGET_H

#include <stdio.h>
#include <iostream>

#include <QOpenGLWidget>

#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osg/PositionAttitudeTransform>
#include <osgViewer/Viewer>



class QMouseEvent;
class QWheelEvent;
class osgGA::EventQueue;
class QEvent;
class QTimer;

class RotateCallback : public osg::NodeCallback {
public:
  RotateCallback() : roll_(0.0), pitch_(0.0), yaw_(0.0) {

  }

  virtual void operator ()(osg::Node *node, osg::NodeVisitor *nv) {
    osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform*>(node);
    if (pat != nullptr) {
      pat->setAttitude(osg::Quat(roll_, osg::X_AXIS, pitch_, osg::Y_AXIS, yaw_, osg::Z_AXIS));
    }
    traverse(node, nv);
  }

  void SetRotateValue(const double roll, double pitch, double yaw) {
    this->roll_ = roll;
    this->pitch_ = pitch;
    this->yaw_ = yaw;
  }

private:
  double roll_;
  double pitch_;
  double yaw_;
};

class QtOSGWidget : public QOpenGLWidget
{
public:
  QtOSGWidget(qreal scaleX, qreal scaleY, QWidget* parent = 0);
  
  virtual ~QtOSGWidget();
  
  void setScale(qreal X, qreal Y);

public slots:
  //void Update3D(const MspAttitudeDownDC &maddc);

protected:

  virtual void paintGL();

  virtual void resizeGL( int width, int height );

  virtual void initializeGL();

  virtual void mouseMoveEvent(QMouseEvent* event);

  virtual void mousePressEvent(QMouseEvent* event);

  virtual void mouseReleaseEvent(QMouseEvent* event);

  // 鼠标滚轮事件
  virtual void wheelEvent(QWheelEvent* event);

  virtual bool event(QEvent* event);


private:
  void UpdateTimer();

private:
  osgGA::EventQueue* getEventQueue() const;

  osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _mGraphicsWindow;
  osg::ref_ptr<osgViewer::Viewer> _mViewer;
  qreal m_scaleX, m_scaleY;

  osg::Node *model_;
  osg::ref_ptr<osg::PositionAttitudeTransform> pat_;
  QTimer *timer_;
  RotateCallback *rotate_callback_;
};


#endif // QT_OSG_WIDGET_H
