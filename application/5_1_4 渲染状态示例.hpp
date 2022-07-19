#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/ClipNode>
#include <osg/PolygonMode>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/AnimationPath>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
#include <iostream>

osg::ref_ptr<osg::Node> CreateClipNode(osg::ref_ptr<osg::Node> sub_graph){

	osg::ref_ptr<osg::Group> root  = new osg::Group();

	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();

	// 多边形线形绘制模式， 正面和反面都绘制
	osg::ref_ptr<osg::PolygonMode> polymode = new osg::PolygonMode();
	polymode->setMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);

	// 启用多边形 线形 绘制模式，并指定状态继承树形为 OVERRIDE
	stateset->setAttribute(polymode,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);

	// 多边形 线形 绘制节点
	osg::ref_ptr<osg::Group> wireframe_subgraph = new osg::Group();
	// 设置渲染状态
	wireframe_subgraph->setStateSet(stateset.get());
	wireframe_subgraph->addChild(sub_graph.get());

	root->addChild(wireframe_subgraph.get());

	osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform();
	// 更新回调， 实现动态剪裁
	osg::ref_ptr<osg::NodeCallback> nc = new osg::AnimationPathCallback(sub_graph->getBound().center(),osg::Vec3(0.0f,0.0f,1.0f),osg::inDegrees(45.0f));
	transform->setUpdateCallback(nc.get());

	// 创建剪裁节点
	osg::ref_ptr<osg::ClipNode> clipnode = new osg::ClipNode();
	osg::BoundingSphere bs = sub_graph->getBound();
	bs.radius()*=0.4f;


	// 设置裁剪节点的包围盒
	osg::BoundingBox bb;
	bb.expandBy(bs);

	// 根据前面指定的包围盒创建6个裁剪平面
	clipnode->createClipBox(bb);
	// 禁止炼选
	clipnode->setCullingActive(false);

	transform->addChild(clipnode.get());
	root->addChild(transform.get());


	// 创建未被裁剪的节点
	osg::ref_ptr<osg::Group> cliped = new osg::Group();
	cliped->setStateSet(clipnode->getStateSet());
	cliped->addChild(sub_graph.get());

	root->addChild(cliped);
	
	return root.get();
}

void TestClipNode(){

	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// 加载模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cessna.osg");

	root->addChild(CreateClipNode(node));

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
}