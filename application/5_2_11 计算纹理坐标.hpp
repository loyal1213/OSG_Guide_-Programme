/*
�ںܶ��ʱ��ֱ��ָ�����������Ƿǳ�������ģ��������������꣩��ֻ�����������棨��Բ׶��Բ���������ڲ�����Ť���������ӳ�䵽ƽ���ϣ�������������ӳ�����ʱ�������һ���̶ȵ�Ť����һ����ԣ�������������Խ����������Ҫ��Ť���Ⱦ�Խ����ʱ��ֱ��ָ����������һ��ͱȽ������ˡ�

���´���ͨ��һ���̳���osg::NodeVisitor����������������������ģ�͵����ж��㼰���ߣ�Ȼ����ݶ��㡢���߼�һ���ı�����ȷ���������ꡣ��������룺
*/
// stdafx.h

#include <osg/Node>
#include <osg/Geometry>
#include <osg/Geode>  
#include <osg/Group>

#include <osg/Camera>

#include <osg/MatrixTransform> // �ƶ��ڵ�ľ����࣬��õ��ƶ��ڵ���ࡣ���涯����ת���ƽڵ㡣
#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osg/TexGen> //  ָ�������Զ�������������ĺ�����������������ļ��㷽ʽ������������ռ仹���������ռ������в�ͬ�ļ���
#include <osg/TexEnv>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>



// .cpp
 
/*
 *ֻ�����������棨��Բ׶��Բ���������ڲ�����Ť���������ӳ�䵽ƽ����
 *������������ӳ�����ʱ�������һ���̶ȵ�Ť��
 *������������Խ����������Ҫ��Ť���Ⱦ�Խ��
 *���ֱ��ָ�����������Ƿǳ�������ģ��������������꣩
 *
 *ͨ��һ���̳���osg::NodeVisitor����������������������ģ�͵����ж��㼰����
 *���ݶ��㡢���߼�һ���ı�����ȷ����������
*/
 
//�����������������̳���NodeVisitor
class TexCoordGenerator:public osg::NodeVisitor
{
public:
	//���������ӽڵ�
	TexCoordGenerator() :NodeVisitor(NodeVisitor::TRAVERSE_ALL_CHILDREN)
	{
		//
	}
 
	//ͨ����Χ����ȷ�����ʵı���
	void apply(osg::Geode &geode)
	{
		const osg::BoundingSphere &bsphere = geode.getBound();
 
		float scale = 10;
 
		if (bsphere.radius() != 0)
		{
			scale = 5 / bsphere.radius();
		}
 
		//�������м����壬��������������
		for (unsigned i = 0; i < geode.getNumDrawables(); ++i)
		{
			osg::Geometry *geo = dynamic_cast<osg::Geometry *>(geode.getDrawable(i));
 
			if (geo)
			{
				osg::Vec2Array *tc = generate_coords(geo->getVertexArray(), geo->getNormalArray(), scale);
				geo->setTexCoordArray(0, tc);
			}
		}
		NodeVisitor::apply(geode);
	}
protected:
	//������������
	osg::Vec2Array *generate_coords(osg::Array *vx, osg::Array *nx, float scale)
	{
		osg::Vec2Array *v2a = dynamic_cast<osg::Vec2Array *>(vx);
		osg::Vec3Array *v3a = dynamic_cast<osg::Vec3Array *>(vx);
		osg::Vec4Array *v4a = dynamic_cast<osg::Vec4Array *>(vx);
		osg::Vec2Array *n2a = dynamic_cast<osg::Vec2Array *>(vx);
		osg::Vec3Array *n3a = dynamic_cast<osg::Vec3Array *>(vx);
		osg::Vec4Array *n4a = dynamic_cast<osg::Vec4Array *>(vx);
 
		osg::ref_ptr<osg::Vec2Array> tc = new osg::Vec2Array();
		for (unsigned i = 0; i < vx->getNumElements(); ++i)
		{
			osg::Vec3 P;
			if (v2a)
				P.set((*v2a)[i].x(), (*v2a)[i].y(), 0);
			if (v3a)
				P.set((*v3a)[i].x(), (*v3a)[i].y(), (*v3a)[i].z());
			if (v4a)
				P.set((*v4a)[i].x(), (*v4a)[i].y(), (*v4a)[i].z());
			
			osg::Vec3 N(0, 0, 1);
			if (n2a)
				N.set((*n2a)[i].x(), (*n2a)[i].y(), 0);
			if (n3a)
				N.set((*n3a)[i].x(), (*n3a)[i].y(), (*n3a)[i].z());
			if (n4a)
				N.set((*n4a)[i].x(), (*n4a)[i].y(), (*n4a)[i].z());
 
			int axis = 0;
			if (N.y() > N.x() && N.y() > N.z())
				axis = 1;
			if (-N.y() > N.x() && -N.y() > N.z())
				axis = 1;
			if (N.z() > N.x() && N.z() > N.y())
				axis = 2;
			if (-N.z() > N.x() && -N.z() > N.y())
				axis = 2;
			
			
			osg::Vec2 uv;
 
			switch (axis)
			{
				case 0: uv.set(P.y(), P.z());
					break;
				case 1: uv.set(P.x(), P.z());
					break;
				case 2: uv.set(P.x(), P.y());
					break;
				default:;
			}
 
			tc->push_back(uv * scale);
		}
		return tc.release();
	}
};
 
//������ά����״̬����
osg::ref_ptr<osg::StateSet> createTexture2DState(osg::ref_ptr<osg::Image> image)
{
	//������ά�������
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
	texture->setDataVariance(osg::Object::DYNAMIC);

	//������ͼ
	texture->setImage(image.get());

	//�����˲�
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

	//���û���ģʽ
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
 
	//����״̬������
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);
 
	return stateset.release();
}
 
int TestComputingTextureCoordinates()
{
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/primitives.gif");
	osg::ref_ptr<osg::StateSet> stateset = createTexture2DState(image.get());
 
	TexCoordGenerator tcg;
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("dumptruck.osg");
	node->accept(tcg);
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
