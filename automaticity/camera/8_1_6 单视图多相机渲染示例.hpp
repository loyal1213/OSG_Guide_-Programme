// stdafx.h

#include <osg/Node>
#include <osg/Geode>  
#include <osg/Geometry>
#include <osg/Group>

#include <osg/MatrixTransform> //移动节点的矩阵类，最常用的移动节点的类。可随动、旋转控制节点
#include <osg/PositionAttitudeTransform> //位置变换节点类，提供模型的位置变换、大小缩放、原点位置的设置、坐标系的变换
#include <osg/Camera> //相机节点，管理OSG中的模型——视图矩阵，相机的管理主要是通过各种变换实现的

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
#include <osgViewer/Viewer> //只允许单视图，单视图可以同时包含多个相机渲染，也可以在多窗口中渲染
#include <osgViewer/ViewerEventHandlers> //事件监听
//.cpp
 
/*
 *单窗口多相机渲染
 *
 *创建图形环境的步骤：
 *通过WindowingSystemInterface类得到系统窗口接口，该系统接口主要是为了管理窗口系统与图形环境
 *根据需要设置图形环境特性的参数
 *通过图形环境特性创建图形环境
 *通过图形环境创建窗口
 *
 *建立视口的关键步骤：
 *创建图形环境（创建一个设备上下文对象）
 *将该对象和一个相机关联
 *设置视口位置
 *添加相机到窗口中
 */
void singleWindowMultipleCameras(osg::ref_ptr<osgViewer::Viewer> viewer)
{
	//创建窗口系统接口（调用底层API得到绘图设备、窗口特性）
	osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();
 
	if (!wsi)
	{
		osg::notify(osg::NOTICE) << "Error, no WindowSystemInterface available, cannot create windows." << std::endl;
		return;
	}
 
	//得到当前窗口分辨率
	unsigned int width, height;
	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);
 
	//设置图形环境特性（设置要创建的图形设备上下文）
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
	//设置图形设备的尺寸
	traits->x = 0;
	traits->y = 0;
	traits->width = 1000;
	traits->height = 1000;
	//显示标题栏
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
 
	//创建图形环境（创建图形设备上下文）
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
 
	if (gc->valid())
	{
		osg::notify(osg::INFO) << "GraphicsWindow has been created successfully." << std::endl;
 
		gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.6f, 1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		osg::notify(osg::NOTICE) << "GraphicsWindow has not been created successfully." << std::endl;
	}
 
	//设置第一个视口（创建cameraMaster主相机）
	osg::ref_ptr<osg::Camera> cameraMaster = viewer->getCamera();
	//设置图形环境
	cameraMaster->setGraphicsContext(gc.get());
 
	//根据分辨率确定合适的投影来保证显示的图形不变形
	double fovy, aspectRatio, zNear, zFar;
	cameraMaster->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	double newAspectRatio = double(traits->width) / double(traits->height);
	double aspectRatioChange = newAspectRatio / aspectRatio;
	if (aspectRatioChange != 1.0)
	{
		cameraMaster->getProjectionMatrix() *= osg::Matrix::scale(1.0 / aspectRatioChange, 1.0, 1.0);
	}
 
	//设置视口位置
	cameraMaster->setViewport(new osg::Viewport(0, 0, 800, 800));
	//根据是否使用双缓存，确定绘制哪个缓存，如果使用双缓存，先绘制前缓存
	GLenum bufferMaster = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	//设置缓冲区（设置每帧开始绘制时，绘制哪个缓存）
	cameraMaster->setDrawBuffer(bufferMaster);
	cameraMaster->setReadBuffer(bufferMaster);
 
	//设置第二个视口（创建从属相机）
	osg::ref_ptr<osg::Camera> cameraClient = new osg::Camera();
	//设置图形环境
	cameraClient->setGraphicsContext(gc.get());
	//设置视口位置，视口的坐标为左下角为坐标原点
	cameraClient->setViewport(new osg::Viewport(0, 0, 400, 400));
	//根据是否使用双缓存，确定绘制哪个缓存，如果使用双缓存，先绘制前缓存
	GLenum bufferClient = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	//设置缓冲区（设置每帧开始绘制时，绘制哪个缓存）
	cameraClient->setDrawBuffer(bufferClient);
	cameraClient->setReadBuffer(bufferClient);
 
	//添加从属相机
	viewer->addSlave(cameraClient, osg::Matrix::scale(aspectRatio, 1.0, 1.0), osg::Matrix());
}
 
int TestSingleViewMultipleCamera()
{
	osg::ref_ptr<osg::Node> root = osgDB::readNodeFile("cow.osg");
 
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	//启用单视图多相机渲染
	singleWindowMultipleCameras(viewer.get());
 
	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
 
	viewer->getCamera()->setNodeMask(1);

	viewer->setSceneData(root.get());
	viewer->realize();
 
	return viewer->run();
}