/*
�������� 
    �����������ά����ʱ������任�Ƿǳ��ؼ��ģ�ͨ���ʵ��ľ���任���Ի�ø����ƶ�������ȾЧ������ˣ��ڱ�д�Լ��ĳ������β�����ʱ��������������߼��ľ���������Ƿǳ���Ҫ�ģ�
����Գ�ѧ����˵������һ���Ѷȵġ��� osg ��, �Ѿ��ṩ��һ��������������麯���ӿڣ���Ϊ osgGA::MatrixManipulator ��
	��дһ���Լ��Ĳ���������Ҫ�������Ҫ�������£�
	1. ������̰���ʱ����ô����
	2. ��εõ���ǰ�ľ����������
	3. ��ο��Ƶ�ǰ���ٶ�
	4. �Ƿ�����ײ���
	5. ������ó���λ��
*/
#include  "TravelManipulator.h"
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

void TestTravelManipulator(){

	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	// �����������뵽������
	TravelManipulator::TravelToScene(viewer.get());

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// ��ȡ����ģ��
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("lz.osg");

	root->addChild(node.get());


	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();

}