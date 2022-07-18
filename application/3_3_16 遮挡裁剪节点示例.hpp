#include <osgViewer/Viewer>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>

#include <osg/OccluderNode>
#include <osg/StateSet>
#include <osg/ConvexPlanarOccluder>

#include <osg/BoundingBox>
#include <osg/BoundingSphere>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

#include <iostream>


// 创建遮挡节点
osg::ref_ptr<osg::Node> CreateOccluder(const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3, const osg::Vec3& v4){

	osg::ref_ptr<osg::OccluderNode> occluder_node = new osg::OccluderNode();

	// 创建遮挡平面
	osg::ref_ptr<osg::ConvexPlanarOccluder> convex_planar_occluder = new osg::ConvexPlanarOccluder();

	// 关联遮挡板平面
	occluder_node->setOccluder(convex_planar_occluder);
	occluder_node->setName("occluder");

	osg::ConvexPlanarPolygon & occluder_polygon = convex_planar_occluder->getOccluder();
	occluder_polygon.add(v1);
	occluder_polygon.add(v2);
	occluder_polygon.add(v3);
	occluder_polygon.add(v4);

	// 为遮挡面画一个四边形
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

	osg::ref_ptr<osg::Vec3Array> coords = new osg::Vec3Array(occluder_polygon.getVertexList().begin(),occluder_polygon.getVertexList().end());

	geom->setVertexArray(coords);

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(1);
	colors->at(0).set(1.0,1.0,1.0,0.5);
	geom->setColorArray(colors.get());
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(geom.get());

	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();

	// 关闭光照
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	// 使用混合， 以保证 alpha 纹理正确
	stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
	// 设置透明渲染元
	stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	geom->setStateSet(stateset.get());

	// 添加四边形作为遮挡节点，遮挡节点本身不具备遮挡能力
	occluder_node->addChild(geode.get());

	return occluder_node;
}

// 创建绕模型的遮挡场景
osg::ref_ptr<osg::Group> CreateOccludersAroundModel(osg::ref_ptr<osg::Node> model){
	// 创建场景组节点
	osg::ref_ptr<osg::Group> scene = new osg::Group();
	scene->setName("OccluderScene");

	// 添加子节点
	scene->addChild(model);
	model->setName("cow.osg");

	// 计算模型包围盒
	const osg::BoundingSphere bs = model->getBound();

	// 根据包围盒创建前后左右4个遮挡面
	osg::BoundingBox bb;
	bb.expandBy(bs);


	// 前遮挡面
	scene->addChild(CreateOccluder(
		bb.corner(0),
		bb.corner(1),
		bb.corner(5),
		bb.corner(4)
		));

	// 右遮挡面
	scene->addChild(CreateOccluder(
		bb.corner(1),
		bb.corner(3),
		bb.corner(7),
		bb.corner(5)
		));

	// 左遮挡面
	scene->addChild(CreateOccluder(
		bb.corner(2),
		bb.corner(0),
		bb.corner(4),
		bb.corner(6)
		));

	// 后遮挡面
	scene->addChild(CreateOccluder(
		bb.corner(3),
		bb.corner(2),
		bb.corner(6),
		bb.corner(7)
		));


	return scene.get();
}

void TestOccluder(){

	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	root->addChild(CreateOccludersAroundModel(node.get()));

	// root->addChild(osgDB::readNodeFile("lz.osg"));

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
}
