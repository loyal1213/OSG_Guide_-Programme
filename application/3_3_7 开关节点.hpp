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

	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// ����һ���ڵ㣬 ��ȡţģ��
	osg::ref_ptr<osg::Node> node1 = new osg::Node();
	node1 = osgDB::readNodeFile("cow.osg");

	// ����һ���ڵ㣬��ȡ�����ģ��
	osg::ref_ptr<osg::Node> node2 = osgDB::readNodeFile("glider.osg");

	// ����һ�����ؽڵ㣬ֻ��Ⱦ�����������Ⱦţ
	osg::ref_ptr<osg::Switch> sw = new osg::Switch();
	// ���ţģ�ͣ�������Ⱦ��Ĭ��Ϊ true
	sw->addChild(node1.get(),false);
	// ��ӻ����ģ�ͣ�״̬Ϊ��Ⱦ
	sw->addChild(node2.get(),true);


	root->addChild(sw.get());



	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}