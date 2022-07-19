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

	// ��������λ���ģʽ�� ����ͷ��涼����
	osg::ref_ptr<osg::PolygonMode> polymode = new osg::PolygonMode();
	polymode->setMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);

	// ���ö���� ���� ����ģʽ����ָ��״̬�̳�����Ϊ OVERRIDE
	stateset->setAttribute(polymode,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);

	// ����� ���� ���ƽڵ�
	osg::ref_ptr<osg::Group> wireframe_subgraph = new osg::Group();
	// ������Ⱦ״̬
	wireframe_subgraph->setStateSet(stateset.get());
	wireframe_subgraph->addChild(sub_graph.get());

	root->addChild(wireframe_subgraph.get());

	osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform();
	// ���»ص��� ʵ�ֶ�̬����
	osg::ref_ptr<osg::NodeCallback> nc = new osg::AnimationPathCallback(sub_graph->getBound().center(),osg::Vec3(0.0f,0.0f,1.0f),osg::inDegrees(45.0f));
	transform->setUpdateCallback(nc.get());

	// �������ýڵ�
	osg::ref_ptr<osg::ClipNode> clipnode = new osg::ClipNode();
	osg::BoundingSphere bs = sub_graph->getBound();
	bs.radius()*=0.4f;


	// ���òü��ڵ�İ�Χ��
	osg::BoundingBox bb;
	bb.expandBy(bs);

	// ����ǰ��ָ���İ�Χ�д���6���ü�ƽ��
	clipnode->createClipBox(bb);
	// ��ֹ��ѡ
	clipnode->setCullingActive(false);

	transform->addChild(clipnode.get());
	root->addChild(transform.get());


	// ����δ���ü��Ľڵ�
	osg::ref_ptr<osg::Group> cliped = new osg::Group();
	cliped->setStateSet(clipnode->getStateSet());
	cliped->addChild(sub_graph.get());

	root->addChild(cliped);
	
	return root.get();
}

void TestClipNode(){

	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// ����ģ��
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cessna.osg");

	root->addChild(CreateClipNode(node));

	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
}