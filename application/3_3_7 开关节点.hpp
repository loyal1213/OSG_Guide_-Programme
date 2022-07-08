#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Switch>

#include <osg/Geometry>
#include <osg/Billboard>
#include <osg/Texture2D>
#include <osg/Image>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
#include <osg/MatrixTransform>

#include <osg/AutoTransform>
#include <osgText/Text>

#include <osgUtil/Optimizer>


int TestSwitch(){

	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// 创建一个节点， 读取牛模型
	osg::ref_ptr<osg::Node> node1 = new osg::Node();
	node1 = osgDB::readNodeFile("cow.osg");

	// 创建一个节点，读取滑翔机模型
	osg::ref_ptr<osg::Node> node2 = osgDB::readNodeFile("glider.osg");

	// 创建一个开关节点，只渲染滑翔机而不渲染牛
	osg::ref_ptr<osg::Switch> sw = new osg::Switch();
	// 添加牛模型，但不渲染，默认为 true
	sw->addChild(node1.get(),false);
	// 添加滑翔机模型，状态为渲染
	sw->addChild(node2.get(),true);


	root->addChild(sw.get());



	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}