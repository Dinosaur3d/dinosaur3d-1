#ifndef DINOSAUR3DWIDGET_H
#define DINOSAUR3DWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QGridLayout>
#include <QDebug>
#include <iostream>

#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/MultiTouchTrackballManipulator>
#include <osgDB/ReadFile>
#include <osgQt/GraphicsWindowQt>

class Dinosaur3dWidget : public QWidget, public osgViewer::CompositeViewer
{
    Q_OBJECT
public:
    explicit Dinosaur3dWidget(QWidget *parent = nullptr, osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::CompositeViewer::SingleThreaded);
    QWidget* addViewWidget( osgQt::GraphicsWindowQt* gw, osg::Node* scene);
    osgQt::GraphicsWindowQt* createGraphicsWindow( int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false );
    virtual void paintEvent( QPaintEvent * );

signals:

public slots:

protected:
    QTimer _timer;
};

#endif // DINOSAUR3DWIDGET_H
