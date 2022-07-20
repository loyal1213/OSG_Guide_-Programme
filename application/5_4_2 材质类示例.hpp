#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>  
#include <osg/Geometry>
#include <osg/Group>
#include <osg/StateSet>
#include <osg/Material>
#include <osg/CullFace>

#include <osg/Camera>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

#include <iostream>

// 创建一个四边形节点
osg::ref_ptr<osg::Node> CreateNode5(){

	// 创建一个叶子节点对象
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

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
	/*osg::ref_ptr<osg::Vec4Array> v4_color = new osg::Vec4Array();
	v4_color->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	v4_color->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	v4_color->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	v4_color->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));

	// 设置颜色数组
	geometry->setColorArray(v4_color);
	// 设置颜色的绑定方式为单个顶点
	geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);*/

	// 创建法线数组
	osg::ref_ptr<osg::Vec3Array> normal = new osg::Vec3Array();
	// 添加法线
	normal->push_back(osg::Vec3(0.0f,-1.0f,0.0f));

	// 设置法线数组
	geometry->setNormalArray(normal.get());
	geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

	// 添加图元，绘制图元为四边形
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

	// 添加到叶子节点
	geode->addDrawable(geometry);

	return geode;
}

void TestMaterial(){
	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::ref_ptr<osg::Node> node = CreateNode5();

	// 得到状态属性
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset = node->getOrCreateStateSet();

	// 创建材质对象
	osg::ref_ptr<osg::Material> material = new osg::Material();
	// 设置正面散射颜色
	material->setDiffuse(osg::Material::FRONT,osg::Vec4(0.0f,0.0f,1.0f,1.0f));
	// 设置正面镜面颜色
	material->setSpecular(osg::Material::FRONT,osg::Vec4(1.0f,0.0f,0.0f,1.0f));
	// 设置正面指数
	material->setShininess(osg::Material::FRONT,90.0f);
	stateset->setAttribute(material);

	// 设置背面剔除
	osg::ref_ptr<osg::CullFace> cullface = new osg::CullFace(osg::CullFace::BACK);
	stateset->setAttribute(cullface);
	stateset->setMode(GL_CULL_FACE,osg::StateAttribute::ON);

	root->addChild(node);

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
}

