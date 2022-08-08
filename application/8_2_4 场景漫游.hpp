/*
场景漫游 
    在浏览整个三维场景时，矩阵变换是非常关键的，通过适当的矩阵变换可以获得各种移动或者渲染效果，因此，在编写自己的场景漫游操作器时，如何做出符合逻辑的矩阵操作器是非常重要的，
但这对初学者来说还是有一定难度的。在 osg 中, 已经提供了一个矩阵操作器的虚函数接口，即为 osgGA::MatrixManipulator 。
	编写一个自己的操作器，需要处理的主要问题如下：
	1. 鼠标或键盘按下时该怎么处理
	2. 如何得到当前的矩阵及其逆矩阵
	3. 如何控制当前的速度
	4. 是否开启碰撞检测
	5. 如何设置出生位置
*/
#include  "TravelManipulator.h"
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

void TestTravelManipulator(){

	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	// 将漫游器加入到场景中
	TravelManipulator::TravelToScene(viewer.get());

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// 读取地形模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("lz.osg");

	root->addChild(node.get());


	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();

}