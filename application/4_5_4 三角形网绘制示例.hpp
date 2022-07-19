// stdafx.h 

#include <osg/Node> //节点类
#include <osg/Geode> //是个几何节点，可以说是一个几何Group节点，一般的可绘制几何体都是通过它来传向root进行渲染，是OSG几何绘制的最高管理节点
#include <osg/Group> //对节点起到组织作用，一般作为父节点或者根节点出现
#include <osg/Geometry> //基本绘制几何体类，用户绘制基本的几何体

#include <osgDB/ReadFile>
#include <osgDB/WriteFile> 

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers> //事件监听

#include <osgGA/StateSetManipulator> //事件响应类，对渲染状态进行控制

#include <osgUtil/Optimizer> //优化器
#include <osgUtil/DelaunayTriangulator> //Delaunay三角网
//.cpp
// 基本的绘图基元——简单的几何体——复杂的几何体——复杂的场景。

/*
创建顶点数组并添加数据
设置其他相关属性（可选）
创建osgUtil::DelaunayTriangulator类对象并生成三角网
创建几何体对象，把三角网类对象生成的绘制图元添加到几何体中
按w键显示网格化模型，可以看出网格密度不同
由于光照条件默认开启，旋转模型可以看出不同部位亮度显示不同
按l键显示关闭光照，旋转模型，可以看出亮度不变
*/

/*

步骤：

1）创建顶点数组；

2）创建osgUtil::DelaunayTriangulator类对象并初始化顶点数组，同时生成三角网；

3）创建一个几何体对象，把osgUtil::DelaunayTriangulator类对象生成的绘制图元加入到几何体中，在生成三角网时还可以添加限制条件：点、线、多边形。
*/

osg::Drawable *createTriangulate()
{
	//创建顶点数组
	osg::ref_ptr<osg::Vec3Array> coords = new osg::Vec3Array();
	//不同于前面例子中添加数组数据的方法，先获取顶点的值并设置为数组，然后将数组值添加到顶点数组
	//设置顶点的值
	float vertex[][3] = { -5.0f,-5.0f, 0.4f,
		1.0f, -5.6f, 0.0f,
		5.0f, -4.0f, -0.5f,
		-6.2f, 0.0f, 4.2f,
		-1.0f,-0.5f, 4.8f,
		4.3f, 1.0f, 3.0f,
		-4.8f, 5.4f, 0.3f,
		0.6f, 5.1f,-0.8f,
		5.2f, 4.5f, 0.1f };
	//计算顶点数组的大小
	unsigned int n = sizeof(vertex) / sizeof(float[3]);
	//添加顶点数据
	for (unsigned int i = 0; i < n; i++){
		coords->push_back(osg::Vec3(vertex[i][0], vertex[i][1], vertex[i][2]));
	}

	//创建颜色数组
	osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array();
	//添加颜色数据  
	color->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	color->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	color->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	color->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	color->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	color->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	color->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	color->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	color->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));

	//创建Delaunay三角网对象
	osg::ref_ptr<osgUtil::DelaunayTriangulator> dt = new osgUtil::DelaunayTriangulator(coords.get());
	//生成三角网
	dt->triangulate();

	//创建几何体
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
	//设置顶点数组
	geometry->setVertexArray(coords.get());
	//设置颜色数组
	geometry->setColorArray(color.get());
	//设置颜色的绑定方式为单个顶点
	geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	//添加到绘图基元
	geometry->addPrimitiveSet(dt->getTriangles());

	return geometry.release();
}

int TestDelaunayTriangulator()
{
	//添加到叶节点
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(createTriangulate());

	//添加到根节点
	osg::ref_ptr<osg::Group> root = new osg::Group();
	root->addChild(geode.get());

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	//方便查看在多边形之间切换，以查看三角网
	viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));
	viewer->setSceneData(root.get());
	viewer->realize();
	return viewer->run();
}