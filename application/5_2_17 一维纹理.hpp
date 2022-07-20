// stdafx.h

#include <osg/Geode>    

#include <osg/Texture1D>  //һά����ӳ��
#include <osg/Texture2D> //��ά����ӳ��
#include <osg/TextureCubeMap> //����������ӳ��
#include <osg/TexGen> //ָ�������Զ�������������ĺ�����������������ļ��㷽ʽ������������ռ仹���������ռ������в�ͬ�ļ���
#include <osg/ShapeDrawable> //Ԥ���弸�����࣬������osg::Drawable�ࡣOSG��ʹ�ø�������OSG��Ƕ��Ԥ���弸������osg::Drawable��������Ⱦ��Щ������
#include <osg/MatrixTransform>

#include <osgViewer/Viewer>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
//.cpp

/*
*����һά��������� ���������һ�ַ���
*һά����ֻ��һ�����������ر仯
*��˴˴��ֶ�Ϊ��ָ�����ݣ�
*�½�һ��Image���������д�����ɫ����
*Ȼ������һά����ı߽��ȡ��ʽΪ�ظ�ӳ��
*��ɫ��������������������
*
*��������Ϊ�ڵ����Ⱦ״̬���޷���ֵ
*/
void CreateTexture1D(osg::StateSet &ss)
{
	//ʵ����һ��osg::Image�࣬���ڷ�װ�洢����ͼ�����ݵ�ͼ����
	osg::ref_ptr<osg::Image> image = new osg::Image();
	/*virtual void osg::Image::setImage(int s, int t, int r, 
	GLint internalTextureformat,
	GLenum pixelFormat,
	GLenum type,
	unsigned char *data,
	AllocationMode mode,
	int packing = 1)
	*/
	image->setImage(256, 1, 1,
		GL_RGBA, GL_RGBA,
		GL_UNSIGNED_BYTE,
		new unsigned char[4 * 256],
		osg::Image::USE_NEW_DELETE);
	unsigned char *ptr = image->data();
	for (unsigned int i = 0; i < 256; ++i)
	{
		*ptr++ = i;
		*ptr++ = i;
		*ptr++ = 255;
		*ptr++ = 255;
	}

	osg::ref_ptr<osg::Texture1D> texture = new osg::Texture1D();
	texture->setImage(image.get());
	/*�������������/��װģʽ
	enum WrapParmeter
	{
	WRAP_S, //x��
	WRAP_T, //y��
	WRAP_T //z��
	};
	enum WrapMode
	{
	CLAMP, //��ȡ
	CLAMP_TO_EDGE, //�߿�ʼ�ձ�����
	CLAMP_TO_BORDER, //��ʹ�õ�����ȡ��ͼ��ı߿�û�б߿��ʹ�ó����߿����ɫ
	REPEAT, //������ظ�ӳ��
	MIRROR //��������ظ�ӳ��
	};
	*/
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	/*��������Ĺ��˷���/���˴���
	enum FilterParameter
	{
	MIN_FILTER, //��С
	MAG_FILTER //�Ŵ�
	};
	enum FilterMode
	{
	LINEAR, //����Χ4�����ص�ƽ��ֵ��Ϊ����
	LINEAR_MIPMAP_LINEAR, //ʹ�����Ծ��ͼ������������ֵ
	LINEAR_MIPMAP_NEAREST, //���Եظ�д�ٽ�������Ԫֵ
	NEAREST, //ȡ�ȽϽӽ���������Ϊ����
	NEAREST_MIPMAP_LINEAR, //������������ѡ�����ٽ���������ȡ����֮������Ծ���ֵ
	NEAREST_MIPMAP_NEAREST //ѡ�����ٽ�������Ԫֵ
	};
	*/
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	ss.setTextureAttributeAndModes(0, texture.get());
}
/*
*����һά��������� �����������һ�ַ���
*
*��������ֵΪ��Ⱦ״̬
*/
osg::ref_ptr<osg::StateSet> CreateTexture1D()
{
	//������ͼ����ʵ������һ���߶�Ϊ1�Ķ�άͼ��
	osg::ref_ptr<osg::Image> image = new osg::Image;
	//Ϊimage����һ���ռ�
	image->allocateImage(1024, 1, 1, GL_RGBA, GL_FLOAT);
	//��������ͼ�����ݸ�ʽRGBA
	image->setInternalTextureFormat(GL_RGBA);

	//Ϊimage�������
	osg::Vec4 *dataPtr = (osg::Vec4 *)image->data();
	for (int i = 0; i < 1024; ++i){
		*dataPtr++ = osg::Vec4(1.0f, 0.5f, 0.8f, 0.5f);
	}

	//����һά����
	osg::ref_ptr<osg::Texture1D> texture = new osg::Texture1D();
	//���û���ģʽ
	texture->setWrap(osg::Texture1D::WRAP_S, osg::Texture1D::MIRROR);
	//�����˲�
	texture->setFilter(osg::Texture1D::MIN_FILTER, osg::Texture1D::LINEAR);
	//������ͼ
	texture->setImage(image.get());


	//�����Զ��������꣬���ƶ���ص�ƽ��
	osg::ref_ptr<osg::TexGen> texgen = new osg::TexGen;
	/*��������������Զ�����ģʽ
	enum Mode
	{
	OBJECT_LINEAR, //�������ԣ�������ͼ���ƶ����屣�̶ֹ�
	EYE_LINEAR, //�����ƶ�����Ķ�̬������
	SPHERE_MAP, //������ͼ
	NORMAL_MAP, //������ͼ����������ͼ����
	REFLECTION_MAP //������ͼ
	};
	*/
	texgen->setMode(osg::TexGen::OBJECT_LINEAR);
	/*��������
	enum Coord
	{
	S, //x
	T, //y
	R, //z
	Q //w
	};
	*/
	texgen->setPlane(osg::TexGen::S, osg::Plane(0.0f, 0.0f, 1.0f, -10000));

	// �������Լ�
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;

	//����һά����
	stateset->setTextureAttribute(0, texture.get(), osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0, GL_TEXTURE_1D, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	//��������������
	stateset->setTextureAttribute(0, texgen.get(), osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0, GL_TEXTURE_GEN_S, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	return stateset.get();
}

void TestTexture1D(){

	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	// ����ģ��
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cessna.osg");


	// ���õ�Ԫ 1 �Զ������������꣬��ʹ������
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset = CreateTexture1D();


	// ��������״̬
	node->setStateSet(stateset.get());
	root->addChild(node);

	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
}