#include <osgViewer/Viewer>
#include <osgViewer/GraphicsWindow>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Camera>
#include <osg/Image>
#include <osg/BufferObject>
#include <osg/GraphicsContext>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>


#include <iostream>


// ����һ��ȫ�ֱ���
osg::ref_ptr<osg::Image> image_c = new osg::Image();

// �õ�ץͼ
struct CaptureDrawCallback:public osg::Camera::DrawCallback{

	CaptureDrawCallback(osg::ref_ptr<osg::Image> image) : image_(image){}

	virtual ~CaptureDrawCallback(){}

	void operator () (const osg::Camera& camera)  const{
		// �õ�����ϵͳ�ӿ�
		osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();

		unsigned int width = 0, height = 0;
		// �õ���Ļ�ֱ���
		wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0),width,height);
		// ����һ�� image
		image_->allocateImage(width,height,1,GL_RGB,GL_UNSIGNED_BYTE);
		// ��ȡ������Ϣץͼ
		image_->readPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE);
	
	}

private:
	osg::ref_ptr<osg::Image> image_ ;
};

// ץͼ�¼�����
class ImageHandler:public osgGA::GUIEventHandler{
public:
	ImageHandler(){}
	virtual ~ImageHandler(){}

	 virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa) { 
		 osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*> (&aa);

		 if (viewer == nullptr){
			 return false;
		 }

		 // ����һ����̬����
		 static int screen_capure_sequence = 0;

		 switch(ea.getEventType()){
		 case osgGA::GUIEventAdapter::KEYDOWN:{
			 if (ea.getKey() == 'c' || ea.getKey() == 'C'){
				 char filename[128];
				 // ��ȷһ��������ļ������Ա�֤�ܹ�����ץͼ
				 sprintf(filename,"ScreenShot%04d.bmp",screen_capure_sequence);
				 screen_capure_sequence++;
				 // д���ļ�
				 osgDB::writeImageFile(*(image_c.get()),filename);
			 }
			 break;

		 default:
			 return false;
											  }
		 }

		 return true;
	 }

};



int TestEventHandle(){

	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// ��ȡģ��
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	root->addChild(node.get());

	// �õ����
	osg::ref_ptr<osg::Camera> camera = viewer->getCamera();

	// ��������¼��ص�
	camera->setPostDrawCallback(new CaptureDrawCallback(image_c.get()));

	// ���ץͼ�¼�
    viewer->addEventHandler(new ImageHandler());

	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}




