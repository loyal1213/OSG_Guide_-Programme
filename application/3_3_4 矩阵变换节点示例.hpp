#include <osgViewer/Viewer>


#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Billboard>
#include <osg/Texture2D>
#include <osg/Image>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
#include <osg/MatrixTransform>
<<<<<<< HEAD
#include <osg/Math>
=======
>>>>>>> d0a741895d18a6be5f9dc53fca0d0901dc0a024b

int TestMatrixTransform(){

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// 读取 cow 的模型，以进行对比
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	// 创建矩阵变换节点 mt1
	osg::ref_ptr<osg::MatrixTransform> mt1 = new osg::MatrixTransform();
	// 创建一个矩阵
	osg::Matrix m1;
	// 在X方向上平移10个单位
	m1.makeTranslate(osg::Vec3(10.0f,0.0f,0.0f));
	// 绕x轴旋转 45°
	// m1.makeRotate(90.0f,1.0f,0.0f,0.0f);
	m1.scale(osg::Vec3(0.1,0.1,0.1));
	// 设置矩阵
	mt1->setMatrix(m1);
	// 添加子节点
	mt1->addChild(node.get());


	// 创建矩阵变换节点 mt2
	osg::ref_ptr<osg::MatrixTransform> mt2 = new osg::MatrixTransform();
	// 创建一个矩阵
	osg::Matrix m2;
	// 在X方向上平移10个单位
	m2.makeTranslate(osg::Vec3(-10.0f,0.0f,0.0f));
	// 绕x轴旋转 45°
	// m2.makeRotate(45.0f,1.0f,0.0f,0.0f);
	m2.scale(osg::Vec3(0.1,0.1,0.1));
	// 设置矩阵
	mt2->setMatrix(m2);
	// 添加子节点
	mt2->addChild(node.get());


	// 创建矩阵变换节点 mt2
	osg::ref_ptr<osg::MatrixTransform> mt3 = new osg::MatrixTransform();
<<<<<<< HEAD
	
=======
>>>>>>> d0a741895d18a6be5f9dc53fca0d0901dc0a024b
	// 创建一个矩阵
	osg::Matrix m3;
	// 在X方向上平移0个单位
	m3.makeTranslate(osg::Vec3(0.0f,0.0f,0.0f));
	// 绕x轴旋转 45°
<<<<<<< HEAD
	m3.makeRotate(0.0f, osg::Vec3(1.0f,0.0f,0.0f), 0.0f, osg::Vec3(0.0f,1.0f,0.0f),  osg::DegreesToRadians(90.0f),  osg::Vec3(0.0f, 0.0f, 1.0f));
	m3.scale(osg::Vec3(0.1,0.1,0.1));

=======
	// m2.makeRotate(45.0f,1.0f,0.0f,0.0f);
	m3.scale(osg::Vec3(0.1,0.1,0.1));
>>>>>>> d0a741895d18a6be5f9dc53fca0d0901dc0a024b
	// 设置矩阵
	mt3->setMatrix(m3);
	// 添加子节点
	mt3->addChild(node.get());


	// 添加到场景
	root->addChild(mt1.get());
	root->addChild(mt2.get());
	root->addChild(mt3.get());


	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}