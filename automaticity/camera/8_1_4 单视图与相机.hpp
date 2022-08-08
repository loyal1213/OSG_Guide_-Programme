/*
һ����ͼ�����
����Ŀ�곡����ͼ�ı任����������������������գ���Ҫ����Ϊ��

��1��������̶������ż��ϣ�������׼��������ͼ�任����

��1���Գ������а��ţ�ʹ��Ƭ�и�����λ�ڶ�����ϣ����λ�ã�ģ�ͱ任����

��3��ѡ���������ͷ���������Ŵ�����ͶӰ�任����

��4��ȷ��������Ƭ�Ĵ�С����Ŵ��ӿڱ任����

��������沽��֮�󣬾Ϳ��Ի��Ƴ����ˡ���ͼ���Լ����Ϊһ���������ա�

�ӿڣ�Viewport��������������ϵ�е���ά���徭��һϵ�м��α仯������ƽ�ơ�ѡ��������ȣ���Ϊ��ʹ��ʾ���������Ժ��ʵ�λ�á���С�ͷ�����ʾ����������Ҫͨ��ͶӰ��Ȼ����һ����ά�Ӿ��壬��������вü���ֻʹͶӰ���Ӿ����ڵĲ�����ʾ������Ȼ������Ļ�����ڶ���һ�����Σ���Ϊ�ӿڡ��Ӿ���ͶӰ���ͼ�ξ����ӿ�����ʾ�������һЩ�ʵ��任��ʹͼ������Ļ����ϵ����ʾ��OSG����SimpleViewer��Viewer��CompositeViewer�����ӿ��ࡣ
����osg::Camera��
osg::Camera��̳���osg::Transform��osg::CullSetting�࣬��������OSG�е�ģ�͡�����ͼ��������Ĺ�����Ҫ��ͨ�����ֱ任ʵ�ֵġ�

1���ӵ�任�������ӵ�ķ����λ�á�Ĭ������£��ӵ㶨λΪ����ԭ�㣬ָ��Y������

2��ģ�ͱ任����ģ�͵�λ�á���С�ͽǶȵı任����λ�ñ任�ڵ㣨osg::PositionAttitudeTransform���;���任�ڵ㣨osg::MatrixTransform����

3��ͶӰ�任�����������ģ���Ӿ���ת���󣬳����е������������ϣ����λ���ϣ���������ʾ��ֻ���ö�άͼ����ʾ��ά���壬���Ҫ��ͶӰ������ά������ʵ�ϣ�ͶӰ�任��Ŀ�ľ��Ƕ���һ���Ӿ��壬ʹ�Ӿ��������Ĳ��ֱ��ü��������ս���ͼ���ֻ���Ӿ����ڵ��йز��֡�����͸��ͶӰ��Perspective Projection��������ͶӰ��Orthographic Projection����

4���ӿڱ任�����Ӿ�����ͶӰ��������ʾ�ڶ�ά���ӿ�ƽ���ϡ������������α任��ͶӰ�任�Ͳü��任���������ʾ����Ļ������ָ���������ڣ��������ͨ��Ϊ���Σ���Ϊ�ӿڡ�

5���ü��任����OSG�У�Ĭ����6���ü�ƽ����ȥ��û�б�Ҫ��ʾ�����塣�����Զ��������ü�ƽ�����ü�������ü�ƽ��ķ����кܶ࣬��osg::Scissor��osg::ClipPlane�ȡ�

*/

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
 
void TestSingleCameras()
{
	osg::ref_ptr<osg::Node> root = osgDB::readNodeFile("cow.osg");
 
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();


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
	traits->width = width;
	traits->height = height;
	//��ʾ������
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	//����ͼ�λ���������ͼ���豸�����ģ�
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

	if (gc->valid()){
		osg::notify(osg::INFO) << "GraphicsWindow has been created successfully." << std::endl;

		gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 1.6f, 1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}else{
		osg::notify(osg::NOTICE) << "GraphicsWindow has not been created successfully." << std::endl;
	}

	//���õ�һ���ӿڣ�����cameraMaster�������
	osg::ref_ptr<osg::Camera> cameraMaster = viewer->getCamera();
	

	//���ݷֱ���ȷ�����ʵ�ͶӰ����֤��ʾ��ͼ�β�����
	double fovy, aspectRatio, zNear, zFar;


	/*�Գ�͸���Ӿ���ĺ���*/
	//ʹ�öԳ�͸���Ӿ�����Ҫע����Ұ�Ƕȣ�����������ûᵼ��ͼ�εı��Σ���������������
	//����/��ȡһ���Գ�͸����ͼ��ƽ��ͷ��ľ��󣬲��뵱ǰ�������
	//��1��������ʾ yzƽ����Ұ�Ƕ�
	//��2��������ʾ ƽ��ͷ���ݺ��
	//��3��������ʾ �ӵ����ü���ľ���
	//��4��������ʾ �ӵ��Զ�ü���ľ���
	//����Ϊ����
	cameraMaster->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	double newAspectRatio = double(traits->width) / double(traits->height);
	double aspectRatioChange = newAspectRatio / aspectRatio;
	if (aspectRatioChange != 1.0){
		cameraMaster->getProjectionMatrix() *= osg::Matrix::scale(1.0 / aspectRatioChange, 1.0, 1.0);
	}

	//�����ӿ�λ��
	cameraMaster->setViewport(new osg::Viewport(0, 0, width, height));
	//�����Ƿ�ʹ��˫���棬ȷ�������ĸ����棬���ʹ��˫���棬�Ȼ���ǰ����
	GLenum bufferMaster = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	//���û�����������ÿ֡��ʼ����ʱ�������ĸ����棩
	cameraMaster->setDrawBuffer(bufferMaster);
	cameraMaster->setReadBuffer(bufferMaster);


	//����ͼ�λ���
	cameraMaster->setGraphicsContext(gc.get());


	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
 
	viewer->setSceneData(root.get());
	viewer->realize();
 
	viewer->run();
}