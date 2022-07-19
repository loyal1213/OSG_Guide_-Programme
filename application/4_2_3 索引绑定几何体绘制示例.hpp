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
osg::ref_ptr<osg::Node> CreateQuad2(){

	// ����һ��Ҷ�ӽڵ����
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	// ����һ�����������
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

	// �����������飬ע�ⶥ������˳������ʱ���
	osg::ref_ptr<osg::Vec3Array> vertex = new osg::Vec3Array();

	vertex->push_back(osg::Vec3(0.0f,  0.0f, 0.0f));
	vertex->push_back(osg::Vec3(1.0f,  0.0f, 0.0f));
	vertex->push_back(osg::Vec3(1.0f,  0.0f, 1.0f));
	vertex->push_back(osg::Vec3(0.0f,  0.0f, 1.0f));
	vertex->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

	// ���ö�������
	geometry->setVertexArray(vertex.get());


	// �����ı��ζ����������飬ָ����ͼ��ԪΪ�ı��Σ� ע�����˳��
	osg::ref_ptr<osg::DrawElementsUInt> quad = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS,0);
	// �������
	quad->push_back(0);
	quad->push_back(1);
	quad->push_back(2);
	quad->push_back(3);
	
	// ��ӵ�������
	geometry->addPrimitiveSet(quad);

	// ���������ζ����������飬ָ������ͼ��ԪΪ�����Σ�ע�����˳��
	osg::ref_ptr<osg::DrawElementsUInt> triangle = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES,0);
	// �������
	triangle->push_back(4);
	triangle->push_back(0);
	triangle->push_back(3);

	// ��ӵ�������
	geometry->addPrimitiveSet(triangle);


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

	// ������ɫ��������
	osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>* colorIndex = new osg::TemplateIndexArray<unsigned int,osg::Array::UIntArrayType,4,4>();
	// ������ݣ�ע�����˳���붥��˳�� һ һ ��Ӧ
	colorIndex->push_back(0);
	colorIndex->push_back(1);
	colorIndex->push_back(2);
	colorIndex->push_back(3);
	colorIndex->push_back(2);

	/*osg::Vec3Array* vecarray = new osg::Vec3Array;     //������������
	osg::UByteArray* vecindex = new osg::UByteArray;   //������������
	osg::Vec4Array* colarray = new osg::Vec4Array;     //��ɫRGBֵ����
	osg::UByteArray* colindex = new osg::UByteArray;   //��ɫ��������
	osg::Vec3Array* norarray = new osg::Vec3Array;     //������������
	osg::UByteArray* norindex = new osg::UByteArray;   //������������*/

	// ������ɫ����
	geometry->setColorArray(v4_color);
	// ������ɫ�İ󶨷�ʽΪ��������
	geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	// ������ɫ��������
	// geometry->setColorIndices(colorIndex);


	// ������������
	osg::ref_ptr<osg::Vec3Array> normal = new osg::Vec3Array();
	// ��ӷ���
	normal->push_back(osg::Vec3(0.0f,-1.0f,0.0f));

	// ���÷�������
	geometry->setNormalArray(normal.get());
	geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

	// ���ͼԪ������ͼԪΪ�ı���
	// geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

	// ��ӵ�Ҷ�ӽڵ�
	geode->addChild(geometry);

	return geode;
}


void TestQuad2(){
	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	root->addChild(CreateQuad2());

	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
}

