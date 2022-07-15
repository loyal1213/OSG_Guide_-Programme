#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgText/Text>
#include <osgText/Font>

#include <osgUtil/Optimizer>

// ��������
osg::ref_ptr<osg::Camera> CreaAllKindText(){

	osg::ref_ptr<osg::Camera> camera = new osg::Camera();

	// ����ͶӰ����
	camera->setProjectionMatrix(osg::Matrix::ortho2D(0,1280,0,800));

	// ������ͼ����
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrix(osg::Matrix::identity());

	// �����Ȼ���
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);

	// ������Ⱦ˳�����������֮ǰ��Ⱦ
	camera->setRenderOrder(osg::Camera::POST_RENDER);

	// ����Ϊ�������¼���ʼ�ղ��õ�����
	camera->setAllowEventFocus(false);

	// 1. ����һ�� osgText::Font ������󣬲���ȡ����
	osg::ref_ptr<osgText::Font> font = osgText::readFontFile("fonts/arial.ttf");

	// ��ӵ�Ҷ�ڵ���
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	osg::ref_ptr<osg::StateSet> stateset = geode->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

	//---------------------------------------------------------------------------------
	// �������������ʽ

	// ������������ĸ�ʽΪ LEFT_TO_RIGHT  ������
	{
		// 2. ����һ��osgText::Text ����������������
		osg::ref_ptr<osgText::Text> text = new osgText::Text();
		text->setFont(font);
		text->setCharacterSize(20.0f);
		text->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
		text->setPosition(osg::Vec3(50.0f, 750.0f, 0.0f));	
		
		text->setLayout(osgText::Text::LEFT_TO_RIGHT);
		
		text->setText("text->setLayout(osgText::Text::LEFT_TO_RIGHT);");

		// ���� addDrawable 
		geode->addDrawable(text);
	}

	// ������������ĸ�ʽΪ RIGHT_TO_LEFT  ���ҵ���
	{
		osg::ref_ptr<osgText::Text> text = new osgText::Text();
		text->setFont(font);
		text->setCharacterSize(20.0f);
		text->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
		text->setPosition(osg::Vec3(800.0f, 700.0f, 0.0f));	

		text->setLayout(osgText::Text::RIGHT_TO_LEFT);		
		text->setAlignment(osgText::Text::RIGHT_BASE_LINE);


		text->setText("text->setLayout(osgText::Text::RIGHT_TO_LEFT);");

		// ���� addDrawable 
		geode->addDrawable(text);
	}


	// ������������ĸ�ʽΪ VERTICAL  
	{
		osg::ref_ptr<osgText::Text> text = new osgText::Text();
		text->setFont(font);
		text->setCharacterSize(20.0f);
		text->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
		text->setPosition(osg::Vec3(400.0f, 800.0f, 0.0f));	

		text->setLayout(osgText::Text::VERTICAL);		
	
		text->setText("text->setLayout(osgText::Text::VERTICAL);");

		// ���� addDrawable 
		geode->addDrawable(text);
	}


	// ������������ĸ�ʽΪ ��Ӱ
	{
		osg::ref_ptr<osgText::Text> text = new osgText::Text();
		text->setFont(font);
		text->setCharacterSize(40.0f);
		text->setColor(osg::Vec4(1.0f, 1.0f, 0.0f,1.0f));
		text->setPosition(osg::Vec3(50.0f, 650.0f, 0.0f));	

		// ������ӰΪ Drop_SHADOW_BOTTOM_RIGHT
		text->setBackdropType(osgText::Text::DROP_SHADOW_BOTTOM_RIGHT);
		text->setBackdropColor(osg::Vec4(0.0f,1.0f,0.0f,1.0f));
		text->setBackdropOffset(0.1f,-0.1f);


		text->setText("text->setBackdropType(osgText::Text::DROP_SHADOW_BOTTOM_RIGHT);");

		// ���� addDrawable 
		geode->addDrawable(text);
	}


	// ���ñ߿�
	{
		osg::ref_ptr<osgText::Text> text = new osgText::Text();
		text->setFont(font);
		text->setCharacterSize(30.0f);
		text->setColor(osg::Vec4(1.0f, 0.0f, 0.0f,1.0f));
		text->setPosition(osg::Vec3(50.0f, 600.0f, 0.0f));	

		// ���ñ߿�������
		text->setDrawMode(osgText::Text::TEXT|osgText::Text::BOUNDINGBOX|osgText::Text::ALIGNMENT);
		
		text->setText("text->setDrawMode(osgText::Text::TEXT|osgText::Text::BOUNDINGBOX|osgText::Text::ALIGNMENT);");

		// ���� addDrawable 
		geode->addDrawable(text);
	}

	// ������ɫ��бģʽ
	{
		osg::ref_ptr<osgText::Text> text = new osgText::Text();
		text->setFont(font);
		text->setCharacterSize(30.0f);
		text->setColor(osg::Vec4(1.0f, 0.0f, 1.0f,1.0f));
		text->setPosition(osg::Vec3(50.0f, 550.0f, 0.0f));	

		// ������ɫ��бģʽΪ PER_CHARACTER
		text->setColorGradientMode(osgText::Text::PER_CHARACTER);
		// ������б4���������ɫ
		text->setColorGradientCorners(osg::Vec4(1.0f,0.0f,0.0f,1.0f),osg::Vec4(0.0f,1.0f,0.0f,1.0f),osg::Vec4(0.0f,0.0f,1.0f,1.0f),osg::Vec4(1.0f,1.0f,1.0f,1.0f));

		text->setText("text->setColorGradientMode(osgText::Text::PER_CHARACTER);");

		// ���� addDrawable 
		geode->addDrawable(text);
	}



	camera->addChild(geode.get());
	
	return camera.get();
}



void TestAllKindText(){
	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	root->addChild(CreaAllKindText());

	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
}