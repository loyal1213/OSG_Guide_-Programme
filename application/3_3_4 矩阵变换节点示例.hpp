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
#include <osg/MatrixTransform>
<<<<<<< HEAD
#include <osg/Math>
=======
>>>>>>> d0a741895d18a6be5f9dc53fca0d0901dc0a024b

int TestMatrixTransform(){

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// ��ȡ cow ��ģ�ͣ��Խ��жԱ�
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	// ��������任�ڵ� mt1
	osg::ref_ptr<osg::MatrixTransform> mt1 = new osg::MatrixTransform();
	// ����һ������
	osg::Matrix m1;
	// ��X������ƽ��10����λ
	m1.makeTranslate(osg::Vec3(10.0f,0.0f,0.0f));
	// ��x����ת 45��
	// m1.makeRotate(90.0f,1.0f,0.0f,0.0f);
	m1.scale(osg::Vec3(0.1,0.1,0.1));
	// ���þ���
	mt1->setMatrix(m1);
	// ����ӽڵ�
	mt1->addChild(node.get());


	// ��������任�ڵ� mt2
	osg::ref_ptr<osg::MatrixTransform> mt2 = new osg::MatrixTransform();
	// ����һ������
	osg::Matrix m2;
	// ��X������ƽ��10����λ
	m2.makeTranslate(osg::Vec3(-10.0f,0.0f,0.0f));
	// ��x����ת 45��
	// m2.makeRotate(45.0f,1.0f,0.0f,0.0f);
	m2.scale(osg::Vec3(0.1,0.1,0.1));
	// ���þ���
	mt2->setMatrix(m2);
	// ����ӽڵ�
	mt2->addChild(node.get());


	// ��������任�ڵ� mt2
	osg::ref_ptr<osg::MatrixTransform> mt3 = new osg::MatrixTransform();
<<<<<<< HEAD
	
=======
>>>>>>> d0a741895d18a6be5f9dc53fca0d0901dc0a024b
	// ����һ������
	osg::Matrix m3;
	// ��X������ƽ��0����λ
	m3.makeTranslate(osg::Vec3(0.0f,0.0f,0.0f));
	// ��x����ת 45��
<<<<<<< HEAD
	m3.makeRotate(0.0f, osg::Vec3(1.0f,0.0f,0.0f), 0.0f, osg::Vec3(0.0f,1.0f,0.0f),  osg::DegreesToRadians(90.0f),  osg::Vec3(0.0f, 0.0f, 1.0f));
	m3.scale(osg::Vec3(0.1,0.1,0.1));

=======
	// m2.makeRotate(45.0f,1.0f,0.0f,0.0f);
	m3.scale(osg::Vec3(0.1,0.1,0.1));
>>>>>>> d0a741895d18a6be5f9dc53fca0d0901dc0a024b
	// ���þ���
	mt3->setMatrix(m3);
	// ����ӽڵ�
	mt3->addChild(node.get());


	// ��ӵ�����
	root->addChild(mt1.get());
	root->addChild(mt2.get());
	root->addChild(mt3.get());


	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}