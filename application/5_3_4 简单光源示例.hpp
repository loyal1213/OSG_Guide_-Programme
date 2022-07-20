/*
�ڳ�������ӹ�Դ��Ҫ����Ϊ��

����Ⱦ״̬��ָ������ģ�͵ķ��ߡ�������Ⱦ״̬�п������ղ������Դ��������osg::Light�����Զ����Դ��������������Դosg::LightSource���󣬽���Դ�����������С�������Դ�ڵ���ӵ�����ͼ���С�

���У���Ҫ��ȷ���ǣ�����ģ��ֻ�������е�λ����ʱ�Ż���ȷ��ʾ���ա�

*/
// stdafx.h

#include <osg/Node>
#include <osg/Geode>  
#include <osg/Geometry>
#include <osg/Group>

#include <osg/Camera>

#include <osg/Light>			//	�̳���osg::StateAttribute������ƹ��ģʽ�����Բ�����Ϣ
#include <osg/LightSource>	    //  �̳���osg::Group���ƹ�����࣬�̳���osg::Group��Ĺ���ڵ�Ľӿڣ����ƹ���Ϊһ���ڵ���뵽����ͼ�н�����Ⱦ

#include <osg/BoundingSphere>
#include <osg/BoundingBox>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgViewer/ViewerEventHandlers> //  �¼�����
#include <osgGA/StateSetManipulator>     //	�¼���Ӧ�࣬����Ⱦ״̬���п��ƣ���w����/��/��ģʽ���л�����l�������������ģʽ���л�����b���Ƿ��������޳�ģʽ���л�����t���Ƿ��������������л�

#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>


//.cpp

/*
�ڳ�������ӹ�Դ��
����Ⱦ״̬��ָ������ģ�͵ķ���
����Ⱦ״̬�п������ղ������Դ
����osg::Light�����Զ����Դ����
������Դosg::LightSource���󣬽���Դ������������
����Դ�ڵ���ӵ�����ͼ����
����ģ��ֻ�������е�λ����ʱ�Ż���ȷ��ʾ����
*/

//�򳡾�����ӹ�Դ
osg::ref_ptr<osg::Group> CreateLight(osg::ref_ptr<osg::Node> node)
{
	osg::ref_ptr<osg::Group> lightRoot = new osg::Group();
	lightRoot->addChild(node);

	// ��������
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset = lightRoot->getOrCreateStateSet();

	//�����ռ����������������߰���ʱ�������ߵ�������
	//stateset->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);

	//�����е����ű任�Ǿ���ʱ�������߹�һ�����Ա�֤����Ϊ��λ����
	//stateset->setMode(GL_NORMALIZE, osg::StateAttribute::ON);

	//Ϊ��ù���Ч��������գ���Ҫ������ղ���������һ����Դ
	//�������� ��ON��ΪOFF��رչ���
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	//����һ����ԴGL_LIGHT0
	stateset->setMode(GL_LIGHT0, osg::StateAttribute::ON);

	//�����Χ��
	osg::BoundingSphere bs;
	node->computeBound();
	bs = node->getBound();

	//����һ��Light���󣬶����Դ����
	osg::ref_ptr<osg::Light> light = new osg::Light();
	light->setLightNum(0);

	//���÷���
	light->setDirection(osg::Vec3(0.0f, 0.0f, -1.0f));
	//����λ��
	light->setPosition(osg::Vec4(bs.center().x(), bs.center().y(), bs.center().z() + bs.radius(), 1.0f));
	//���û��������ɫ
	light->setAmbient(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//����ɢ������ɫ
	light->setDiffuse(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));

	//���ú�˥��ָ��
	light->setConstantAttenuation(1.0f);
	//��������˥��ָ��
	light->setLinearAttenuation(0.0f);
	//���ö��η�˥��ָ��
	light->setQuadraticAttenuation(0.0f);

	// ������Դ
	osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource();
	lightSource->setLight(light.get());


	lightRoot->addChild(lightSource.get());

	return lightRoot.get();
}

int TestLight()
{
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cow.osg");
	osg::ref_ptr<osg::Group> root = new osg::Group();
	root->addChild(CreateLight(node.get()));

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));
	viewer->setSceneData(root.get());
	viewer->realize();

	return viewer->run();
}