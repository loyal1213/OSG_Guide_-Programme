// stdafx.h

#include <osg/Node>
#include <osg/Geode>  
#include <osg/Geometry>
#include <osg/Group>

#include <osg/MatrixTransform> //�ƶ��ڵ�ľ����࣬��õ��ƶ��ڵ���ࡣ���涯����ת���ƽڵ�
#include <osg/PositionAttitudeTransform> //λ�ñ任�ڵ��࣬�ṩģ�͵�λ�ñ任����С���š�ԭ��λ�õ����á�����ϵ�ı任
#include <osg/Camera>  //����ڵ㣬����OSG�е�ģ�͡�����ͼ��������Ĺ�����Ҫ��ͨ�����ֱ任ʵ�ֵ�

#include <osgGA/TrackballManipulator> 

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers> //�¼�����
#include <osgViewer/CompositeViewer>


//.cpp

/*
*����ͼ�����Ⱦ
*
*��OSG�У�����ͼ�Ĺ�����ͨ��osgViewer::CompositeViewer��ʵ�ֵģ�����������ͼ�Ĺ���ͬ���������̳���osgViewer::ViewerBase��
*
*��ͼ�Ĵ�����
*����ͼ�λ�������
*����ͼ�λ�������
*������ͼ
*������ͼ��������
*�����ݷֱ���ȷ�����ʵ�ͶӰ����֤��ʾ��ͼ�β����Σ�
*��������ӿڼ�ͼ�λ���
*���ò�����
*������¼�����
*/


osg::ref_ptr<osg::GraphicsContext> CreateGraphicsContext(osg::ref_ptr<osg::GraphicsContext::Traits> traits)
{
	//����ͼ�λ�������
	//osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
	traits->x = 100;
	traits->y = 100;
	traits->width = 900;
	traits->height = 700;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	//����ͼ�λ�������
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
	if (gc->valid())
	{
		osg::notify(osg::INFO) << "GraphicsWindow has been created successfully." << std::endl;

		//���������ɫ����ɫ����Ȼ���
		gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 1.6f, 1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		osg::notify(osg::NOTICE) << "GraphicsWindow has not been created successfully." << std::endl;
	}

	return gc.get();
}

//��ͼһ
void View1(osg::ref_ptr<osgViewer::CompositeViewer> viewer, osg::ref_ptr<osg::Node> node, osg::ref_ptr<osg::GraphicsContext::Traits> traits, osg::ref_ptr<osg::GraphicsContext> gc)
{
	//������ͼһ
	osg::ref_ptr<osgViewer::View> view = new osgViewer::View();
	viewer->addView(view.get());

	//������ͼ��������
	view->setSceneData(node.get());

	//��������ӿڼ�ͼ�λ���
	view->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width / 2, traits->height / 2));
	view->getCamera()->setGraphicsContext(gc.get());

	//���ò�����
	view->setCameraManipulator(new osgGA::TrackballManipulator);

	//����¼�����
	view->addEventHandler(new osgViewer::StatsHandler);
	view->addEventHandler(new osgViewer::WindowSizeHandler);
	view->addEventHandler(new osgViewer::ThreadingHandler);
	view->addEventHandler(new osgViewer::RecordCameraPathHandler);
}

//��ͼ��
void View2(osg::ref_ptr<osgViewer::CompositeViewer> viewer, osg::ref_ptr<osg::Node> node, osg::ref_ptr<osg::GraphicsContext::Traits> traits, osg::ref_ptr<osg::GraphicsContext> gc)
{
	//������ͼ��
	osg::ref_ptr<osgViewer::View> view = new osgViewer::View();
	viewer->addView(view.get());

	//������ͼ��������
	view->setSceneData(node.get());

	//��������ӿڼ�ͼ�λ���
	view->getCamera()->setViewport(new osg::Viewport(traits->width / 2, 0, traits->width / 2, traits->height / 2));
	view->getCamera()->setGraphicsContext(gc.get());

	//���ò�����
	view->setCameraManipulator(new osgGA::TrackballManipulator);
}

//��ͼ��
void View3(osg::ref_ptr<osgViewer::CompositeViewer> viewer, osg::ref_ptr<osg::Node> node, osg::ref_ptr<osg::GraphicsContext::Traits> traits, osg::ref_ptr<osg::GraphicsContext> gc)
{
	//������ͼ��
	osg::ref_ptr<osgViewer::View> view = new osgViewer::View();
	viewer->addView(view.get());

	//������ͼ��������
	view->setSceneData(node.get());

	//���ݷֱ���ȷ�����ʵ�ͶӰ����֤��ʾ��ͼ�β�����
	double fovy, aspectRatio, zNear, zFar;
	view->getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	double newAspectRatio = double(traits->width) / double(traits->height / 2);
	double aspectRatioChange = newAspectRatio / aspectRatio;
	if (aspectRatioChange != 1.0)
	{
		view->getCamera()->getProjectionMatrix() *= osg::Matrix::scale(1.0 / aspectRatioChange, 1.0, 1.0);
	}

	//��������ӿڼ�ͼ�λ���
	view->getCamera()->setViewport(new osg::Viewport(traits->width / 2, traits->height / 2, traits->width / 2, traits->height / 2));
	view->getCamera()->setGraphicsContext(gc.get());

	//���ò�����
	view->setCameraManipulator(new osgGA::TrackballManipulator);
}

int TestCompositeViewer()
{
	//��ȡģ��
	osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("cow.osg");
	osg::ref_ptr<osg::Node> cessna = osgDB::readNodeFile("cessna.osg");

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(cow.get());
	optimizer.optimize(cessna.get());

	//���ò�����ͼ�λ���
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
	osg::ref_ptr<osg::GraphicsContext> gc = CreateGraphicsContext(traits);

	//����CompositeViewer����
	osg::ref_ptr<osgViewer::CompositeViewer> viewer = new osgViewer::CompositeViewer();
	View1(viewer, cow.get(), traits.get(), gc.get());
	View2(viewer, cessna.get(), traits.get(), gc.get());
	View3(viewer, cessna.get(), traits.get(), gc.get());

	viewer->realize();

	return viewer->run();
}

