/*
��ά����ӳ����һ����Ӧ�÷����һ���֣���Ϊ����Ⱦ������Ȼ�ܴﵽ�ܺõ�Ч�������������ϴ�ʱ����Ⱦ��������ռ�õ��ڴ���Դ���࣬���Ӧ����ʵ�ʵ���Ŀ���١�

��ά������Կ�����һ���Ķ�ά����ͼ����������ɵ�һ���ж����ȵ�����ͼ��

��ά����osg::Texture3D��̳���osg::Texture�࣬������֧������ͼ����ʹ����ά����ķ���ͬʹ�ö�ά����������ƣ���ά����ͼ��Ķ�ά������ͼ�Ĵ�С�����ظ�ʽ��Ҫһ�£�ʹ����ά����ʱ��Ҫ��ʼ��һ��ͼ�λ�����
*/
// stdafx.h

#include <osg/Node>
#include <osg/Geode>  
#include <osg/Group>

#include <osg/Image> 
#include <osg/StateSet>

#include <osg/TexGen> //ָ�������Զ�������������ĺ�����������������ļ��㷽ʽ������������ռ仹���������ռ������в�ͬ�ļ���
#include <osg/TexEnv>
#include <osg/Texture3D>  //�̳���osg::Texture�࣬������֧������ͼ����

#include <osgViewer/Viewer>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

//.cpp
 
/*
 *��ά����ӳ��
 *ʹ����ά����ķ���ͬʹ�ö�ά�����������
 *��ά����ͼ��Ķ�ά������ͼ�Ĵ�С�����ظ�ʽ��Ҫһ��
 *ʹ����ά����ʱ��Ҫ��ʼ��һ��ͼ�λ���
 *
 *��������ǽ����Ŷ�ά��������ѹ��һ����Ⱦ��
*/
 
 
//����һ���ı��νڵ�
osg::ref_ptr<osg::Node> createNode(){

	//��ӵ�Ҷ�ڵ�
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();


	//���ö���
	osg::ref_ptr<osg::Vec3Array> vc = new osg::Vec3Array;
	//�������  
	vc->push_back(osg::Vec3(0.0, 0.0, 0.0));
	vc->push_back(osg::Vec3(1.0, 0.0, 0.0));
	vc->push_back(osg::Vec3(1.0, 0.0, 1.0));
	vc->push_back(osg::Vec3(0.0, 0.0, 1.0));
 
	geom->setVertexArray(vc.get());

	//������������
	osg::ref_ptr<osg::Vec2Array> vt = new osg::Vec2Array();
	//�������
	vt->push_back(osg::Vec2(0.0f, 0.0f));
	vt->push_back(osg::Vec2(1.0f, 0.0f));
	vt->push_back(osg::Vec2(1.0f, 1.0f));
	vt->push_back(osg::Vec2(0.0f, 1.0f));
 
	//����������������setTexCoordArray(unsigned int unit, Array *)��������Ԫ/������������
	geom->setTexCoordArray(0, vt.get());

	//���÷���
	osg::ref_ptr<osg::Vec3Array> nc = new osg::Vec3Array();
	//��ӷ���
	nc->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));	
 

	//���÷�������setNormalArray(Array *array)
	geom->setNormalArray(nc.get());
	//���÷��ߵİ󶨷�ʽsetNormalBinding(AttributeBinding ab)Ϊȫ������
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
 
	//���ͼԪ�����ƻ�ԪΪ�ı���
	//���ݽ�������ָ���������ݺͰ󶨷�ʽ��ָ����Ⱦ������ķ�ʽ����ͬ��ʽ��Ⱦ����ͼ�β�ͬ����ʱЧ����ͬ�������������ڲ����Ƶ�Ҳ�����𣬺���Ϊ��
	//bool addPrimitiveSet(PrimitiveSet *primitiveset)����˵����osg::PrimitiveSet���޷���ʼ��������࣬�����Ҫ������������ָ��������Ⱦ�����Ϊosg::DrawArrays
	//osg::DrawArrays(GLenum mode, GLint first, GLsizei count)����Ϊָ���Ļ�ͼ��Ԫ�����Ƽ�����ĵ�һ����������ָ�������λ������ʹ�õĶ��������
	//PrimitiveSet��̳���osg::Object����࣬�����߱�һ��һ�㳡���е����ԣ�PrimitiveSet����Ҫ��װ��OpenGL�Ļ�ͼ��Ԫ��������ͼ��Ԫ����
	//POINTS��/LINES��/LINE_STRIP���߶�/LINE_LOOP�����
	//TRIANGLESһϵ��������(��������)/TRIANGLE_STRIPһϵ��������(���ú�����������)/TRIANGLE_FANһϵ�������Σ�����˳������һ�������Ƶ������β�ͬ
	//QUADS�ı���/QUAD_STRIPһϵ���ı���/POLYGON�����
 
	// ���ͼԪ
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
 
	// ����
	geode->addDrawable(geom.get());
 
	return geode.release();
}
 
//��ʼ��һ��ͼ�λ���
class MyGraphicsContext{
public:
	MyGraphicsContext(){

		//����ͼ�λ�������
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
		//�������Ͻ�����
		traits->x = 0;
		traits->y = 0;
		//���ÿ�Ⱥ͸߶�
		traits->width = 1;
		traits->height = 1;

		//���ô�����չ
		traits->windowDecoration = false;
		//����˫����
		traits->doubleBuffer = false;
		traits->sharedContext = 0;
		//����pbuffer
		traits->pbuffer = true;
		//����ͼ�λ���
		_gc = osg::GraphicsContext::createGraphicsContext(traits.get());
 
		//�������ʧ��
		if (!_gc){
			// ����pbufferΪfalse
			traits->pbuffer = false;
			// ���´���ͼ�λ���
			_gc = osg::GraphicsContext::createGraphicsContext(traits.get());
		}
 
		//�Ƿ��ʼ��
		if (_gc.valid()){
			//��ʼ��
			_gc->realize();
			_gc->makeCurrent();
		}
	}

