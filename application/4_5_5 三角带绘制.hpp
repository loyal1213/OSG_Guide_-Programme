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
#include <osgUtil/TriStripVisitor> // 三角带绘制示例

osg::ref_ptr<osg::Geode> CreateTriStripVisitor()
{
	// 创建一个叶子节点对象
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	// 创建一个几何体对象
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

	// 创建顶点数组，注意顶点的添加顺序是逆时针的
	osg::ref_ptr<osg::Vec3Array> vertex = new osg::Vec3Array();
	// 添加数据
	vertex->push_back(osg::Vec3(0.0,0,0.0));
	vertex->push_back(osg::Vec3(1.0,0,0.0));
	vertex->push_back(osg::Vec3(1.0,0,1.0));
	vertex->push_back(osg::Vec3(0.0,0,1.0));

	// 设置顶点数据
	geometry->setVertexArray(vertex.get());

	// 创建纹理坐标
	osg::ref_ptr<osg::Vec2Array> vt = new osg::Vec2Array();
	// 添加数据
	vt->push_back(osg::Vec2(0.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,1.0f));
	vt->push_back(osg::Vec2(0.0f,1.0f));


	// 设置纹理坐标
	geometry->setTexCoordArray(0,vt.get());

	// 创建颜色数组
	osg::ref_ptr<osg::Vec4Array> v4_color = new osg::Vec4Array();
	v4_color->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	v4_color->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	v4_color->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	v4_color->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));

	// 设置颜色数组
	geometry->setColorArray(v4_color);
	// 设置颜色的绑定方式为单个顶点
	geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	// 创建法线数组
	osg::ref_ptr<osg::Vec3Array> normal = new osg::Vec3Array();
	// 添加法线
	normal->push_back(osg::Vec3(0.0f,-1.0f,0.0f));

	// 设置法线数组
	geometry->setNormalArray(normal.get());
	geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

	// 添加图元，绘制图元为四边形
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

	// 对几何体进行条带化
	osgUtil::TriStripVisitor stripper;
	stripper.stripify(*(geometry));

	// 添加到叶子节点
	geode->addDrawable(geometry);

	return geode;
}

void TestTriStripVisitor()
{
	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	root->addChild(CreateTriStripVisitor());

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
}