// stdafx.h
// ����ӳ�䣨��������һά/��ά/������ͼ����ӳ��
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
void createTexture1D(osg::StateSet &ss)
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
osg::ref_ptr<osg::StateSet> createTexture1D()
{
	//������ͼ����ʵ������һ���߶�Ϊ1�Ķ�άͼ��
	osg::ref_ptr<osg::Image> image = new osg::Image;
	//Ϊimage����һ���ռ�
	image->allocateImage(1024, 1, 1, GL_RGBA, GL_FLOAT);
	//��������ͼ�����ݸ�ʽRGBA
	image->setInternalTextureFormat(GL_RGBA);
	//Ϊimage�������
	osg::Vec4 *dataPtr = (osg::Vec4 *)image->data();
	for (int i = 0; i < 1024; ++i)
	{
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
 
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;
	//����һά����
	stateset->setTextureAttribute(0, texture.get(), osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0, GL_TEXTURE_1D, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	//��������������
	stateset->setTextureAttribute(0, texgen.get(), osg::StateAttribute::OVERRIDE);
	stateset->setTextureMode(0, GL_TEXTURE_GEN_S, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
 
	return stateset.get();
}
 
/*
 *���ö�ά���������
 *ֱ��ʹ�����е�ͼ���ļ�����
 *������������ı߽���ɫ
 *
 *��������ֵΪ����
 */
osg::ref_ptr<osg::Texture2D> createTexture2D()
{
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
	texture->setImage(osgDB::readImageFile("Images/clockface.jpg"));
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_BORDER);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_BORDER);
	texture->setBorderColor(osg::Vec4(1.0, 1.0, 0.0, 1.0));
 
	return texture.get();
}
 
/*
 *�������������������
 *����ͼ������Ҫ6�����ͼ������
 *�����ڻ�����ͼ��ʵ��
 *�������Ⱦ����TexGen
 *�����Զ�Ϊ����ͼ��������ö���������������
 *
 *��������ֵΪ��Ⱦ״̬
 */
osg::ref_ptr<osg::StateSet> createTextureCubeMap()
{
	osg::ref_ptr<osg::TextureCubeMap> texture = new osg::TextureCubeMap();
	texture->setImage(osg::TextureCubeMap::POSITIVE_X, osgDB::readImageFile("Cubemap_snow/posx.jpg"));
	texture->setImage(osg::TextureCubeMap::NEGATIVE_X, osgDB::readImageFile("Cubemap_snow/negx.jpg"));
	texture->setImage(osg::TextureCubeMap::POSITIVE_Y, osgDB::readImageFile("Cubemap_snow/posy.jpg"));
	texture->setImage(osg::TextureCubeMap::NEGATIVE_Y, osgDB::readImageFile("Cubemap_snow/negy.jpg"));
	texture->setImage(osg::TextureCubeMap::POSITIVE_Z, osgDB::readImageFile("Cubemap_snow/posz.jpg"));
	texture->setImage(osg::TextureCubeMap::NEGATIVE_Z, osgDB::readImageFile("Cubemap_snow/negz.jpg"));
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
	texture->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
 
	osg::ref_ptr<osg::StateSet> sta = createTexture1D();
	sta->setTextureAttributeAndModes(0, texture.get());
	sta->setTextureAttributeAndModes(0, new osg::TexGen);
 
	return sta.get();
}
 
int TestTexture1DOr2DOrMap()
{
	//��1��һά����ͼ��
	osg::ref_ptr<osg::Geode> texl1D = new osg::Geode();
	//ʹ��createTexturedQuadGeometry()���ٹ���һ���ı��Σ����������������귶ΧΪ��0, 0������3,1������ʵ��ˮƽ������ظ�����
	//createTexturedQuadGeometry(const Vec3& corner,const Vec3& widthVec,const Vec3& heightVec, float l, float b, float r, float t);���ڴ�����������������ı��μ���ͼ��
	//����1�Ǵ����½���ʼ�ĵ㣬�������ʼ����������꣬�����㰴�ղ���2�Ͳ���3�������Σ��������귶ΧΪ��l, b������r, t��
	texl1D->addDrawable(osg::createTexturedQuadGeometry(osg::Vec3(-3.0, 0.0, -0.5), osg::Vec3(1.0, 0.0, 0.0), osg::Vec3(0.0, 0.0, 1.0), 0.0, 0.0, 3.0, 1.0));
	//��������Ϊ��ǰ�ڵ����Ⱦ״̬���޷���ֵ�������ڽ�����Ⱦ
	createTexture1D(*(texl1D->getOrCreateStateSet()));
 
	//��2��һά����ͼ��
	osg::ref_ptr<osg::Node> texl1D2 = osgDB::readNodeFile("cessna.osg");
	//��������ֵΪ��Ⱦ״̬����Ҫ���ýڵ����Ⱦ״̬
	osg::ref_ptr<osg::StateSet> sta = createTexture1D();
	texl1D2->setStateSet(sta.get());
	//ʹ�þ���ת���ڵ���Сģ�ͣ�һ��ʹ�þ���ת���ڵ��������ģ�ͣ����ŵ�ԭ���Ķ��ٱ���������ģ�͵�λ�ã���ĳ�������ƶ����٣���������ת
	//���ֳܷ�����д���������һ�����ǰ��ĸ��ǣ��������Ǿ���˷������˳��ͬ���Ҳ��ͬ
	//setMatrix(osg::Matrix::scale(osg::Vec3(double sx, double sy, double sz)) * osg::Matrix::translate(osg::Vec3(double tx, double ty, double tz)) * osg::Matrix::rotate(double angle, const Vec3d &axis));
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
	mt->setMatrix(osg::Matrix::scale(0.05, 0.05, 0.05));
	mt->addChild(texl1D2);
 
	//��1����ά����ͼ��
	osg::ref_ptr<osg::Geode> texl2D = new osg::Geode();
	//�����ı��ε��������귶ΧΪ��-0.1, -0.1������1.1, 1.1������˻������Ƴ�����ı߽�
	texl2D->addDrawable(osg::createTexturedQuadGeometry(osg::Vec3(2, 0.0, -0.5), osg::Vec3(1.0, 0.0, 0.0), osg::Vec3(0.0, 0.0, 1.0), -0.1, -0.1, 1.1, 1.1));
	//��������ֵΪ������Ϊ��Ⱦ״̬�����ԣ������Ҫ�Ȼ�ȡ�����ýڵ����Ⱦ״̬��Ȼ��������Ⱦ״̬�����Ժ�ģʽ
	osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D();
	tex = createTexture2D();
	osg::ref_ptr<osg::StateSet> sta2 = new osg::StateSet();
	sta2->setTextureAttributeAndModes(0, createTexture2D());
	texl2D->setStateSet(sta2.get());
	//����5����Ժϲ�������1��
	//texl2D->getOrCreateStateSet()->setTextureAttributeAndModes(0, createTexture2D());
 
	//��1������������ͼ��
	osg::ref_ptr<osg::Geode> texlCube = new osg::Geode();
	texlCube->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(5.0, 0.0, 0.0), 0.7)));
	//��������ֵΪ��Ⱦ״̬����Ҫ���ýڵ����Ⱦ״̬
	osg::ref_ptr<osg::StateSet> sta1 = new osg::StateSet();
	texlCube->setStateSet(createTextureCubeMap());
 
	osg::ref_ptr<osg::Group> root = new osg::Group();
	root->addChild(texl1D.get());
	root->addChild(mt.get());
	root->addChild(texl2D.get());
	root->addChild(texlCube.get());
 
	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
 
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
 
	viewer->setSceneData(root.get());
	viewer->realize();
 
	return viewer->run();
}