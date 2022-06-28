#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PrimitiveSet>
#include <osg/LineWidth>

#include <osgDB/ReaderWriter>
#include <osgDB/ReadFile>


// ������ͼԪ
osg::ref_ptr<osg::Node> CreateSimple(){

	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();


	// ����һЩ����
	osg::ref_ptr<osg::Vec3Array> coords = new osg::Vec3Array();
	// ������ɫ
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
	// ���뷨����
	osg::ref_ptr<osg::Vec3Array> normal = new osg::Vec3Array();
	// �����߿�
	osg::ref_ptr<osg::LineWidth> line_width = new osg::LineWidth(); 


	geode->addDrawable(geom.get());

	// ���ö���
	geom->setVertexArray(coords.get());

	// ���ö�����ɫ
	geom->setColorArray(colors.get());
	geom->setColorBinding(osg::Geometry::AttributeBinding::BIND_PER_VERTEX);


	// ���÷�����
	geom->setNormalArray(normal.get());
	geom->setNormalBinding(osg::Geometry::AttributeBinding::BIND_PER_VERTEX);
	
	// �����߿�
	line_width->setWidth(15.0);
	geode->getOrCreateStateSet()->setAttributeAndModes(line_width.get(),osg::StateAttribute::ON);


	// ���ö��������ʽ
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, 4));


	// ����
	coords->push_back(osg::Vec3(-10.0, 5.0, -10.0));
	coords->push_back(osg::Vec3( 10.0, 5.0, -10.0));
	coords->push_back(osg::Vec3( 10.0, 5.0,  10.0));
	coords->push_back(osg::Vec3(-10.0, 5.0,  10.0));

	// ��ɫ
	colors->push_back(osg::Vec4f(1.0,0.0,0.0,0.5));
	colors->push_back(osg::Vec4f(0.0,1.0,0.0,0.5));
	colors->push_back(osg::Vec4f(0.0,0.0,1.0,0.5));
	colors->push_back(osg::Vec4f(1.0,1.0,0.0,0.5));


	// ѹ��һ��������
	normal->push_back(osg::Vec3(0.0,-1.0,0.0));


	return geode;

}



int TestGeom(){

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> group = new osg::Group();


	group->addChild(CreateSimple());
	group->addChild(osgDB::readNodeFile("glider.osg"));

	viewer->setSceneData(group);

	return viewer->run();
}