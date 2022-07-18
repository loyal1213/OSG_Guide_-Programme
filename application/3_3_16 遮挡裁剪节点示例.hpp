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


// �����ڵ��ڵ�
osg::ref_ptr<osg::Node> CreateOccluder(const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3, const osg::Vec3& v4){

	osg::ref_ptr<osg::OccluderNode> occluder_node = new osg::OccluderNode();

	// �����ڵ�ƽ��
	osg::ref_ptr<osg::ConvexPlanarOccluder> convex_planar_occluder = new osg::ConvexPlanarOccluder();

	// �����ڵ���ƽ��
	occluder_node->setOccluder(convex_planar_occluder);
	occluder_node->setName("occluder");

	osg::ConvexPlanarPolygon & occluder_polygon = convex_planar_occluder->getOccluder();
	occluder_polygon.add(v1);
	occluder_polygon.add(v2);
	occluder_polygon.add(v3);
	occluder_polygon.add(v4);

	// Ϊ�ڵ��滭һ���ı���
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

	// �رչ���
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	// ʹ�û�ϣ� �Ա�֤ alpha ������ȷ
	stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
	// ����͸����ȾԪ
	stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	geom->setStateSet(stateset.get());

	// ����ı�����Ϊ�ڵ��ڵ㣬�ڵ��ڵ㱾���߱��ڵ�����
	occluder_node->addChild(geode.get());

	return occluder_node;
}

// ������ģ�͵��ڵ�����
osg::ref_ptr<osg::Group> CreateOccludersAroundModel(osg::ref_ptr<osg::Node> model){
	// ����������ڵ�
	osg::ref_ptr<osg::Group> scene = new osg::Group();
	scene->setName("OccluderScene");

	// ����ӽڵ�
	scene->addChild(model);
	model->setName("cow.osg");

	// ����ģ�Ͱ�Χ��
	const osg::BoundingSphere bs = model->getBound();

	// ���ݰ�Χ�д���ǰ������4���ڵ���
	osg::BoundingBox bb;
	bb.expandBy(bs);


	// ǰ�ڵ���
	scene->addChild(CreateOccluder(
		bb.corner(0),
		bb.corner(1),
		bb.corner(5),
		bb.corner(4)
		));

	// ���ڵ���
	scene->addChild(CreateOccluder(
		bb.corner(1),
		bb.corner(3),
		bb.corner(7),
		bb.corner(5)
		));

	// ���ڵ���
	scene->addChild(CreateOccluder(
		bb.corner(2),
		bb.corner(0),
		bb.corner(4),
		bb.corner(6)
		));

	// ���ڵ���
	scene->addChild(CreateOccluder(
		bb.corner(3),
		bb.corner(2),
		bb.corner(6),
		bb.corner(7)
		));


	return scene.get();
}

void TestOccluder(){

	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	root->addChild(CreateOccludersAroundModel(node.get()));

	// root->addChild(osgDB::readNodeFile("lz.osg"));

	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
}
