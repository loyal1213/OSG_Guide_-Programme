// stdafx.h

#include <osg/Geometry> 
#include <osg/Geode>    

#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Quat>
#include <osg/Matrix>

#include <osg/ShapeDrawable> //Ԥ���弸�����࣬������osg::Drawable�ࡣOSG��ʹ�ø�������OSG��Ƕ��Ԥ���弸������osg::Drawable��������Ⱦ��Щ������
#include <osg/Transform> //һ����ڵ㣬�����ӽڵ㶼ͨ��4x4������б任��ͨ�������ڳ����ж�λ�������ɹ켣���ܻ����ڶ���
#include <osg/NodeCallback > //�ڵ���»ص�
#include <osg/Depth> //��װOpenGL glDepthFunc / Mask / Range����
#include <osg/CullFace>

#include <osg/TexMat>
#include <osg/TexGen> //ָ�������Զ�������������ĺ�����������������ļ��㷽ʽ������������ռ仹���������ռ������в�ͬ�ļ���
#include <osg/TexEnv>
#include <osg/TextureCubeMap> //����������ӳ��

#include <osgViewer/Viewer>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

//.cpp
 
//��ȡ����ͼ
osg::ref_ptr<osg::TextureCubeMap> readCubeMap()
{
	//��ȡ����ͼƬ
	/*��ȡ����ͼ������ͼʱ��������ͼҪ��������ĸ�����һһ��Ӧ
		POSITIVE_X=0, //Left  X������
		NEGATIVE_X=1, //Right  X������
		POSITIVE_Y=2, //Front  Y������
		NEGATIVE_Y=3, //Back  Y������
		POSITIVE_Z=4, //Up  Z������
		NEGATIVE_Z=5 //Down  Z������
	*/
	osg::ref_ptr<osg::Image> imagePosX = osgDB::readImageFile("Cubemap_snow/posx.jpg");
	osg::ref_ptr<osg::Image> imageNegX = osgDB::readImageFile("Cubemap_snow/negx.jpg");
	osg::ref_ptr<osg::Image> imagePosY = osgDB::readImageFile("Cubemap_snow/posy.jpg");
	osg::ref_ptr<osg::Image> imageNegY = osgDB::readImageFile("Cubemap_snow/negy.jpg");
	osg::ref_ptr<osg::Image> imagePosZ = osgDB::readImageFile("Cubemap_snow/posz.jpg");
	osg::ref_ptr<osg::Image> imageNegZ = osgDB::readImageFile("Cubemap_snow/negz.jpg");
 
	//�����������������
	osg::ref_ptr<osg::TextureCubeMap> cubemap = new osg::TextureCubeMap();
	//�ж�����ͼƬ����
	if (imagePosX.get() && imageNegX.get() && imagePosY.get() && imageNegY.get() && imagePosZ.get() && imageNegZ.get())
	{
		//��������ͼ��6�������ͼ
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_X, imagePosX.get());
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_X, imageNegX.get());
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_Y, imagePosY.get());
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Y, imageNegY.get());
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_Z, imagePosZ.get());
		cubemap->setImage(osg::TextureCubeMap::NEGATIVE_Z, imageNegZ.get());
 
		//����������ģʽ
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
		cubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
		cubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
		cubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);
 
		//�����˲������Ժ�mipmap
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
		cubemap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
		cubemap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	}
 
	return cubemap.release();
}
 
//��������ͼ����
struct TexMatCallback:public osg::NodeCallback
{
public:
	TexMatCallback(osg::TexMat &tm) :
		_texMat(tm)
	{
		//
	}
	virtual void operator()(osg::Node *node, osg::NodeVisitor *nv)
	{
		osgUtil::CullVisitor *cv = dynamic_cast<osgUtil::CullVisitor *>(nv);
		if (cv)
		{
			//�õ�ģ����ͼ����������ת�Ƕ�
			const osg::Matrix &MV = *(cv->getModelViewMatrix());
			const osg::Matrix R = osg::Matrix::rotate(osg::DegreesToRadians(112.0f), 0.0f, 0.0f, 0.0f) * osg::Matrix::rotate(osg::DegreesToRadians(90.0f), 1.0f, 0.0f, 0.0f);
			osg::Quat q = MV.getRotate();
			const osg::Matrix C = osg::Matrix::rotate(q.inverse());
			
			//�����������
			_texMat.setMatrix(C*R);
		}
		traverse(node, nv);
	}
 
