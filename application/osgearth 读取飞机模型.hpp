#include <Windows.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>
#include <osgEarth/MapNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>

//�����¼�
#include<osgViewer/ViewerEventHandlers>

//osgGA��صĿ�
#include<osgGA/StateSetManipulator>

#include <osgViewer/Viewer>
#include <osgDB/WriteFile>

using namespace std;


int TestOsgearthPlane()
{
	osg::ref_ptr<osgViewer::Viewer>viewer = new osgViewer::Viewer;

	//���״̬�¼���������Ӧ���̺�����¼�����ӦL T B W
	viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));

	//���ڴ�С�仯����ӦF
	viewer->addEventHandler(new osgViewer::WindowSizeHandler);

	//���·����¼ Z
	viewer->addEventHandler(new osgViewer::RecordCameraPathHandler);

	//�����ĵ���ʾH
	viewer->addEventHandler(new osgViewer::HelpHandler);

	//���� C
	viewer->addEventHandler(new osgViewer::ScreenCaptureHandler);

	//���һЩ����״̬���ã���ӦS
	viewer->addEventHandler(new osgViewer::StatsHandler);

	//������
	osg::ref_ptr<osgEarth::Util::EarthManipulator> earthManipulator = new osgEarth::Util::EarthManipulator;
	//�������������
	viewer->setCameraManipulator(earthManipulator);

	//���ڵ�
	osg::ref_ptr<osg::Group> root = new osg::Group;

	//���ص���ڵ�
	osg::ref_ptr<osg::Node> earthNode = osgDB::readNodeFile("gdal_interp.earth");
	//������ڵ������ڵ�
	root->addChild(earthNode.get());



	//����ͼ�ڵ��Ƿ񴴽���
	osg::ref_ptr<osgEarth::MapNode> mapNode = osgEarth::MapNode::findMapNode(earthNode.get());
	if (!mapNode.get()) return 0;

	//�ռ����ã�ˮƽ�ʹ�ֱ
	const osgEarth::SpatialReference *geoSRS = mapNode->getMapSRS()->getGeographicSRS();

	//���ģ��
	osg::Node* model = osgDB::readNodeFile("cessna.osg");
	//osg�й���ֻ����з��ߵ�ģ�������ã���ģ�;������ź����ǲ����ã�
	//������Ҫ�ֶ��������ԣ��÷�������ģ�ʹ�С�仯���仯��GL_NORMALIZE �� GL_RESCALE_NORMAL
	model->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);

	osg::Matrix Lmatrix;
	geoSRS->getEllipsoid()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(40.0), osg::DegreesToRadians(116.0), 500000.0, Lmatrix);  //ά�ȣ����ȣ��߶ȣ�localToWorld

	
	//�Ŵ�һЩ�����㿴��
	Lmatrix.preMult(osg::Matrix::scale(osg::Vec3(30000, 30000, 30000)));//x,y,z��Ŵ���


	osg::MatrixTransform* mt = new osg::MatrixTransform;
	mt->setMatrix(Lmatrix);
	mt->addChild(model);
	root->addChild(mt);

	//�����ֳ�����
	viewer->setSceneData(root.get());
	//ʵ��
	viewer->realize();

	// �ӵ㶨λ��������,�˾�������к����ֱ�Ӷ�λ�������꣬ע�ͺ�����������ʾģ�ͣ����������Զ���λ
	// Viewpoint(const char* name, double lon, double lat, double z, double heading, double pitch, double range);
	earthManipulator->setViewpoint(osgEarth::Viewpoint("ģ�����˻�", 116, 40, 0.0, 0.0, -90.0, 1.5e6));


	return viewer->run();


}