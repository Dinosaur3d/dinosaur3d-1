﻿//#include "DinosaurWindow.h"
//#include <QApplication>

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);

//    DinosaurWindow w;
//    w.show();

//    return a.exec();
//}

#include <QTimer>
#include <QApplication>
#include <QGridLayout>
#include <QWidget>

#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>

#include <osgDB/ReadFile>

#include <osgQt/GraphicsWindowQt>

#include <iostream>

class ViewerWidget : public QWidget, public osgViewer::CompositeViewer
{
public:
    ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::CompositeViewer::SingleThreaded) : QWidget()
    {
        setThreadingModel(threadingModel);

        // disable the default setting of viewer.done() by pressing Escape.
        setKeyEventSetsDone(0);

        QWidget* widget1 = addViewWidget( createGraphicsWindow(0,0,100,100), osgDB::readNodeFile("../model/cow.osg") );

        QGridLayout* grid = new QGridLayout;
        grid->addWidget( widget1, 0, 0 );
        setLayout( grid );

        connect( &_timer, SIGNAL(timeout()), this, SLOT(update()) );
        _timer.start( 10 );
    }

    QWidget* addViewWidget( osgQt::GraphicsWindowQt* gw, osg::Node* scene )
    {
        osgViewer::View* view = new osgViewer::View;
        addView( view );

        osg::Camera* camera = view->getCamera();
        camera->setGraphicsContext( gw );

        const osg::GraphicsContext::Traits* traits = gw->getTraits();

        camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
        camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
        camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );

        view->setSceneData( scene );
        view->addEventHandler( new osgViewer::StatsHandler );
        view->setCameraManipulator( new osgGA::TrackballManipulator );

        return gw->getGLWidget();
    }

    osgQt::GraphicsWindowQt* createGraphicsWindow( int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false )
    {
        osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
        osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
        traits->windowName = name;
        traits->windowDecoration = windowDecoration;
        traits->x = x;
        traits->y = y;
        traits->width = w;
        traits->height = h;
        traits->doubleBuffer = true;
        traits->alpha = ds->getMinimumNumAlphaBits();
        traits->stencil = ds->getMinimumNumStencilBits();
        traits->sampleBuffers = ds->getMultiSamples();
        traits->samples = ds->getNumMultiSamples();

        return new osgQt::GraphicsWindowQt(traits.get());
    }

    virtual void paintEvent( QPaintEvent* )
    { frame(); }

protected:

    QTimer _timer;
};

int main( int argc, char** argv )
{
    osg::ArgumentParser arguments(&argc, argv);

    osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::ViewerBase::CullDrawThreadPerContext;
    while (arguments.read("--SingleThreaded")) threadingModel = osgViewer::ViewerBase::SingleThreaded;
    while (arguments.read("--CullDrawThreadPerContext")) threadingModel = osgViewer::ViewerBase::CullDrawThreadPerContext;
    while (arguments.read("--DrawThreadPerContext")) threadingModel = osgViewer::ViewerBase::DrawThreadPerContext;
    while (arguments.read("--CullThreadPerCameraDrawThreadPerContext")) threadingModel = osgViewer::ViewerBase::CullThreadPerCameraDrawThreadPerContext;

    QApplication app(argc, argv);
    ViewerWidget* viewWidget = new ViewerWidget(threadingModel);
    viewWidget->setGeometry( 100, 100, 800, 600 );
    viewWidget->show();
    return app.exec();
}
