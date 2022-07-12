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

	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// ��ȡģ��
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	root->addChild(node.get());


	// ����һ��������
	osg::ref_ptr<osgGA::AnimationPathManipulator> apm = new osgGA::AnimationPathManipulator("animation.path");

	// ���ò�����
	viewer->setCameraManipulator(apm.get());


	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}




