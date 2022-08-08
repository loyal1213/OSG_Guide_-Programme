// stdafx.h

#include <osg/Node>
#include <osg/Geode>  
#include <osg/Geometry>
#include <osg/Group>

#include <osg/MatrixTransform> //�ƶ��ڵ�ľ����࣬��õ��ƶ��ڵ���ࡣ���涯����ת���ƽڵ�
#include <osg/PositionAttitudeTransform> //λ�ñ任�ڵ��࣬�ṩģ�͵�λ�ñ任����С���š�ԭ��λ�õ����á�����ϵ�ı任
#include <osg/Camera>  //����ڵ㣬����OSG�е�ģ�͡�����ͼ��������Ĺ�����Ҫ��ͨ�����ֱ任ʵ�ֵ�

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer> //����ͼ�Ĺ�����������ͼ�Ĺ���ͬ������

//.cpp

//��ͼ����
osg::ref_ptr<osgViewer::Viewer> MultipleView(osg::ref_ptr<osg::Node> node, int i)
{
	int xoffset = 50;
	int yoffset = 200;

	//����ͼ�λ�������
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
	//������ͼ���ڵĺ�����λ�ò�ͬ��������ص�
	if (i == 1){
		traits->x = xoffset + 0;
	}else{
		traits->x = xoffset + 650;
	}

	traits->y = yoffset + 0;
	traits->width = 600;
	traits->height = 480;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	//������ͼ
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	//�������
	osg::ref_ptr<osg::Camera> camera = viewer->getCamera();

	//����ͼ�λ�������
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());	
	//����ͼ�λ���
	camera->setGraphicsContext(gc.get());

	//�����ӿ�
	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));

	//���û���
	GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	//���û�����������ÿ֡��ʼ����ʱ�������ĸ����棩
	camera->setDrawBuffer(buffer);
	camera->setReadBuffer(buffer);

	//���ó�������
	viewer->setSceneData(node.get());

	return viewer.get();
}

int TestMultipleViewMultipleWindows()
{
	//��ȡģ�ͣ��ֱ���Ϊ������ͼ�Ľڵ�
	osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("cow.osg");
	osg::ref_ptr<osg::Node> cessna = osgDB::readNodeFile("cessna.osg");

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(cow.get());
	optimizer.optimize(cessna.get());

	//����������ͼ
	osg::ref_ptr<osgViewer::Viewer> viewer1 = MultipleView(cow, 1);
	osg::ref_ptr<osgViewer::Viewer> viewer2 = MultipleView(cessna, 2);

	//����һ��CompositeViewer����
	osg::ref_ptr<osgViewer::CompositeViewer> viewer = new osgViewer::CompositeViewer();

	//�����ͼ
	viewer->addView(viewer1.get());
	viewer->addView(viewer2.get());
	viewer->realize();

	return viewer->run();
}