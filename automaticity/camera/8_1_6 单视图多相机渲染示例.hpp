// stdafx.h

#include <osg/Node>
#include <osg/Geode>  
#include <osg/Geometry>
#include <osg/Group>

#include <osg/MatrixTransform> //�ƶ��ڵ�ľ����࣬��õ��ƶ��ڵ���ࡣ���涯����ת���ƽڵ�
#include <osg/PositionAttitudeTransform> //λ�ñ任�ڵ��࣬�ṩģ�͵�λ�ñ任����С���š�ԭ��λ�õ����á�����ϵ�ı任
#include <osg/Camera> //����ڵ㣬����OSG�е�ģ�͡�����ͼ��������Ĺ�����Ҫ��ͨ�����ֱ任ʵ�ֵ�

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
#include <osgViewer/Viewer> //ֻ������ͼ������ͼ����ͬʱ������������Ⱦ��Ҳ�����ڶര������Ⱦ
#include <osgViewer/ViewerEventHandlers> //�¼�����
//.cpp
 
/*
 *�����ڶ������Ⱦ
 *
 *����ͼ�λ����Ĳ��裺
 *ͨ��WindowingSystemInterface��õ�ϵͳ���ڽӿڣ���ϵͳ�ӿ���Ҫ��Ϊ�˹�����ϵͳ��ͼ�λ���
 *������Ҫ����ͼ�λ������ԵĲ���
 *ͨ��ͼ�λ������Դ���ͼ�λ���
 *ͨ��ͼ�λ�����������
 *
 *�����ӿڵĹؼ����裺
 *����ͼ�λ���������һ���豸�����Ķ���
 *���ö����һ���������
 *�����ӿ�λ��
 *��������������
 */
void singleWindowMultipleCameras(osg::ref_ptr<osgViewer::Viewer> viewer)
{
	//��������ϵͳ�ӿڣ����õײ�API�õ���ͼ�豸���������ԣ�
	osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();
 
	if (!wsi)
	{
		osg::notify(osg::NOTICE) << "Error, no WindowSystemInterface available, cannot create windows." << std::endl;
		return;
	}
 
	//�õ���ǰ���ڷֱ���
	unsigned int width, height;
	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);
 
	//����ͼ�λ������ԣ�����Ҫ������ͼ���豸�����ģ�
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
	//����ͼ���豸�ĳߴ�
	traits->x = 0;
	traits->y = 0;
	traits->width = 1000;
	traits->height = 1000;
	//��ʾ������
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
 
	//����ͼ�λ���������ͼ���豸�����ģ�
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
 
	if (gc->valid())
	{
		osg::notify(osg::INFO) << "GraphicsWindow has been created successfully." << std::endl;
 
		gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.6f, 1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		osg::notify(osg::NOTICE) << "GraphicsWindow has not been created successfully." << std::endl;
	}
 
	//���õ�һ���ӿڣ�����cameraMaster�������
	osg::ref_ptr<osg::Camera> cameraMaster = viewer->getCamera();
	//����ͼ�λ���
	cameraMaster->setGraphicsContext(gc.get());
 
	//���ݷֱ���ȷ�����ʵ�ͶӰ����֤��ʾ��ͼ�β�����
	double fovy, aspectRatio, zNear, zFar;
	cameraMaster->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	double newAspectRatio = double(traits->width) / double(traits->height);
	double aspectRatioChange = newAspectRatio / aspectRatio;
	if (aspectRatioChange != 1.0)
	{
		cameraMaster->getProjectionMatrix() *= osg::Matrix::scale(1.0 / aspectRatioChange, 1.0, 1.0);
	}
 
	//�����ӿ�λ��
	cameraMaster->setViewport(new osg::Viewport(0, 0, 800, 800));
	//�����Ƿ�ʹ��˫���棬ȷ�������ĸ����棬���ʹ��˫���棬�Ȼ���ǰ����
	GLenum bufferMaster = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	//���û�����������ÿ֡��ʼ����ʱ�������ĸ����棩
	cameraMaster->setDrawBuffer(bufferMaster);
	cameraMaster->setReadBuffer(bufferMaster);
 
	//���õڶ����ӿڣ��������������
	osg::ref_ptr<osg::Camera> cameraClient = new osg::Camera();
	//����ͼ�λ���
	cameraClient->setGraphicsContext(gc.get());
	//�����ӿ�λ�ã��ӿڵ�����Ϊ���½�Ϊ����ԭ��
	cameraClient->setViewport(new osg::Viewport(0, 0, 400, 400));
	//�����Ƿ�ʹ��˫���棬ȷ�������ĸ����棬���ʹ��˫���棬�Ȼ���ǰ����
	GLenum bufferClient = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	//���û�����������ÿ֡��ʼ����ʱ�������ĸ����棩
	cameraClient->setDrawBuffer(bufferClient);
	cameraClient->setReadBuffer(bufferClient);
 
	//��Ӵ������
	viewer->addSlave(cameraClient, osg::Matrix::scale(aspectRatio, 1.0, 1.0), osg::Matrix());
}
 
int TestSingleViewMultipleCamera()
{
	osg::ref_ptr<osg::Node> root = osgDB::readNodeFile("cow.osg");
 
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	//���õ���ͼ�������Ⱦ
	singleWindowMultipleCameras(viewer.get());
 
	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
 
	viewer->getCamera()->setNodeMask(1);

	viewer->setSceneData(root.get());
	viewer->realize();
 
	return viewer->run();
}