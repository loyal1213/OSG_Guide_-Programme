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
//  manipulators n. 操纵器，调制器；操作者；善于操控者；善于摆布（欺骗）他人者

// 创建三级 PageLod 场景
osg::ref_ptr<osg::Group> CreatePageLod(){

	// 创建 pagelod 对象
	osg::ref_ptr<osg::PagedLOD> page = new osg::PagedLOD();
	// 设置中心位置
	page->setCenter(osg::Vec3(0.0f,0.0f,0.0f));

	// 添加节点, 设置 0 级的内容为 cow.osg
	page->setFileName(0,"cow.osg");
	// 设置可视化范围为 0.0f ~ 50.0f
	page->setRange(0,0.0f,50.0f);

	// 添加节点, 设置 0 级的内容为 cow.osg
	page->setFileName(1, "glider.osg");
	// 设置可视化范围为 0.0f ~ 50.0f
	page->setRange(1, 50.0f,100.0f);


	// 添加节点, 设置 0 级的内容为 cow.osg
	page->setFileName(2,"cessna.osg");
	// 设置可视化范围为 0.0f ~ 50.0f
	page->setRange(2, 100.0f, 200.0f);

	return page.get();

}




// level of detail
int TestPageLod(){

	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();


	// 创建矩阵变换节点
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
	
	// 添加子节点
	mt->addChild(CreatePageLod());

	// 旋转，调整到合适的角度
	osg::Matrix m;
	m.makeRotate(60.0f,0.0f,0.0f,1.0f);

	// 设置矩阵
	mt->setMatrix(m);

	// 添加 pagelod 场景
	root->addChild(mt.get());

	// 写入 pagedlod
	osgDB::writeNodeFile(*(root.get()),"page.osg");

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}