#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/PagedLOD>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgGA/TrackballManipulator>
#include <osgUtil/Optimizer>
#include <iostream>
//  manipulators n. ���������������������ߣ����ڲٿ��ߣ����ڰڲ�����ƭ��������

// �������� PageLod ����
osg::ref_ptr<osg::Group> CreatePageLod(){

	// ���� pagelod ����
	osg::ref_ptr<osg::PagedLOD> page = new osg::PagedLOD();
	// ��������λ��
	page->setCenter(osg::Vec3(0.0f,0.0f,0.0f));

	// ��ӽڵ�, ���� 0 ��������Ϊ cow.osg
	page->setFileName(0,"cow.osg");
	// ���ÿ��ӻ���ΧΪ 0.0f ~ 50.0f
	page->setRange(0,0.0f,50.0f);

	// ��ӽڵ�, ���� 0 ��������Ϊ cow.osg
	page->setFileName(1, "glider.osg");
	// ���ÿ��ӻ���ΧΪ 0.0f ~ 50.0f
	page->setRange(1, 50.0f,100.0f);


	// ��ӽڵ�, ���� 0 ��������Ϊ cow.osg
	page->setFileName(2,"cessna.osg");
	// ���ÿ��ӻ���ΧΪ 0.0f ~ 50.0f
	page->setRange(2, 100.0f, 200.0f);

	return page.get();

}




// level of detail
int TestPageLod(){

	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();


	// ��������任�ڵ�
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
	
	// ����ӽڵ�
	mt->addChild(CreatePageLod());

	// ��ת�����������ʵĽǶ�
	osg::Matrix m;
	m.makeRotate(60.0f,0.0f,0.0f,1.0f);

	// ���þ���
	mt->setMatrix(m);

	// ��� pagelod ����
	root->addChild(mt.get());

	// д�� pagedlod
	osgDB::writeNodeFile(*(root.get()),"page.osg");

	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}