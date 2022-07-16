#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Camera>
#include <osg/Matrix>
#include <osg/ClusterCullingCallback>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgText/Text>
#include <osgText/Font>
#include <osgText/FadeText>

#include <osgUtil/Optimizer>

#include <iostream>

// 创建文字
osg::ref_ptr<osgText::Text> CreateText1(const std::string &str,osg::Vec3 pos){

   osg::ref_ptr<osgText::FadeText> text = new osgText::FadeText();

   // 设置筛选回调
   osg::Vec3 normal(0.0f,0.0f,1.0f);
   // 设置为聚集筛选，一种类似于背面筛选的场景筛选方法，但是它可以将多个对象组合起来进行统一的背面筛选
   text->setCullCallback(new osg::ClusterCullingCallback(pos,normal,0.0f));

   text->setText(str);
   text->setFont("fonts/arial.ttf");
   text->setPosition(pos);
   text->setCharacterSize(300.0f);
   text->setCharacterSizeMode(osgText::Text::OBJECT_COORDS_WITH_MAXIMUM_SCREEN_SIZE_CAPPED_BY_FONT_HEIGHT);
   text->setAutoRotateToScreen(true);

	return text;
}


// 创建渐变文字
osg::ref_ptr<osg::Node> CreateFadeText(){
	osg::ref_ptr<osg::Group> group = new osg::Group();

	// 关闭深度测试
	group->getOrCreateStateSet()->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);

	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	group->addChild(geode.get());


	// 设置TEXT
	std::vector<std::string> textList;
	textList.push_back("1");
	textList.push_back("2");
	textList.push_back("3");
	textList.push_back("4");
	textList.push_back("5");
	textList.push_back("6");
	textList.push_back("7");
	textList.push_back("8");
	textList.push_back("9");
	textList.push_back("10");
	
	int num_x = 200;
	int delta = 10;

	// 添加 Drawable
	int t = 0;
	for (int i = 100;i <num_x;i+=delta,t++){
		geode->addDrawable(CreateText1(textList[t%textList.size()],osg::Vec3(0,i,100)));
	}

	return group;
}

void TestFadeText(){
	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();


	root->addChild(CreateFadeText());

	root->addChild(osgDB::readNodeFile("lz.osg"));

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
}