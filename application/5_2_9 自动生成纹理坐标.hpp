#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Image>
#include <osg/TexGen>
#include <osg/StateSet>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

// ������ά��������
osg::ref_ptr<osg::StateSet> CreateTexture1DState(){

	//��ȡ��ͼ
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/primitives.gif");

	// ������ά����
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
	// ���ñ߽紦��
	texture->setWrap(osg::Texture2D::WRAP_S,osg::Texture2D::REPEAT);
	texture->setWrap(osg::Texture2D::WRAP_T,osg::Texture2D::REPEAT);

	// �����˲�
	texture->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER,osg::Texture::NEAREST);

	// ������ͼ
	texture->setImage(image);

	// �����Զ��������꣬��ָ����ص�ƽ��
	osg::ref_ptr<osg::TexGen> texgen = new osg::TexGen();
	texgen->setMode(osg::TexGen::OBJECT_LINEAR);
	texgen->setPlane(osg::TexGen::S,osg::Plane(0.0f,0.0f,1.0f,0.0f));

	// �������Լ�
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();

	// ���ö�ά����
	stateset->setTextureAttribute(0, texture.get(), osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0, GL_TEXTURE_2D, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0,GL_TEXTURE_GEN_T,osg::StateAttribute::OVERRIDE);

	return stateset.get();
}



void TestTexGen(){
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cessna.osg");

	// �Զ�����������������
	osg::ref_ptr<osg::StateSet> stateset = CreateTexture1DState();

	node->setStateSet(stateset);
	root->addChild(node.get());


	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	
	viewer->setSceneData(root.get());
	viewer->realize();

	viewer->run();

}