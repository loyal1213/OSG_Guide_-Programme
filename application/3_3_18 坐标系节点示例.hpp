#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/CoordinateSystemNode>
#include <osg/ShapeDrawable>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>


#include <osgUtil/Optimizer>
#include <iostream>
//  manipulators n. ���������������������ߣ����ڲٿ��ߣ����ڰڲ�����ƭ��������

// ����һ������
osg::ref_ptr<osg::Node> CreateEarth(){
	// ����һ������
	osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f), osg::WGS_84_RADIUS_POLAR));

	//  ��ӵ�Ҷ�ӽڵ�
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(sd.get());

	// ��������
	std::string filename("Images/land_shallow_topo_2048.jpg");
	geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, new osg::Texture2D(osgDB::readImageFile(filename)));

	// ��������ϵ�ڵ�
	osg::ref_ptr<osg::CoordinateSystemNode> csn = new osg::CoordinateSystemNode;

	// ������Բ��ģ�ͣ�Ĭ�ϵ�����ϵΪ wgs_84
	csn->setEllipsoidModel(new osg::EllipsoidModel());

	// ����ӽڵ�
	csn->addChild(geode.get());

	return csn.get();
}


int TestCoordinateSystemNode(){

	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();


	// ��ӵ�����
	root->addChild(CreateEarth());


	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}