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


// 定义一个全局变量
osg::ref_ptr<osg::Image> image_c = new osg::Image();

// 得到抓图
struct CaptureDrawCallback:public osg::Camera::DrawCallback{

	CaptureDrawCallback(osg::ref_ptr<osg::Image> image) : image_(image){}

	virtual ~CaptureDrawCallback(){}

	void operator () (const osg::Camera& camera)  const{
		// 得到窗口系统接口
		osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();

		unsigned int width = 0, height = 0;
		// 得到屏幕分辨率
		wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0),width,height);
		// 分配一个 image
		image_->allocateImage(width,height,1,GL_RGB,GL_UNSIGNED_BYTE);
		// 读取像素信息抓图
		image_->readPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE);
	
	}

private:
	osg::ref_ptr<osg::Image> image_ ;
};

// 抓图事件处理
class ImageHandler:public osgGA::GUIEventHandler{
public:
	ImageHandler(){}
	virtual ~ImageHandler(){}

	 virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa) { 
		 osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*> (&aa);

		 if (viewer == nullptr){
			 return false;
		 }

		 // 定义一个静态变量
		 static int screen_capure_sequence = 0;

		 switch(ea.getEventType()){
		 case osgGA::GUIEventAdapter::KEYDOWN:{
			 if (ea.getKey() == 'c' || ea.getKey() == 'C'){
				 char filename[128];
				 // 明确一个合理的文件名，以保证能够连续抓图
				 sprintf(filename,"ScreenShot%04d.bmp",screen_capure_sequence);
				 screen_capure_sequence++;
				 // 写入文件
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

	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// 读取模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");

	root->addChild(node.get());

	// 得到相机
	osg::ref_ptr<osg::Camera> camera = viewer->getCamera();

	// 设置相机事件回调
	camera->setPostDrawCallback(new CaptureDrawCallback(image_c.get()));

	// 添加抓图事件
    viewer->addEventHandler(new ImageHandler());

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}




