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
osg::ref_ptr<osg::Camera> CreaAllKindText(){

	osg::ref_ptr<osg::Camera> camera = new osg::Camera();

	// 设置投影矩阵
	camera->setProjectionMatrix(osg::Matrix::ortho2D(0,1280,0,800));

	// 设置视图矩阵
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrix(osg::Matrix::identity());

	// 清除深度缓存
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);

	// 设置渲染顺序，在主摄像机之前渲染
	camera->setRenderOrder(osg::Camera::POST_RENDER);

	// 设置为不接收事件，始终不得到焦点
	camera->setAllowEventFocus(false);

	// 1. 创建一个 osgText::Font 字体对象，并读取字体
	osg::ref_ptr<osgText::Font> font = osgText::readFontFile("fonts/arial.ttf");

	// 添加到叶节点中
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	osg::ref_ptr<osg::StateSet> stateset = geode->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

	//---------------------------------------------------------------------------------
	// 设置文字输出格式

	// 设置文字输出的格式为 LEFT_TO_RIGHT  从左到右
	{
		// 2. 创建一个osgText::Text 对象，设置字体属性
		osg::ref_ptr<osgText::Text> text = new osgText::Text();
		text->setFont(font);
		text->setCharacterSize(20.0f);
		text->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
		text->setPosition(osg::Vec3(50.0f, 750.0f, 0.0f));	
		
		text->setLayout(osgText::Text::LEFT_TO_RIGHT);
		
		text->setText("text->setLayout(osgText::Text::LEFT_TO_RIGHT);");

		// 调用 addDrawable 
		geode->addDrawable(text);
	}

	// 设置文字输出的格式为 RIGHT_TO_LEFT  从右到左
	{
		osg::ref_ptr<osgText::Text> text = new osgText::Text();
		text->setFont(font);
		text->setCharacterSize(20.0f);
		text->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
		text->setPosition(osg::Vec3(800.0f, 700.0f, 0.0f));	

		text->setLayout(osgText::Text::RIGHT_TO_LEFT);		
		text->setAlignment(osgText::Text::RIGHT_BASE_LINE);


		text->setText("text->setLayout(osgText::Text::RIGHT_TO_LEFT);");

		// 调用 addDrawable 
		geode->addDrawable(text);
	}


	// 设置文字输出的格式为 VERTICAL  
	{
		osg::ref_ptr<osgText::Text> text = new osgText::Text();
		text->setFont(font);
		text->setCharacterSize(20.0f);
		text->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
		text->setPosition(osg::Vec3(400.0f, 800.0f, 0.0f));	

		text->setLayout(osgText::Text::VERTICAL);		
	
		text->setText("text->setLayout(osgText::Text::VERTICAL);");

		// 调用 addDrawable 
		geode->addDrawable(text);
	}


	// 设置文字输出的格式为 阴影
	{
		osg::ref_ptr<osgText::Text> text = new osgText::Text();
		text->setFont(font);
		text->setCharacterSize(40.0f);
		text->setColor(osg::Vec4(1.0f, 1.0f, 0.0f,1.0f));
		text->setPosition(osg::Vec3(50.0f, 650.0f, 0.0f));	

		// 设置阴影为 Drop_SHADOW_BOTTOM_RIGHT
		text->setBackdropType(osgText::Text::DROP_SHADOW_BOTTOM_RIGHT);
		text->setBackdropColor(osg::Vec4(0.0f,1.0f,0.0f,1.0f));
		text->setBackdropOffset(0.1f,-0.1f);


		text->setText("text->setBackdropType(osgText::Text::DROP_SHADOW_BOTTOM_RIGHT);");

		// 调用 addDrawable 
		geode->addDrawable(text);
	}


	// 设置边框
	{
		osg::ref_ptr<osgText::Text> text = new osgText::Text();
		text->setFont(font);
		text->setCharacterSize(30.0f);
		text->setColor(osg::Vec4(1.0f, 0.0f, 0.0f,1.0f));
		text->setPosition(osg::Vec3(50.0f, 600.0f, 0.0f));	

		// 设置边框对齐绘制
		text->setDrawMode(osgText::Text::TEXT|osgText::Text::BOUNDINGBOX|osgText::Text::ALIGNMENT);
		
		text->setText("text->setDrawMode(osgText::Text::TEXT|osgText::Text::BOUNDINGBOX|osgText::Text::ALIGNMENT);");

		// 调用 addDrawable 
		geode->addDrawable(text);
	}

	// 设置颜色倾斜模式
	{
		osg::ref_ptr<osgText::Text> text = new osgText::Text();
		text->setFont(font);
		text->setCharacterSize(30.0f);
		text->setColor(osg::Vec4(1.0f, 0.0f, 1.0f,1.0f));
		text->setPosition(osg::Vec3(50.0f, 550.0f, 0.0f));	

		// 设置颜色倾斜模式为 PER_CHARACTER
		text->setColorGradientMode(osgText::Text::PER_CHARACTER);
		// 设置倾斜4个角落的颜色
		text->setColorGradientCorners(osg::Vec4(1.0f,0.0f,0.0f,1.0f),osg::Vec4(0.0f,1.0f,0.0f,1.0f),osg::Vec4(0.0f,0.0f,1.0f,1.0f),osg::Vec4(1.0f,1.0f,1.0f,1.0f));

		text->setText("text->setColorGradientMode(osgText::Text::PER_CHARACTER);");

		// 调用 addDrawable 
		geode->addDrawable(text);
	}



	camera->addChild(geode.get());
	
	return camera.get();
}



void TestAllKindText(){
	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	root->addChild(CreaAllKindText());

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
}