/**********************************************************
*Write by FlySky
*zzuxp@163.com  http://www.OsgChina.org  
**********************************************************/

#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Scissor>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

int TestClipPlane2()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer() ;

	//����һ���ü���
	osg::ref_ptr<osg::Scissor> cc = new osg::Scissor();
	// ���òü�ƽ��
	cc->setScissor(150,150,300,300);

	osg::ref_ptr<osg::Group> root  = new osg::Group();

	//��ȡģ��
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	root ->addChild (node.get()) ;

	//���òü�ƽ��
	root ->getOrCreateStateSet()->setAttributeAndModes(cc.get(), osg::StateAttribute ::ON ) ;

	//�Ż���������
	osgUtil::Optimizer optimzier ;
	optimzier.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0;
}