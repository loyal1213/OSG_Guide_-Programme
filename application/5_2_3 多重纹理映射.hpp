#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>

#include <osg/Texture2D>
#include <osg/TexGen>
#include <osg/TexEnv>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

void TestTextureMultiple2(){

	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// ����ģ��
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");


	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/primitives.gif");

	if (image.get()){

		// ������ά����
		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
		texture->setImage(image.get());

		// �����Զ���������
		osg::ref_ptr<osg::TexGen> texgen = new osg::TexGen();
		texgen->setMode(osg::TexGen::SPHERE_MAP);

		// ������������ģʽΪBLEND
		osg::ref_ptr<osg::TexEnv> texenv = new osg::TexEnv();
		texenv->setMode(osg::TexEnv::BLEND);
		// ����BLEND ��������ɫ
		texenv->setColor(osg::Vec4(0.6f,0.6f,0.6f,0.0f));

		// ���õ�Ԫ 1 �Զ������������꣬��ʹ������
		osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
		stateset->setTextureAttributeAndModes(1,texture.get(),osg::StateAttribute::ON);
		stateset->setTextureAttributeAndModes(1,texgen.get(),osg::StateAttribute::ON);

		// ����������
		stateset->setTextureAttribute(1,texenv.get());

		// ��������״̬
		node->setStateSet(stateset.get());

	}


	root->addChild(node);

	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
}