	//�������
	osg::TexMat &_texMat;
};
 
//һ���任�࣬ʹ��պ����ӵ���ת
class MoveEarthySkyWithEyePointTransform :public osg::Transform
{
public:
	//�ֲ����������������
	virtual bool computerLocalToWorldMatrix(osg::Matrix &matrix, osg::NodeVisitor *nv) const
	{
		osgUtil::CullVisitor *cv = dynamic_cast<osgUtil::CullVisitor *>(nv);
		if (cv)
		{
			osg::Vec3 eyePointLocal = cv->getEyeLocal();
			matrix.preMult(osg::Matrix::translate(eyePointLocal));
		}
		return true;
	}
 
	//����������Ϊ�ֲ�����
	virtual bool computerWorldToLocalMatrix(osg::Matrix &matrix, osg::NodeVisitor *nv) const
	{
		osgUtil::CullVisitor *cv = dynamic_cast<osgUtil::CullVisitor *>(nv);
		if (cv)
		{
			osg::Vec3 eyePointLocal = cv->getEyeLocal();
			matrix.postMult(osg::Matrix::translate(-eyePointLocal));
		}
		return true;
	}
};
 
/*
*������պ�
*������պУ���ã�����������ʵ������������ʵ�����Ч��
*��ȡ����ͼ������ͼ
*��������ͼ�������
*�����Զ�������������
*�����������
*��������ͼ����
*������Ⱦ˳��
*���뵽Ҷ�ڵ��л���
*�رչ��ղ��رձ����޳���
*��Ҷ�ڵ���ӵ�����任�ڵ��У���ʵ�ֺ��ʵľ���任
*/
osg::ref_ptr<osg::Node> createSkyBox()
{
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
 
	//��������ӳ�䷽ʽ��ָ��Ϊ�����ʽ���������е���ɫ����ԭ������ɫ
	osg::ref_ptr<osg::TexEnv> te = new osg::TexEnv();
	te->setMode(osg::TexEnv::REPLACE);
	stateset->setTextureAttributeAndModes(0, te.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
 
	//�Զ������������꣬���䷽ʽ��REFLECTION_MAP��
	/*
		NORMAL_MAP, //��׼ģʽ-����ͼ����
		REFLECTION_MAP //����ģʽ-��������
		SPHERE_MAP //����ģ��-��������
	};
	*/
	osg::ref_ptr<osg::TexGen> tg = new osg::TexGen();
	tg->setMode(osg::TexGen::NORMAL_MAP);
	stateset->setTextureAttributeAndModes(0, tg.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	
	//�����������
	osg::ref_ptr<osg::TexMat>tm = new osg::TexMat();
	stateset->setTextureAttribute(0, tm.get());
	
	//��������ͼ����
	osg::ref_ptr<osg::TextureCubeMap> skymap = readCubeMap();
	stateset->setTextureAttributeAndModes(0, skymap.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	stateset->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
	
	//���������ΪԶƽ��
	osg::ref_ptr<osg::Depth> depth = new osg::Depth();
	depth->setFunction(osg::Depth::ALWAYS);
	depth->setRange(1.0, 1.0); //Զƽ��
	stateset->setAttributeAndModes(depth, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
 
	//������Ⱦ˳��Ϊ-1������Ⱦ
	stateset->setRenderBinDetails(-1, "RenderBin");
 
	osg::ref_ptr<osg::Drawable> drawable = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f), 1));
	
	//��������뵽Ҷ�ڵ�
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->setCullingActive(false);
	geode->setStateSet(stateset.get());
	geode->addDrawable(drawable.get());
	
	//���ñ任
	osg::ref_ptr<osg::Transform> transform = new MoveEarthySkyWithEyePointTransform();
	//�رձ����޳�
	transform->setCullingActive(false);
	transform->addChild(geode.get());
	osg::ref_ptr<osg::ClearNode> clearNode = new osg::ClearNode();
	clearNode->setCullCallback(new TexMatCallback(*tm));
	clearNode->addChild(transform.get());
 
	return clearNode.release();
}
 
int TestCubeMap()
{
	osg::ref_ptr<osg::Group> root = new osg::Group();
	//������պ�
	root->addChild(createSkyBox());
 
	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
 
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	viewer->setSceneData(root.get());
	viewer->realize();
 
	return viewer->run();
}