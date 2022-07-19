#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
/* ���Ʋ���Ⱦ���������Ҫ����
(1)  ����������������  ���㡢�������ꡢ��ɫ������
(2)  ʾ����һ�����������(osg::Geometry), ���� �����������顢�����������顢��ɫ���顢 �������顢 �󶨷�ʽ�� ���ݽ���
(3)  ����Ҷ�ڵ���Ʋ���Ⱦ
*/
// ����һ���ı��νڵ�
osg::ref_ptr<osg::Node> CreateQuad1(){

	// ����һ��Ҷ�ӽڵ����
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	// ����һ�����������
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

	// �����������飬ע�ⶥ������˳������ʱ���
	osg::ref_ptr<osg::Vec3Array> vertex = new osg::Vec3Array();

	vertex->push_back(osg::Vec3(0.0,0,0.0));
	vertex->push_back(osg::Vec3(1.0,0,0.0));
	vertex->push_back(osg::Vec3(1.0,0,1.0));
	vertex->push_back(osg::Vec3(0.0,0,1.0));

	// ���ö�������
	geometry->setVertexArray(vertex.get());

	// ������������
	osg::ref_ptr<osg::Vec2Array> vt = new osg::Vec2Array();
	// �������
	vt->push_back(osg::Vec2(0.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,0.0f));
	vt->push_back(osg::Vec2(1.0f,1.0f));
	vt->push_back(osg::Vec2(0.0f,1.0f));


	// ������������
	geometry->setTexCoordArray(0,vt.get());

	// ������ɫ����
	osg::ref_ptr<osg::Vec4Array> v4_color = new osg::Vec4Array();
	v4_color->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	v4_color->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	v4_color->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	v4_color->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));

	// ������ɫ����
	geometry->setColorArray(v4_color);
	// ������ɫ�İ󶨷�ʽΪ��������
	geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	// ������������
	osg::ref_ptr<osg::Vec3Array> normal = new osg::Vec3Array();
	// ��ӷ���
	normal->push_back(osg::Vec3(0.0f,-1.0f,0.0f));

	// ���÷�������
	geometry->setNormalArray(normal.get());
	geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

	// ���ͼԪ������ͼԪΪ�ı���
	geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

	// ��ӵ�Ҷ�ӽڵ�
	geode->addDrawable(geometry);

	return geode;
}


void TestQuad1(){
	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	root->addChild(CreateQuad1());

	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
}

