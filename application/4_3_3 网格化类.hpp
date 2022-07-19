#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/ShapeDrawable>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

/* Shape类是所有形状类别的基类。Shape既可用于剪裁和碰撞检测也可用于定义程序性地产生几何体的那些基本形状。
下面的类继承自Shape类: 
TriangleMesh 
Sphere 
InfinitePlane 
HeightField 
Cylinder 
Cone 
CompositeShape 
Box 
为了使这些形状可以被渲染，我们需要把他们和Drawable类的实例关联起来。ShapeDrawable类提供了这样的功能。
这个类继承自Drawable并允许我们把Shape实例附加到可以被渲染的东西上。
既然ShapeDrawable类继承自Drawable，ShapDrawable实例就可以被加到Geode类实例上。
*/

// 创建一个四边形节点
osg::ref_ptr<osg::Node> CreateShape(){

	// 创建一个叶子节点对象
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	// 设置半径和高度
	float radius = 0.8f, height = 1.0f;

	// 创建精细度对象，精细度对象越高，细分就越多
	osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints();
	// 设置精细度为 0.5f
	hints->setDetailRatio(0.5f);

	// 添加一个球体，第一个参数为预定义几何体对象，第二个是精度， 默认为 0
	geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),radius),hints.get()));

	// 添加一个正方体
	geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2*radius),hints.get()));

	// 添加一个圆锥
	geode->addDrawable(new osg::ShapeDrawable(new osg::Cone(osg::Vec3(4.0f,0.0f,0.0f),radius,height),hints.get()));

	// 添加一个圆柱体
	geode->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(6.0f,0.0f,0.0f),radius,height),hints.get()));

	// 添加一个太空舱
	geode->addDrawable(new osg::ShapeDrawable(new osg::Capsule(osg::Vec3(8.0f,0.0f,0.0f),radius,height),hints.get()));

	return geode;
}


void TestShape(){
	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	root->addChild(CreateShape());

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
}

