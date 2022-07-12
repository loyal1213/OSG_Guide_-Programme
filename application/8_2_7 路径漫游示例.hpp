#include <iostream>
#include <osgViewer/Viewer>
#include <osgViewer/GraphicsWindow>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Camera>
#include <osg/Image>
#include <osg/BufferObject>
#include <osg/GraphicsContext>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgGA/AnimationPathManipulator>

#include <osgUtil/Optimizer>


int TestAnimationPath(){

	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// 读取模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	root->addChild(node.get());


	// 申请一个操作器
	osg::ref_ptr<osgGA::AnimationPathManipulator> apm = new osgGA::AnimationPathManipulator("animation.path");

	// 启用操作器
	viewer->setCameraManipulator(apm.get());


	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}




