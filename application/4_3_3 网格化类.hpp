#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/ShapeDrawable>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

/* Shape����������״���Ļ��ࡣShape�ȿ����ڼ��ú���ײ���Ҳ�����ڶ�������Եز������������Щ������״��
�������̳���Shape��: 
TriangleMesh 
Sphere 
InfinitePlane 
HeightField 
Cylinder 
Cone 
CompositeShape 
Box 
Ϊ��ʹ��Щ��״���Ա���Ⱦ��������Ҫ�����Ǻ�Drawable���ʵ������������ShapeDrawable���ṩ�������Ĺ��ܡ�
�����̳���Drawable���������ǰ�Shapeʵ�����ӵ����Ա���Ⱦ�Ķ����ϡ�
��ȻShapeDrawable��̳���Drawable��ShapDrawableʵ���Ϳ��Ա��ӵ�Geode��ʵ���ϡ�
*/

// ����һ���ı��νڵ�
osg::ref_ptr<osg::Node> CreateShape(){

	// ����һ��Ҷ�ӽڵ����
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	// ���ð뾶�͸߶�
	float radius = 0.8f, height = 1.0f;

	// ������ϸ�ȶ��󣬾�ϸ�ȶ���Խ�ߣ�ϸ�־�Խ��
	osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints();
	// ���þ�ϸ��Ϊ 0.5f
	hints->setDetailRatio(0.5f);

	// ���һ�����壬��һ������ΪԤ���弸������󣬵ڶ����Ǿ��ȣ� Ĭ��Ϊ 0
	geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),radius),hints.get()));

	// ���һ��������
	geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(2.0f,0.0f,0.0f),2*radius),hints.get()));

	// ���һ��Բ׶
	geode->addDrawable(new osg::ShapeDrawable(new osg::Cone(osg::Vec3(4.0f,0.0f,0.0f),radius,height),hints.get()));

	// ���һ��Բ����
	geode->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(6.0f,0.0f,0.0f),radius,height),hints.get()));

	// ���һ��̫�ղ�
	geode->addDrawable(new osg::ShapeDrawable(new osg::Capsule(osg::Vec3(8.0f,0.0f,0.0f),radius,height),hints.get()));

	return geode;
}


void TestShape(){
	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	root->addChild(CreateShape());

	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
}

