/**********************************************************
*Write by FlySky
*zzuxp@163.com  http://www.OsgChina.org  
**********************************************************/

#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>

#include <osg/ClipPlane>
#include <osg/StateAttribute>


#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

int TestClipPlane1()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer() ;

	//����һ���ü���
	osg::ref_ptr<osg::ClipPlane> cp1 = new osg::ClipPlane() ;
	// ���òü�ƽ��
	cp1->setClipPlane(0,1,1,1);
	// ָ��ƽ������
	cp1->setClipPlaneNum(0);


	//����һ���ü�ƽ��
	osg::ref_ptr<osg::ClipPlane> cp2 = new osg::ClipPlane() ;
	// ���òü�ƽ��
	cp2->setClipPlane(1,0,0,1);
	// ָ��ƽ������
	cp2->setClipPlaneNum(1);


	osg::ref_ptr<osg::Group> root  = new osg::Group();

	//��ȡģ��
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	root ->addChild (node.get()) ;

	//���òü�ƽ��
	root ->getOrCreateStateSet()->setAttributeAndModes(cp1.get(), osg::StateAttribute ::ON ) ;
	root ->getOrCreateStateSet()->setAttributeAndModes(cp2.get(), osg::StateAttribute ::ON ) ;

	//�Ż���������
	osgUtil::Optimizer optimzier ;
	optimzier.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0;
}
