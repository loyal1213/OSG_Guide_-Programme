#include <osgViewer/Viewer>


#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Billboard>
#include <osg/Texture2D>
#include <osg/Image>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
#include <osg/MatrixTransform>

#include <osg/AutoTransform>
#include <osgText/Text>


osg::ref_ptr<osg::Node> CreateAutoTransform(osg::Vec3 & position,float size, std::string & label, osg::AutoTransform::AutoRotateMode automode, osgText::Text::AxisAlignment axisAlignment){
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();


	// 字体
	std::string font("font/cour.ttf");

	// 创建Text对象
	osg::ref_ptr<osgText::Text> text = new osgText::Text();
	geode->addDrawable(text.get());

	// 设置字体
	text->setFont(font);
	// 设置字体分辨率
	text->setFontResolution(128.0f,128.0f);
	// 设置字体大小
	text->setCharacterSize(size);
	// 设置字体对齐方式
	text->setAlignment(osgText::Text::CENTER_CENTER);
	// 设置方向
	text->setAxisAlignment(axisAlignment);
	// 设置文字
	text->setText(label);

	// 关闭光照
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

	// 创建自动变换节点
	osg::ref_ptr<osg::AutoTransform> at = new osg::AutoTransform();
	// 添加子节点
	at->addChild(geode.get());


	// 设置自动变换方式
	at->setAutoRotateMode(automode);
	// 根据屏幕大小来缩放节点，默认为 false, 设置为 true 时, 节点无法缩放
	at->setAutoScaleToScreen(false);
	// 设置缩放的最大值和最小比例
	at->setMinimumScale(0.0f);
	at->setMaximumScale(5.0f);

	// 设置位置
	at->setPosition(position);
	return at.get();
}




int TestAutoTransform(){

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	std::string text("Fly To Sky");

	/*
	3 种变换模式：
	rotate_to_screen	  自动朝向屏幕		
	rotate_to_camera      自动朝向相机
	no_rotation           无			
	*/


	// 添加 rotate_to_screen 模式变换节点  
	root->addChild(CreateAutoTransform(osg::Vec3(0.0f,0.0f,10.0f), 10.0f, text, osg::AutoTransform::ROTATE_TO_SCREEN, osgText::Text::XY_PLANE));

	// 添加 no_rotation 模式变换节点
	root->addChild(CreateAutoTransform(osg::Vec3(0.0f,0.0f,20.0f), 20.0f, text, osg::AutoTransform::NO_ROTATION, osgText::Text::YZ_PLANE));

	// 添加 rotate_to_camera 模式变换节点
	root->addChild(CreateAutoTransform(osg::Vec3(0.0f,0.0f,30.0f), 10.0f, text, osg::AutoTransform::ROTATE_TO_CAMERA, osgText::Text::XY_PLANE));


	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}