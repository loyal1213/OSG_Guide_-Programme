#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/LOD>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>


#include <osgUtil/Optimizer>



// level of detail
int TestLod(){

	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// ����һ���ڵ㣬 ��ȡţģ��
	osg::ref_ptr<osg::Node> node1 = osgDB::readNodeFile("cow.osg");

	// ����һ���ڵ㣬��ȡ�����ģ��
	osg::ref_ptr<osg::Node> node2 = osgDB::readNodeFile("glider.osg");

	// ����һ��ϸ�ڲ�� lod �ڵ�
	osg::ref_ptr<osg::LOD> lode = new osg::LOD();
	// ����ӽڵ㣬�� 0 ~ 30 ��Χ����ʾţ
	lode->addChild(node1.get(),0.0f,30.0f);
	// ����ӽڵ㣬�� 30 ~ 100 ��Χ����ʾ�����
	lode->addChild(node2.get(),30.0f,100.0f);

	// д��lode.osg�ļ�
	osgDB::writeNodeFile(*(lode.get()),"lode.osg");


	// ��ӵ�����
	root->addChild(lode.get());


	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}