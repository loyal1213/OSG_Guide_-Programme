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
osg::ref_ptr<osg::Geode> CreateText(){

	

	// 1. ����һ�� osgText::Font ������󣬲���ȡ����
	osg::ref_ptr<osgText::Font> font = new osgText::Font();
	// ��ȡ������Ϣ
	font = osgText::readFontFile("simhei.ttf");

	// 2. ����һ��osgText::Text ����������������
	osg::ref_ptr<osgText::Text> text = new osgText::Text();
	// ���������ļ�
	text->setFont(font);
	// ����������Ϣ
	text->setText(L"http://OsgChina.org -- OpengSceneGraph �й��ٷ�");
	// ���������С
	text->setCharacterSize(0.5f);
	text->setAutoRotateToScreen(true);
	// ����������ɫ
	text->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	osg::Vec3f position = osg::Vec3(0.0f, -10.0f, 0.0f);
	text->setPosition(position);
	// ���ö��뷽ʽ
	text->setAlignment(osgText::Text::CENTER_TOP);
	// ������ת��ʽ
	text->setAxisAlignment(osgText::Text::SCREEN);
	
	// ��ӵ�Ҷ�ڵ���
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	// ���� addDrawable 
	geode->addDrawable(text);

	return geode.get();
}



void TestOsgText(){
	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	root->addChild(CreateText());

	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
}