// stdafx.h 
// ����ӳ�䣨һ��������������ӳ��
#include <osg/Node>  
#include <osg/Geometry>  
#include <osg/Geode> //�Ǹ����νڵ㣬����˵��һ������Group�ڵ㣬һ��Ŀɻ��Ƽ����嶼��ͨ����������root������Ⱦ����OSG���λ��Ƶ���߹���ڵ�  
#include <osg/Group> //�Խڵ�����֯���ã�һ����Ϊ���ڵ���߸��ڵ����

#include <osg/Texture2D> //��ά����ӳ����
#include <osg/TexGen> //ָ�������Զ�������������ĺ�����������������ļ��㷽ʽ������������ռ仹���������ռ������в�ͬ�ļ���
#include <osg/TexEnv> //���õ�ǰ����ӳ�䷽ʽ 

#include <osgDB/ReadFile>  
#include <osgDB/WriteFile>  

#include <osgViewer/Viewer>  

#include <osgUtil/Optimizer> 

//.cpp
 
/*
��������ӳ��
��׼�Ķ�ά����ӳ�䴦����һ�ΰ�һ������ͼ��Ӧ�õ�һ���������
��������ӳ������Ӧ�ü�������
�ڲ�������ջ�����ΰ�����Ӧ�õ�ͬһ���������
�����������һЩ�е�����Ԫ
ÿ������Ԫִ�е������������
�������Ľ�����ݸ���һ������Ԫ
ֱ����������Ԫ�Ĳ������Ϊֹ
������ʾ������Ч��
����������ʵ�ָ߼���Ⱦ���ɣ����ա��������ϳɡ�ϸ�������
���裺
ָ���û����������������
��������������Զ��󲢱���������ͼ������
ΪStateSet���ú��ʵ��������Ժ�ģʽ
���ڲ�ͬ���������Զ�����Ҫָ����ͬ������Ԫ����������
����Ͳ������ø�����Ԫ
���߸�����Ԫ�ᱻ����
*/
static void textureMultiple(osg::Node *node, osg::Image *image)
{
	if (image)
	{
		//������ά����
		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
		texture->setImage(image);
 
		/*
		����������Զ�����ģʽ��
		enum Mode
		{
			OBJECT_LINEAR, //�������ԣ�������ͼ���ƶ����屣�̶ֹ�
			EYE_LINEAR, //�����ƶ�����Ķ�̬������
			SPHERE_MAP, //������ͼ
			NORMAL_MAP, //������ͼ����������ͼ����
			REFLECTION_MAP, //������ͼ
		}
		*/
		osg::ref_ptr<osg::TexGen> texgen = new osg::TexGen;
		texgen->setMode(osg::TexGen::SPHERE_MAP);
 
		/*
		����ӳ��ģʽ����������ͼ�����������屾����ںϣ���
		enum Mode
		{
			DECAL, //����
			MODULATE, //����
			BLEND, //���
			REPLACE, //�滻������
			ADD
		}
		*/
		osg::ref_ptr<osg::TexEnv> texenv = new osg::TexEnv;
		texenv->setMode(osg::TexEnv::BLEND);
		//���û��ģʽBLEND��������ɫ
		texenv->setColor(osg::Vec4(0.6f, 0.6f, 0.6f, 0.0f));
 
		//���õ�Ԫ1�Զ������������꣬��ʹ������
		//����Ԫ�����Ϊ���������
		//һ������Ԫ��һ������ͼ���໥��Ӧ�󶨣������Ÿ�����ͼ��Ĵ�����
		//һ������Ԫ��һ������ͨ����ά������ĸ���״̬�����˲������Ʒ�ʽ����������������
		//��Mipmap�У������������һ������������Ԫ��Mipmap���������ݽ�����
		//�������ӻ���������ӽ�Խ����ͼ������Խ�ߣ�������ͼ��Ҳ��Խ�������������ӽǽ�Զʱ��������������
		osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;
		stateset->setTextureAttributeAndModes(1, texture.get(), osg::StateAttribute::ON);
		stateset->setTextureAttributeAndModes(1, texgen.get(), osg::StateAttribute::ON);
 
		//����������
		stateset->setTextureAttribute(1, texenv.get());
 
		//��������״̬
		node->setStateSet(stateset.get());
	}
}
 
int TestTextureMultiple()
{
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");
	//��ȡ��ͼ
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/primitives.gif");
 
	//��������ӳ��
	textureMultiple(node, image);
 
	osg::ref_ptr<osg::Group> root = new osg::Group();
	root->addChild(node.get());
 
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
 
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
 
	viewer->setSceneData(root.get());
	viewer->realize();
 
	return viewer->run();
}