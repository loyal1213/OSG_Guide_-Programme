// stdafx.h 

#include <osg/Node> //�ڵ���
#include <osg/Geode> //�Ǹ����νڵ㣬����˵��һ������Group�ڵ㣬һ��Ŀɻ��Ƽ����嶼��ͨ����������root������Ⱦ����OSG���λ��Ƶ���߹���ڵ�
#include <osg/Group> //�Խڵ�����֯���ã�һ����Ϊ���ڵ���߸��ڵ����
#include <osg/Geometry> //�������Ƽ������࣬�û����ƻ����ļ�����

#include <osgDB/ReadFile>
#include <osgDB/WriteFile> 

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers> //�¼�����

#include <osgGA/StateSetManipulator> //�¼���Ӧ�࣬����Ⱦ״̬���п���

#include <osgUtil/Optimizer> //�Ż���
#include <osgUtil/DelaunayTriangulator> //Delaunay������
//.cpp
// �����Ļ�ͼ��Ԫ�����򵥵ļ����塪�����ӵļ����塪�����ӵĳ�����

/*
�����������鲢�������
��������������ԣ���ѡ��
����osgUtil::DelaunayTriangulator���������������
������������󣬰���������������ɵĻ���ͼԪ��ӵ���������
��w����ʾ����ģ�ͣ����Կ��������ܶȲ�ͬ
���ڹ�������Ĭ�Ͽ�������תģ�Ϳ��Կ�����ͬ��λ������ʾ��ͬ
��l����ʾ�رչ��գ���תģ�ͣ����Կ������Ȳ���
*/

/*

���裺

1�������������飻

2������osgUtil::DelaunayTriangulator����󲢳�ʼ���������飬ͬʱ������������

3������һ����������󣬰�osgUtil::DelaunayTriangulator��������ɵĻ���ͼԪ���뵽�������У�������������ʱ��������������������㡢�ߡ�����Ρ�
*/

osg::Drawable *createTriangulate()
{
	//������������
	osg::ref_ptr<osg::Vec3Array> coords = new osg::Vec3Array();
	//��ͬ��ǰ������������������ݵķ������Ȼ�ȡ�����ֵ������Ϊ���飬Ȼ������ֵ��ӵ���������
	//���ö����ֵ
	float vertex[][3] = { -5.0f,-5.0f, 0.4f,
		1.0f, -5.6f, 0.0f,
		5.0f, -4.0f, -0.5f,
		-6.2f, 0.0f, 4.2f,
		-1.0f,-0.5f, 4.8f,
		4.3f, 1.0f, 3.0f,
		-4.8f, 5.4f, 0.3f,
		0.6f, 5.1f,-0.8f,
		5.2f, 4.5f, 0.1f };
	//���㶥������Ĵ�С
	unsigned int n = sizeof(vertex) / sizeof(float[3]);
	//��Ӷ�������
	for (unsigned int i = 0; i < n; i++){
		coords->push_back(osg::Vec3(vertex[i][0], vertex[i][1], vertex[i][2]));
	}

	//������ɫ����
	osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array();
	//�����ɫ����  
	color->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	color->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	color->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	color->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	color->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	color->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	color->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	color->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	color->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));

	//����Delaunay����������
	osg::ref_ptr<osgUtil::DelaunayTriangulator> dt = new osgUtil::DelaunayTriangulator(coords.get());
	//����������
	dt->triangulate();

	//����������
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
	//���ö�������
	geometry->setVertexArray(coords.get());
	//������ɫ����
	geometry->setColorArray(color.get());
	//������ɫ�İ󶨷�ʽΪ��������
	geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	//��ӵ���ͼ��Ԫ
	geometry->addPrimitiveSet(dt->getTriangles());

	return geometry.release();
}

int TestDelaunayTriangulator()
{
	//��ӵ�Ҷ�ڵ�
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(createTriangulate());

	//��ӵ����ڵ�
	osg::ref_ptr<osg::Group> root = new osg::Group();
	root->addChild(geode.get());

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	//����鿴�ڶ����֮���л����Բ鿴������
	viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));
	viewer->setSceneData(root.get());
	viewer->realize();
	return viewer->run();
}