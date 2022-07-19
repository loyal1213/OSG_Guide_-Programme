#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgGA/StateSetManipulator>

#include <osgUtil/Optimizer>
#include <osgUtil/SmoothingVisitor>

// 创建一个四边形节点
osg::ref_ptr<osg::Geometry> CreateSmoothing(){

	// 创建一个几何体对象
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

	// 创建顶点数组，注意顶点的添加顺序是逆时针的
	osg::ref_ptr<osg::Vec3Array> vertex = new osg::Vec3Array();

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

	// 添加图元，绘制图元为四边形
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

	return geometry;
}

void TestSmoothing(){
	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// 创建一个几何体对象，注意， 几何体对象并没有指定法线
	// 如果读者注释下面生成顶点法线的代码，就可以看到光照的差别
	osg::ref_ptr<osg::Geometry> geometry = CreateSmoothing();

	// 生成顶点法线
	osgUtil::SmoothingVisitor::smooth(*(geometry.get()));

	// 添加到叶节点
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(geometry.get());


	root->addChild(geode);

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
}