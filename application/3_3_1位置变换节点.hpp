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

	// ��ȡ cow ��ģ�ͣ��Խ��жԱ�
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	// ����λ�ñ任�ڵ� part1
	osg::ref_ptr<osg::PositionAttitudeTransform> pat1 = new osg::PositionAttitudeTransform();
	// ����λ��Ϊ osg::Vec3(-10.0f,0.0f,0.0f)
	pat1->setPosition(osg::Vec3(-10.0f,0.0f,0.0f));
	// �������ţ���x y z ������Сһ��
	pat1->setScale(osg::Vec3(0.5f,0.5f,0.5f));
	// ����ӽڵ�
	pat1->addChild(node.get());

	// ����λ�ñ任�ڵ� part2
	osg::ref_ptr<osg::PositionAttitudeTransform> pat2 = new osg::PositionAttitudeTransform();
	// ����λ��Ϊ osg::Vec3(-10.0f,0.0f,0.0f)
	pat2->setPosition(osg::Vec3(10.0f,0.0f,0.0f));
	// �������ţ���x y z ������Сһ��
	pat2->setScale(osg::Vec3(0.5f,0.5f,0.5f));
	// ����ӽڵ�
	pat2->addChild(node.get());

	// ����λ�ñ任�ڵ� part3
	osg::ref_ptr<osg::PositionAttitudeTransform> pat3 = new osg::PositionAttitudeTransform();
	// ����λ��Ϊ osg::Vec3(-10.0f,0.0f,0.0f)
	pat3->setPosition(osg::Vec3(0.0f,0.0f,10.0f));
	// �������ţ���x y z ������Сһ��
	pat3->setScale(osg::Vec3(0.5f,0.5f,0.5f));
	// ����ӽڵ�
	pat3->addChild(node.get());


	// ����λ�ñ任�ڵ� part4
	osg::ref_ptr<osg::PositionAttitudeTransform> pat4 = new osg::PositionAttitudeTransform();
	// ����λ��Ϊ osg::Vec3(-10.0f,0.0f,0.0f)
	pat4->setPosition(osg::Vec3(0.0f,0.0f,-10.0f));
	// �������ţ���x y z ������Сһ��
	pat4->setScale(osg::Vec3(0.5f,0.5f,0.5f));
	// ����ӽڵ�
	pat4->addChild(node.get());


	// ����λ�ñ任�ڵ� part5
	osg::ref_ptr<osg::PositionAttitudeTransform> pat5 = new osg::PositionAttitudeTransform();
	// ����λ��Ϊ osg::Vec3(-10.0f,0.0f,0.0f)
	pat5->setPosition(osg::Vec3(0.0f,10.0f,0.0f));
	// �������ţ���x y z ������Сһ��
	pat5->setScale(osg::Vec3(0.5f,0.5f,0.5f));
	// ����ӽڵ�
	pat5->addChild(node.get());

	// ����λ�ñ任�ڵ� part6
	osg::ref_ptr<osg::PositionAttitudeTransform> pat6 = new osg::PositionAttitudeTransform();
	// ����λ��Ϊ osg::Vec3(-10.0f,0.0f,0.0f)
	pat6->setPosition(osg::Vec3(0.0f,-10.0f,0.0f));
	// �������ţ���x y z ������Сһ��
	pat6->setScale(osg::Vec3(0.5f,0.5f,0.5f));
	// ����ӽڵ�
	pat6->addChild(node.get());


	// ����λ�ñ任�ڵ� part7
	osg::ref_ptr<osg::PositionAttitudeTransform> pat7 = new osg::PositionAttitudeTransform();
	// ����λ��Ϊ osg::Vec3(-10.0f,0.0f,0.0f)
	pat7->setPosition(osg::Vec3(0.0f,0.0f,0.0f));
	// �������ţ���x y z ������Сһ��
	pat7->setScale(osg::Vec3(0.5f,0.5f,0.5f));
	// ����ӽڵ�
	pat7->addChild(node.get());


	// ��ӵ�����
	root->addChild(pat1.get());
	root->addChild(pat2.get());
	root->addChild(pat3.get());
	root->addChild(pat4.get());
	root->addChild(pat5.get());
	root->addChild(pat6.get());
	root->addChild(pat7.get());

	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}