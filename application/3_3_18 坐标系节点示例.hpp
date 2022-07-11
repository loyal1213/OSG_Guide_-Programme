#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/CoordinateSystemNode>
#include <osg/ShapeDrawable>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>


#include <osgUtil/Optimizer>
#include <iostream>
//  manipulators n. 操纵器，调制器；操作者；善于操控者；善于摆布（欺骗）他人者

// 绘制一个地球
osg::ref_ptr<osg::Node> CreateEarth(){
	// 创建一个地球
	osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f), osg::WGS_84_RADIUS_POLAR));

	//  添加到叶子节点
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(sd.get());

	// 设置纹理
	std::string filename("Images/land_shallow_topo_2048.jpg");
	geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, new osg::Texture2D(osgDB::readImageFile(filename)));

	// 创建坐标系节点
	osg::ref_ptr<osg::CoordinateSystemNode> csn = new osg::CoordinateSystemNode;

	// 设置椭圆体模型，默认的坐标系为 wgs_84
	csn->setEllipsoidModel(new osg::EllipsoidModel());

	// 添加子节点
	csn->addChild(geode.get());

	return csn.get();
}


int TestCoordinateSystemNode(){

	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();


	// 添加到场景
	root->addChild(CreateEarth());


	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}