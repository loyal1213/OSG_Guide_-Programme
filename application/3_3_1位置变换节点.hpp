#include <osgViewer/Viewer>


#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Billboard>
#include <osg/Texture2D>
#include <osg/Image>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>


int TestPositionTransform(){

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// 读取 cow 的模型，以进行对比
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	// 创建位置变换节点 part1
	osg::ref_ptr<osg::PositionAttitudeTransform> pat1 = new osg::PositionAttitudeTransform();
	// 设置位置为 osg::Vec3(-10.0f,0.0f,0.0f)
	pat1->setPosition(osg::Vec3(-10.0f,0.0f,0.0f));
	// 设置缩放，在x y z 方向都缩小一倍
	pat1->setScale(osg::Vec3(0.5f,0.5f,0.5f));
	// 添加子节点
	pat1->addChild(node.get());

	// 创建位置变换节点 part2
	osg::ref_ptr<osg::PositionAttitudeTransform> pat2 = new osg::PositionAttitudeTransform();
	// 设置位置为 osg::Vec3(-10.0f,0.0f,0.0f)
	pat2->setPosition(osg::Vec3(10.0f,0.0f,0.0f));
	// 设置缩放，在x y z 方向都缩小一倍
	pat2->setScale(osg::Vec3(0.5f,0.5f,0.5f));
	// 添加子节点
	pat2->addChild(node.get());

	// 创建位置变换节点 part3
	osg::ref_ptr<osg::PositionAttitudeTransform> pat3 = new osg::PositionAttitudeTransform();
	// 设置位置为 osg::Vec3(-10.0f,0.0f,0.0f)
	pat3->setPosition(osg::Vec3(0.0f,0.0f,10.0f));
	// 设置缩放，在x y z 方向都缩小一倍
	pat3->setScale(osg::Vec3(0.5f,0.5f,0.5f));
	// 添加子节点
	pat3->addChild(node.get());


	// 创建位置变换节点 part4
	osg::ref_ptr<osg::PositionAttitudeTransform> pat4 = new osg::PositionAttitudeTransform();
	// 设置位置为 osg::Vec3(-10.0f,0.0f,0.0f)
	pat4->setPosition(osg::Vec3(0.0f,0.0f,-10.0f));
	// 设置缩放，在x y z 方向都缩小一倍
	pat4->setScale(osg::Vec3(0.5f,0.5f,0.5f));
	// 添加子节点
	pat4->addChild(node.get());


	// 创建位置变换节点 part5
	osg::ref_ptr<osg::PositionAttitudeTransform> pat5 = new osg::PositionAttitudeTransform();
	// 设置位置为 osg::Vec3(-10.0f,0.0f,0.0f)
	pat5->setPosition(osg::Vec3(0.0f,10.0f,0.0f));
	// 设置缩放，在x y z 方向都缩小一倍
	pat5->setScale(osg::Vec3(0.5f,0.5f,0.5f));
	// 添加子节点
	pat5->addChild(node.get());

	// 创建位置变换节点 part6
	osg::ref_ptr<osg::PositionAttitudeTransform> pat6 = new osg::PositionAttitudeTransform();
	// 设置位置为 osg::Vec3(-10.0f,0.0f,0.0f)
	pat6->setPosition(osg::Vec3(0.0f,-10.0f,0.0f));
	// 设置缩放，在x y z 方向都缩小一倍
	pat6->setScale(osg::Vec3(0.5f,0.5f,0.5f));
	// 添加子节点
	pat6->addChild(node.get());


	// 创建位置变换节点 part7
	osg::ref_ptr<osg::PositionAttitudeTransform> pat7 = new osg::PositionAttitudeTransform();
	// 设置位置为 osg::Vec3(-10.0f,0.0f,0.0f)
	pat7->setPosition(osg::Vec3(0.0f,0.0f,0.0f));
	// 设置缩放，在x y z 方向都缩小一倍
	pat7->setScale(osg::Vec3(0.5f,0.5f,0.5f));
	// 添加子节点
	pat7->addChild(node.get());


	// 添加到场景
	root->addChild(pat1.get());
	root->addChild(pat2.get());
	root->addChild(pat3.get());
	root->addChild(pat4.get());
	root->addChild(pat5.get());
	root->addChild(pat6.get());
	root->addChild(pat7.get());

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}