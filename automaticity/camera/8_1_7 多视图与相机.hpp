// stdafx.h

#include <osg/Node>
#include <osg/Geode>  
#include <osg/Geometry>
#include <osg/Group>

#include <osg/MatrixTransform> //移动节点的矩阵类，最常用的移动节点的类。可随动、旋转控制节点
#include <osg/PositionAttitudeTransform> //位置变换节点类，提供模型的位置变换、大小缩放、原点位置的设置、坐标系的变换
#include <osg/Camera>  //相机节点，管理OSG中的模型——视图矩阵，相机的管理主要是通过各种变换实现的

#include <osgGA/TrackballManipulator> 

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers> //事件监听
#include <osgViewer/CompositeViewer>


//.cpp

/*
*多视图相机渲染
*
*在OSG中，多视图的管理是通过osgViewer::CompositeViewer来实现的，它负责多个视图的管理及同步工作，继承自osgViewer::ViewerBase类
*
*视图的创建：
*设置图形环境特性
*创建图形环境特性
*创建视图
*设置视图场景数据
*（根据分辨率确定合适的投影来保证显示的图形不变形）
*设置相机视口及图形环境
*设置操作器
*（添加事件处理）
*/


osg::ref_ptr<osg::GraphicsContext> CreateGraphicsContext(osg::ref_ptr<osg::GraphicsContext::Traits> traits)
{
	//设置图形环境特性
	//osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
	traits->x = 100;
	traits->y = 100;
	traits->width = 900;
	traits->height = 700;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	//创建图形环境特性
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
	if (gc->valid())
	{
		osg::notify(osg::INFO) << "GraphicsWindow has been created successfully." << std::endl;

		//清除窗口颜色及颜色和深度缓冲
		gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 1.6f, 1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		osg::notify(osg::NOTICE) << "GraphicsWindow has not been created successfully." << std::endl;
	}

	return gc.get();
}

//视图一
void View1(osg::ref_ptr<osgViewer::CompositeViewer> viewer, osg::ref_ptr<osg::Node> node, osg::ref_ptr<osg::GraphicsContext::Traits> traits, osg::ref_ptr<osg::GraphicsContext> gc)
{
	//创建视图一
	osg::ref_ptr<osgViewer::View> view = new osgViewer::View();
	viewer->addView(view.get());

	//设置视图场景数据
	view->setSceneData(node.get());

	//设置相机视口及图形环境
	view->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width / 2, traits->height / 2));
	view->getCamera()->setGraphicsContext(gc.get());

	//设置操作器
	view->setCameraManipulator(new osgGA::TrackballManipulator);

	//添加事件处理
	view->addEventHandler(new osgViewer::StatsHandler);
	view->addEventHandler(new osgViewer::WindowSizeHandler);
	view->addEventHandler(new osgViewer::ThreadingHandler);
	view->addEventHandler(new osgViewer::RecordCameraPathHandler);
}

//视图二
void View2(osg::ref_ptr<osgViewer::CompositeViewer> viewer, osg::ref_ptr<osg::Node> node, osg::ref_ptr<osg::GraphicsContext::Traits> traits, osg::ref_ptr<osg::GraphicsContext> gc)
{
	//创建视图二
	osg::ref_ptr<osgViewer::View> view = new osgViewer::View();
	viewer->addView(view.get());

	//设置视图场景数据
	view->setSceneData(node.get());

	//设置相机视口及图形环境
	view->getCamera()->setViewport(new osg::Viewport(traits->width / 2, 0, traits->width / 2, traits->height / 2));
	view->getCamera()->setGraphicsContext(gc.get());

	//设置操作器
	view->setCameraManipulator(new osgGA::TrackballManipulator);
}

//视图三
void View3(osg::ref_ptr<osgViewer::CompositeViewer> viewer, osg::ref_ptr<osg::Node> node, osg::ref_ptr<osg::GraphicsContext::Traits> traits, osg::ref_ptr<osg::GraphicsContext> gc)
{
	//创建视图三
	osg::ref_ptr<osgViewer::View> view = new osgViewer::View();
	viewer->addView(view.get());

	//设置视图场景数据
	view->setSceneData(node.get());

	//根据分辨率确定合适的投影来保证显示的图形不变形
	double fovy, aspectRatio, zNear, zFar;
	view->getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	double newAspectRatio = double(traits->width) / double(traits->height / 2);
	double aspectRatioChange = newAspectRatio / aspectRatio;
	if (aspectRatioChange != 1.0)
	{
		view->getCamera()->getProjectionMatrix() *= osg::Matrix::scale(1.0 / aspectRatioChange, 1.0, 1.0);
	}

	//设置相机视口及图形环境
	view->getCamera()->setViewport(new osg::Viewport(traits->width / 2, traits->height / 2, traits->width / 2, traits->height / 2));
	view->getCamera()->setGraphicsContext(gc.get());

	//设置操作器
	view->setCameraManipulator(new osgGA::TrackballManipulator);
}

int TestCompositeViewer()
{
	//读取模型
	osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("cow.osg");
	osg::ref_ptr<osg::Node> cessna = osgDB::readNodeFile("cessna.osg");

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(cow.get());
	optimizer.optimize(cessna.get());

	//设置并创建图形环境
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
	osg::ref_ptr<osg::GraphicsContext> gc = CreateGraphicsContext(traits);

	//创建CompositeViewer对象
	osg::ref_ptr<osgViewer::CompositeViewer> viewer = new osgViewer::CompositeViewer();
	View1(viewer, cow.get(), traits.get(), gc.get());
	View2(viewer, cessna.get(), traits.get(), gc.get());
	View3(viewer, cessna.get(), traits.get(), gc.get());

	viewer->realize();

	return viewer->run();
}

