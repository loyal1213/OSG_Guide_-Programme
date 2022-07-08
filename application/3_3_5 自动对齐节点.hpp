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


	// ����
	std::string font("font/cour.ttf");

	// ����Text����
	osg::ref_ptr<osgText::Text> text = new osgText::Text();
	geode->addDrawable(text.get());

	// ��������
	text->setFont(font);
	// ��������ֱ���
	text->setFontResolution(128.0f,128.0f);
	// ���������С
	text->setCharacterSize(size);
	// ����������뷽ʽ
	text->setAlignment(osgText::Text::CENTER_CENTER);
	// ���÷���
	text->setAxisAlignment(axisAlignment);
	// ��������
	text->setText(label);

	// �رչ���
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

	// �����Զ��任�ڵ�
	osg::ref_ptr<osg::AutoTransform> at = new osg::AutoTransform();
	// ����ӽڵ�
	at->addChild(geode.get());


	// �����Զ��任��ʽ
	at->setAutoRotateMode(automode);
	// ������Ļ��С�����Žڵ㣬Ĭ��Ϊ false, ����Ϊ true ʱ, �ڵ��޷�����
	at->setAutoScaleToScreen(false);
	// �������ŵ����ֵ����С����
	at->setMinimumScale(0.0f);
	at->setMaximumScale(5.0f);

	// ����λ��
	at->setPosition(position);
	return at.get();
}




int TestAutoTransform(){

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	std::string text("Fly To Sky");

	/*
	3 �ֱ任ģʽ��
	rotate_to_screen	  �Զ�������Ļ		
	rotate_to_camera      �Զ��������
	no_rotation           ��			
	*/


	// ��� rotate_to_screen ģʽ�任�ڵ�  
	root->addChild(CreateAutoTransform(osg::Vec3(0.0f,0.0f,10.0f), 10.0f, text, osg::AutoTransform::ROTATE_TO_SCREEN, osgText::Text::XY_PLANE));

	// ��� no_rotation ģʽ�任�ڵ�
	root->addChild(CreateAutoTransform(osg::Vec3(0.0f,0.0f,20.0f), 20.0f, text, osg::AutoTransform::NO_ROTATION, osgText::Text::YZ_PLANE));

	// ��� rotate_to_camera ģʽ�任�ڵ�
	root->addChild(CreateAutoTransform(osg::Vec3(0.0f,0.0f,30.0f), 10.0f, text, osg::AutoTransform::ROTATE_TO_CAMERA, osgText::Text::XY_PLANE));


	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
	return 0;
}