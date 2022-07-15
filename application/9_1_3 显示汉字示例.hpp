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
osg::ref_ptr<osg::Geode> CreateText(){

	

	// 1. 创建一个 osgText::Font 字体对象，并读取字体
	osg::ref_ptr<osgText::Font> font = new osgText::Font();
	// 读取文字信息
	font = osgText::readFontFile("simhei.ttf");

	// 2. 创建一个osgText::Text 对象，设置字体属性
	osg::ref_ptr<osgText::Text> text = new osgText::Text();
	// 设置字体文件
	text->setFont(font);
	// 设置文字信息
	text->setText(L"http://OsgChina.org -- OpengSceneGraph 中国官方");
	// 设置字体大小
	text->setCharacterSize(0.5f);
	text->setAutoRotateToScreen(true);
	// 设置字体颜色
	text->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	osg::Vec3f position = osg::Vec3(0.0f, -10.0f, 0.0f);
	text->setPosition(position);
	// 设置对齐方式
	text->setAlignment(osgText::Text::CENTER_TOP);
	// 设置旋转方式
	text->setAxisAlignment(osgText::Text::SCREEN);
	
	// 添加到叶节点中
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	// 调用 addDrawable 
	geode->addDrawable(text);

	return geode.get();
}



void TestOsgText(){
	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	root->addChild(CreateText());

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
}