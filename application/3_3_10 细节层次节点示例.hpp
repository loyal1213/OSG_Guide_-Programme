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

	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// 创建一个节点， 读取牛模型
	osg::ref_ptr<osg::Node> node1 = osgDB::readNodeFile("cow.osg");

	// 创建一个节点，读取滑翔机模型
	osg::ref_ptr<osg::Node> node2 = osgDB::readNodeFile("glider.osg");

	// 创建一个细节层次 lod 节点
	osg::ref_ptr<osg::LOD> lode = new osg::LOD();
	// 添加子节点，在 0 ~ 30 范围内显示牛
	lode->addChild(node1.get(),0.0f,30.0f);
	// 添加子节点，在 30 ~ 100 范围内显示滑翔机
	lode->addChild(node2.get(),30.0f,100.0f);

	// 写入lode.osg文件
	osgDB::writeNodeFile(*(lode.get()),"lode.osg");


	// 添加到场景
	root->addChild(lode.get());


	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}