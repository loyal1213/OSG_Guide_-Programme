// hud ��heads up display�� ��̧ͷ��ʾ�����ּ�������Ӧ���ھ���ת������
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
osg::ref_ptr<osg::Camera> CreateHUDText(){

	osg::ref_ptr<osg::Camera> camera = new osg::Camera();

	// ����ͶӰ����
	camera->setProjectionMatrix(osg::Matrix::ortho2D(0,1280,0,800));

	// ������ͼ����ͬʱȷ����������������ͼ��λ�ñ任Ӱ�죬ʹ�þ���֡����
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrix(osg::Matrix::identity());

	// �����Ȼ���
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);

	// ������Ⱦ˳�����������֮ǰ��Ⱦ
	camera->setRenderOrder(osg::Camera::POST_RENDER);

	// ����Ϊ�������¼���ʼ�ղ��õ�����
	camera->setAllowEventFocus(false);


	// 1. ����һ�� osgText::Font ������󣬲���ȡ����
	osg::ref_ptr<osgText::Font> font = osgText::readFontFile("fonts/simhei.ttf");

	// ��ӵ�Ҷ�ڵ���
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	osg::ref_ptr<osg::StateSet> stateset = geode->getOrCreateStateSet();
	// �رչ���
	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	// �ر���Ȳ���
	stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);

	// ��������
	osg::ref_ptr<osgText::Text> text = new osgText::Text();

	// ���������ļ�
	text->setFont(font);
	// ����������Ϣ
	text->setText(L"http://OsgChina.org -- OpengSceneGraph �й��ٷ�");
	// ���������С
	text->setCharacterSize(20.0f);
	text->setAutoRotateToScreen(true);
	// ����������ɫ
	text->setColor(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
	osg::Vec3f position = osg::Vec3(50.0f, 600.0f, 0.0f);
	text->setPosition(position);
	// ���ö��뷽ʽ
	text->setAlignment(osgText::Text::CENTER_TOP);
	// ������ת��ʽ
	text->setAxisAlignment(osgText::Text::SCREEN);

	geode->addDrawable(text.get());

	camera->addChild(geode.get());

	return camera.get();
}



void TestHUDText(){
	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// root->addChild(osgDB::readImageFile("logo.gif")->asNode());
	root->addChild(CreateHUDText());

	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
}