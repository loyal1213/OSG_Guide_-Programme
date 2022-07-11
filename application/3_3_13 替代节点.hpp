#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgSim/Impostor>

#include <osgUtil/Optimizer>

#include <iostream>
//  manipulators n. 操纵器，调制器；操作者；善于操控者；善于摆布（欺骗）他人者



int TestImpostor(){

	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();


	// 创建一个节点，读取牛模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");
	
	// 创建一个替代节点
	osg::ref_ptr<osgSim::Impostor> impostor = new osgSim::Impostor();

	// 设置 50.0f 以后开始使用贴图
	impostor->setImpostorThreshold(50.0f);
	// 设置模型的显示范围在 0 ~ 10000 内
	impostor->addChild(node.get(),0,10000.0f);

	// 添加到场景
	root->addChild(impostor.get());

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}