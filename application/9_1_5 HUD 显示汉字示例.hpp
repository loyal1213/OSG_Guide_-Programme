// hud （heads up display） 即抬头显示，这种技术最早应用在军事转斗机上
#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgText/Text>
#include <osgText/Font>

#include <osgUtil/Optimizer>

// 创建文字
osg::ref_ptr<osg::Camera> CreateHUDText(){

	osg::ref_ptr<osg::Camera> camera = new osg::Camera();

	// 设置投影矩阵
	camera->setProjectionMatrix(osg::Matrix::ortho2D(0,1280,0,800));

	// 设置视图矩阵，同时确保不被场景中其它图形位置变换影响，使用绝对帧引用
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrix(osg::Matrix::identity());

	// 清除深度缓存
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);

	// 设置渲染顺序，在主摄像机之前渲染
	camera->setRenderOrder(osg::Camera::POST_RENDER);

	// 设置为不接收事件，始终不得到焦点
	camera->setAllowEventFocus(false);


	// 1. 创建一个 osgText::Font 字体对象，并读取字体
	osg::ref_ptr<osgText::Font> font = osgText::readFontFile("fonts/simhei.ttf");

	// 添加到叶节点中
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	osg::ref_ptr<osg::StateSet> stateset = geode->getOrCreateStateSet();
	// 关闭光照
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	// 关闭深度测试
	stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);

	// 设置文字
	osg::ref_ptr<osgText::Text> text = new osgText::Text();

	// 设置字体文件
	text->setFont(font);
	// 设置文字信息
	text->setText(L"http://OsgChina.org -- OpengSceneGraph 中国官方");
	// 设置字体大小
	text->setCharacterSize(20.0f);
	text->setAutoRotateToScreen(true);
	// 设置字体颜色
	text->setColor(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
	osg::Vec3f position = osg::Vec3(50.0f, 600.0f, 0.0f);
	text->setPosition(position);
	// 设置对齐方式
	text->setAlignment(osgText::Text::CENTER_TOP);
	// 设置旋转方式
	text->setAxisAlignment(osgText::Text::SCREEN);

	geode->addDrawable(text.get());

	camera->addChild(geode.get());

	return camera.get();
}



void TestHUDText(){
	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// root->addChild(osgDB::readImageFile("logo.gif")->asNode());
	root->addChild(CreateHUDText());

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
}