	bool valid() const{
		return _gc.valid() && _gc->isRealized();
	}
 
private:
	osg::ref_ptr<osg::GraphicsContext> _gc;
};
 
//������ά��������
osg::ref_ptr<osg::StateSet> createState()
{
	//����ͼ�λ���
	MyGraphicsContext gc;
	if (!gc.valid()){
		//�������ʧ�ܣ��򷵻�
		osg::notify(osg::NOTICE) << "Unable to create the graphics context required to build 3d image." << std::endl;
		return 0;
	}
 
	//��ȡ4�Ŷ�ά����ͼ��
	osg::ref_ptr<osg::Image> image_0 = osgDB::readImageFile("Images/lz.rgb");
	osg::ref_ptr<osg::Image> image_1 = osgDB::readImageFile("Images/reflect.rgb");
	osg::ref_ptr<osg::Image> image_2 = osgDB::readImageFile("Images/tank.rgb");
	osg::ref_ptr<osg::Image> image_3 = osgDB::readImageFile("Images/skymap.jpg");
 
	//�ж��Ƿ���ȷ��ȡ
	if (!image_0 || !image_1 || !image_2 || !image_3){
		std::cout << "Warning:could not open files." << std::endl;
		return new osg::StateSet();
	}
 
	//�ж������ʽ�Ƿ�һ��
	if (image_0->getPixelFormat() != image_1->getPixelFormat() || 
		image_0->getPixelFormat() != image_2->getPixelFormat() || 
		image_0->getPixelFormat() != image_3->getPixelFormat() ){
		std::cout << "Warning:image pixel formats not compatible." << std::endl;
		return new osg::StateSet();
	}
 
	//�õ�֧�ֵġ�������ά����Ԫ�Ĵ�С
	/*GLint textureSize = osg::Texture3D::getExtensions(0, true)->maxTexture3DSize();
	if (textureSize > 256){
		textureSize = 256;
	}*/
 
	GLint textureSize = 256;
 
	//��4�Ŷ�ά����ͼ�����ţ��Դﵽ��ͬ�Ĵ�С
	image_0->scaleImage(textureSize, textureSize, 1);
	image_1->scaleImage(textureSize, textureSize, 1);
	image_2->scaleImage(textureSize, textureSize, 1);
	image_3->scaleImage(textureSize, textureSize, 1);
 
	//����һ����ά��������ͼ��ע�����Ϊ4
	osg::ref_ptr<osg::Image> image_3d = new osg::Image();
 
	//������ά��������ͼ�����ԣ���һ���͵ڶ�������������Ĵ�С����������������ά��������ͼ������
	image_3d->allocateImage(textureSize, textureSize, 4, image_0->getPixelFormat(), image_0->getDataType());
 
	// ��4�Ŷ�ά����ͼ��ѹ����ά��������ͼ��
	// ǰ���������ֱ���s/t/r�ϵ�ƫ�ƣ�����ֻ��r�ϵ�ƫ��
	// ���ĸ��������Ӷ�ά����ͼ������
	image_3d->copySubImage(0, 0, 0, image_0.get());
	image_3d->copySubImage(0, 0, 1, image_0.get());
	image_3d->copySubImage(0, 0, 2, image_0.get());
	image_3d->copySubImage(0, 0, 3, image_0.get());
 
	//���������ʽ
	image_3d->setInternalTextureFormat(image_0->getInternalTextureFormat());
 
	//������ά�������
	osg::ref_ptr<osg::Texture3D> texture3D = new osg::Texture3D();
	//�����˲�����֧��Mipmap�˲�
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
	texture3D->setFilter(osg::Texture3D::MIN_FILTER, osg::Texture3D::LINEAR);
	texture3D->setFilter(osg::Texture3D::MAG_FILTER, osg::Texture3D::LINEAR);
	//���û���ģʽ
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
	texture3D->setWrap(osg::Texture3D::WRAP_R, osg::Texture3D::REPEAT);
	//������ά����ͼ������
	texture3D->setImage(image_3d.get());
 
	//�����Զ�������������
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
	osg::ref_ptr<osg::TexGen> texgen = new osg::TexGen();
	//�����Զ�������������Ϊ�Ӿ�����
	texgen->setMode(osg::TexGen::OBJECT_LINEAR);
	//ָ���ο�ƽ��
	texgen->setPlane(osg::TexGen::R, osg::Plane(0.0f, 0.0f, 0.0f, 0.2f));
 
	//����״̬���Զ���
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	//������R���Զ�������������
	stateset->setTextureMode(0, GL_TEXTURE_GEN_R, osg::StateAttribute::ON);
	//�����Զ������������
	stateset->setTextureAttribute(0, texgen.get());
	stateset->setTextureAttributeAndModes(0, texture3D.get(), osg::StateAttribute::ON);
 
	return stateset.release();
}
 
int Test3DTextureMap()
{
	osg::ref_ptr<osg::Node> node = createNode();
	osg::ref_ptr<osg::StateSet> stateset = createState();
	node->setStateSet(stateset.get());
 
	osg::ref_ptr<osg::Group> root = new osg::Group();
	root->addChild(node.get());
 
	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
 
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	viewer->setSceneData(root.get());
	viewer->realize();
 
	return viewer->run();
}