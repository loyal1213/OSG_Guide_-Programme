#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgSim/Impostor>

#include <osgUtil/Optimizer>

#include <iostream>
//  manipulators n. ���������������������ߣ����ڲٿ��ߣ����ڰڲ�����ƭ��������



int TestImpostor(){

	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();


	// ����һ���ڵ㣬��ȡţģ��
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");
	
	// ����һ������ڵ�
	osg::ref_ptr<osgSim::Impostor> impostor = new osgSim::Impostor();

	// ���� 50.0f �Ժ�ʼʹ����ͼ
	impostor->setImpostorThreshold(50.0f);
	// ����ģ�͵���ʾ��Χ�� 0 ~ 10000 ��
	impostor->addChild(node.get(),0,10000.0f);

	// ��ӵ�����
	root->addChild(impostor.get());

	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}