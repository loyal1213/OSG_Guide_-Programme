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

	//创建一个裁剪面
	osg::ref_ptr<osg::ClipPlane> cp1 = new osg::ClipPlane() ;
	// 设置裁剪平面
	cp1->setClipPlane(0,1,1,1);
	// 指定平面索引
	cp1->setClipPlaneNum(0);


	//创建一个裁剪平面
	osg::ref_ptr<osg::ClipPlane> cp2 = new osg::ClipPlane() ;
	// 设置裁剪平面
	cp2->setClipPlane(1,0,0,1);
	// 指定平面索引
	cp2->setClipPlaneNum(1);


	osg::ref_ptr<osg::Group> root  = new osg::Group();

	//读取模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	root ->addChild (node.get()) ;

	//启用裁剪平面
	root ->getOrCreateStateSet()->setAttributeAndModes(cp1.get(), osg::StateAttribute ::ON ) ;
	root ->getOrCreateStateSet()->setAttributeAndModes(cp2.get(), osg::StateAttribute ::ON ) ;

	//优化场景数据
	osgUtil::Optimizer optimzier ;
	optimzier.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();

	return 0;
}
