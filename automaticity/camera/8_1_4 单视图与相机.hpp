/*
一、视图与相机
产生目标场景视图的变换过程类似于用相机进行拍照，主要步骤为：

（1）把相机固定在三脚架上，让它对准场景（视图变换）；

（1）对场景进行安排，使照片中各物体位于读者所希望的位置（模型变换）；

（3）选择照相机镜头，并调整放大倍数（投影变换）；

（4）确定最终照片的大小，如放大（视口变换）。

当完成上面步骤之后，就可以绘制场景了。视图可以简单理解为一个场景拍照。

视口（Viewport）：将世界坐标系中的三维物体经过一系列几何变化（包括平移、选择和伸缩等），为了使显示的物体能以合适的位置、大小和方向显示出来，必须要通过投影，然后定义一个三维视景体，对物体进行裁剪，只使投影在视景体内的部分显示出来，然后在屏幕窗口内定义一个矩形，称为视口。视景体投影后的图形就在视口内显示，最后做一些适当变换，使图形在屏幕坐标系下显示。OSG包含SimpleViewer、Viewer、CompositeViewer三大视口类。
二、osg::Camera类
osg::Camera类继承自osg::Transform和osg::CullSetting类，用来管理OSG中的模型——视图矩阵。相机的管理主要是通过各种变换实现的。

1、视点变换：设置视点的方向和位置。默认情况下，视点定位为坐标原点，指向Y正方向。

2、模型变换：对模型的位置、大小和角度的变换。如位置变换节点（osg::PositionAttitudeTransform）和矩阵变换节点（osg::MatrixTransform）。

3、投影变换：经过上面的模型视景的转换后，场景中的物体放在了所希望的位置上，但由于显示器只能用二维图像显示三维物体，因此要靠投影来降低维数。事实上，投影变换的目的就是定义一个视景体，使视景体外多余的部分被裁减掉，最终进入图像的只是视景体内的有关部分。包括透视投影（Perspective Projection）和正视投影（Orthographic Projection）。

4、视口变换：将视景体内投影的物体显示在二维的视口平面上。即将经过几何变换、投影变换和裁剪变换后的物体显示于屏幕窗口内指定的区域内，这个区域通常为矩形，称为视口。

5、裁剪变换：在OSG中，默认了6个裁剪平面以去除没有必要显示的物体。还可以定义其他裁剪平面来裁剪。定义裁剪平面的方法有很多，如osg::Scissor和osg::ClipPlane等。

*/

// stdafx.h

#include <osg/Node>
#include <osg/Geode>  
#include <osg/Geometry>
#include <osg/Group>

#include <osg/MatrixTransform> //移动节点的矩阵类，最常用的移动节点的类。可随动、旋转控制节点
#include <osg/PositionAttitudeTransform> //位置变换节点类，提供模型的位置变换、大小缩放、原点位置的设置、坐标系的变换
#include <osg/Camera> //相机节点，管理OSG中的模型——视图矩阵，相机的管理主要是通过各种变换实现的

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
#include <osgViewer/Viewer> //只允许单视图，单视图可以同时包含多个相机渲染，也可以在多窗口中渲染
#include <osgViewer/ViewerEventHandlers> //事件监听
 
void TestSingleCameras()
{
	osg::ref_ptr<osg::Node> root = osgDB::readNodeFile("cow.osg");
 
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();


	//创建窗口系统接口（调用底层API得到绘图设备、窗口特性）
	osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();

	if (!wsi)
	{
		osg::notify(osg::NOTICE) << "Error, no WindowSystemInterface available, cannot create windows." << std::endl;
		return;
	}

	//得到当前窗口分辨率
	unsigned int width, height;
	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);

	//设置图形环境特性（设置要创建的图形设备上下文）
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
	//设置图形设备的尺寸
	traits->x = 0;
	traits->y = 0;
	traits->width = width;
	traits->height = height;
	//显示标题栏
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	//创建图形环境（创建图形设备上下文）
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

	if (gc->valid()){
		osg::notify(osg::INFO) << "GraphicsWindow has been created successfully." << std::endl;

		gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 1.6f, 1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}else{
		osg::notify(osg::NOTICE) << "GraphicsWindow has not been created successfully." << std::endl;
	}

	//设置第一个视口（创建cameraMaster主相机）
	osg::ref_ptr<osg::Camera> cameraMaster = viewer->getCamera();
	

	//根据分辨率确定合适的投影来保证显示的图形不变形
	double fovy, aspectRatio, zNear, zFar;


	/*对称透视视景体的函数*/
	//使用对称透视视景体需要注意视野角度，如果调整不好会导致图形的变形，即宽屏变形问题
	//设置/获取一个对称透视视图的平截头体的矩阵，并与当前矩阵相乘
	//第1个参数表示 yz平面视野角度
	//第2个参数表示 平截头的纵横比
	//第3个参数表示 视点距近裁减面的距离
	//第4个参数表示 视点距远裁剪面的距离
	//必须为正数
	cameraMaster->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	double newAspectRatio = double(traits->width) / double(traits->height);
	double aspectRatioChange = newAspectRatio / aspectRatio;
	if (aspectRatioChange != 1.0){
		cameraMaster->getProjectionMatrix() *= osg::Matrix::scale(1.0 / aspectRatioChange, 1.0, 1.0);
	}

	//设置视口位置
	cameraMaster->setViewport(new osg::Viewport(0, 0, width, height));
	//根据是否使用双缓存，确定绘制哪个缓存，如果使用双缓存，先绘制前缓存
	GLenum bufferMaster = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	//设置缓冲区（设置每帧开始绘制时，绘制哪个缓存）
	cameraMaster->setDrawBuffer(bufferMaster);
	cameraMaster->setReadBuffer(bufferMaster);


	//设置图形环境
	cameraMaster->setGraphicsContext(gc.get());


	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
 
	viewer->setSceneData(root.get());
	viewer->realize();
 
	viewer->run();
}