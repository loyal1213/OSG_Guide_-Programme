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

	//创建一个裁剪面
	osg::ref_ptr<osg::Scissor> cc = new osg::Scissor();
	// 设置裁剪平面
	cc->setScissor(150,150,300,300);

	osg::ref_ptr<osg::Group> root  = new osg::Group();

	//读取模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	root ->addChild (node.get()) ;

	//启用裁剪平面
	root ->getOrCreateStateSet()->setAttributeAndModes(cc.get(), osg::StateAttribute ::ON ) ;

	//优化场景数据
	osgUtil::Optimizer optimzier ;
	optimzier.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0;
}