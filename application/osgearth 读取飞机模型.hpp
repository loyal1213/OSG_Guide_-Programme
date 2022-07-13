#include <Windows.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>
#include <osgEarth/MapNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>

//键盘事件
#include<osgViewer/ViewerEventHandlers>

//osgGA相关的库
#include<osgGA/StateSetManipulator>

#include <osgViewer/Viewer>
#include <osgDB/WriteFile>

using namespace std;


int TestOsgearthPlane()
{
	osg::ref_ptr<osgViewer::Viewer>viewer = new osgViewer::Viewer;

	//添加状态事件，可以相应键盘和鼠标事件，响应L T B W
	viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));

	//窗口大小变化，响应F
	viewer->addEventHandler(new osgViewer::WindowSizeHandler);

	//添加路径记录 Z
	viewer->addEventHandler(new osgViewer::RecordCameraPathHandler);

	//帮助文档显示H
	viewer->addEventHandler(new osgViewer::HelpHandler);

	//截屏 C
	viewer->addEventHandler(new osgViewer::ScreenCaptureHandler);

	//添加一些常用状态设置，响应S
	viewer->addEventHandler(new osgViewer::StatsHandler);

	//操作器
	osg::ref_ptr<osgEarth::Util::EarthManipulator> earthManipulator = new osgEarth::Util::EarthManipulator;
	//设置相机操作器
	viewer->setCameraManipulator(earthManipulator);

	//根节点
	osg::ref_ptr<osg::Group> root = new osg::Group;

	//加载地球节点
	osg::ref_ptr<osg::Node> earthNode = osgDB::readNodeFile("gdal_interp.earth");
	//将地球节点加入根节点
	root->addChild(earthNode.get());



	//检测地图节点是否创建好
	osg::ref_ptr<osgEarth::MapNode> mapNode = osgEarth::MapNode::findMapNode(earthNode.get());
	if (!mapNode.get()) return 0;

	//空间设置，水平和垂直
	const osgEarth::SpatialReference *geoSRS = mapNode->getMapSRS()->getGeographicSRS();

	//添加模型
	osg::Node* model = osgDB::readNodeFile("cessna.osg");
	//osg中光照只会对有法线的模型起作用，而模型经过缩放后法线是不会变得，
	//所以需要手动设置属性，让法线随着模型大小变化而变化。GL_NORMALIZE 或 GL_RESCALE_NORMAL
	model->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);

	osg::Matrix Lmatrix;
	geoSRS->getEllipsoid()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(40.0), osg::DegreesToRadians(116.0), 500000.0, Lmatrix);  //维度，经度，高度，localToWorld

	
	//放大一些，方便看到
	Lmatrix.preMult(osg::Matrix::scale(osg::Vec3(30000, 30000, 30000)));//x,y,z轴放大倍数


	osg::MatrixTransform* mt = new osg::MatrixTransform;
	mt->setMatrix(Lmatrix);
	mt->addChild(model);
	root->addChild(mt);

	//设置现场数据
	viewer->setSceneData(root.get());
	//实现
	viewer->realize();

	// 视点定位北京地区,此句代码运行后可以直接定位到该坐标，注释后仍能正常显示模型，不过不会自动定位
	// Viewpoint(const char* name, double lon, double lat, double z, double heading, double pitch, double range);
	earthManipulator->setViewpoint(osgEarth::Viewpoint("模拟无人机", 116, 40, 0.0, 0.0, -90.0, 1.5e6));


	return viewer->run();


}