#include <osg/Image>
#include <osgGA/StateSetManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarth/Registry>
#include <osgEarthSymbology/Geometry>
#include <osgEarthSymbology/GeometryRasterizer>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarth/ImageToHeightFieldConverter>
#include <osgEarth/ImageUtils>
#include <osgEarth/FileUtils>
#include <osgEarth/Registry>
#include <osgEarth/MapFrame>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgEarthUtil/ExampleResources>
#include <math.h>

#include <osgGA/NodeTrackerManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/KeySwitchMatrixManipulator>


#include "EarthWalkManipulato.h"


using namespace osgEarth;
using namespace osgEarth::Util;
using namespace osgEarth::Symbology;


int TestEarthWalk(int argc, char** argv)
{
	//������.earth�ļ�����
	osg::ArgumentParser arguments(&argc, argv);
	osgViewer::Viewer viewer;
	MapNode* s_mapNode = 0L;
	osg::Node* earthFile = osgDB::readNodeFile("gdal_interp.earth");//MapNodeHelper().load(arguments, &viewer);
	if (earthFile)
		s_mapNode = MapNode::get(earthFile);
	if (!s_mapNode)
	{
		OE_WARN << "Unable to load earth file." << std::endl;
		return -1;
	}
	osg::Group* root = new osg::Group();
	root->addChild(earthFile);

	osg::Matrix matrixGood1;
	osg::Vec3d geopoint1, geopoint2, geopoint3;
	GeoPoint point1(s_mapNode->getMapSRS(), 0, 0, 1000);
	point1.createLocalToWorld(matrixGood1);
	//matrixGood1.getLookAt(geopoint1, geopoint2, geopoint3);
	//osg::Vec3 _vector = geopoint1 - geopoint2;

	//���һͷţ���鿴λ���Ƿ���ȷ
	//osg::Node* cow = osgDB::readNodeFile("D:\\temp\\node\\cow.osg");
	//
	//osg::ref_ptr<osg::MatrixTransform> pat = new osg::MatrixTransform;
	//pat->addChild(cow);
	//pat->setMatrix(osg::Matrix::scale(200000, 200000, 200000));

	//osg::Matrix maaat2;
	//osg::ref_ptr<osg::MatrixTransform> pat2 = new osg::MatrixTransform;
	//pat2->setMatrix(osg::Matrix::rotate(matrixGood1.getRotate())*
	//    osg::Matrix::translate(matrixGood1.getTrans()));
	//pat2->addChild(pat);
	//root->addChild(pat2);

	viewer.setSceneData(root);

	//ģ��������
	osgGA::NodeTrackerManipulator* nodeTrack = new osgGA::NodeTrackerManipulator();
	nodeTrack->setTrackNode(root);

	/*************************************����������**��*********************************/
	GeoPoint gPoint1(s_mapNode->getMap()->getSRS(), 32, 118, 400);
	osg::Matrix gMatrix1;
	gPoint1.createLocalToWorld(gMatrix1);//��ȡ��ǰ�����ϵ���ȷ��̬

	//����������������¿��������ڵ�ǰ��̬������̧��60�ȣ�ע����ǰ�ˣ�
	gMatrix1.preMultRotate(osg::Quat(osg::DegreesToRadians(60.0), osg::X_AXIS));
	osg::Quat q1;    gMatrix1.get(q1);//��ȡ��ǰ������̬
	osg::Vec3d vPos1 = gMatrix1.getTrans();//��ȡ��ǰ����λ��

	GeoPoint gPoint2(s_mapNode->getMap()->getSRS(), 32.01, 118.01, 400);
	osg::Matrix gMatrix2;
	gPoint2.createLocalToWorld(gMatrix2);
	gMatrix2.preMultRotate(osg::Quat(osg::DegreesToRadians(60.0), osg::X_AXIS));
	osg::Quat q2;    
	gMatrix2.get(q2);
	osg::Vec3d vPos2 = gMatrix2.getTrans();

	GeoPoint gPoint3(s_mapNode->getMap()->getSRS(), 32.02, 118.02, 400);
	osg::Matrix gMatrix3;
	gPoint3.createLocalToWorld(gMatrix3);
	osg::Quat q3;
	gMatrix3.get(q3);
	osg::Vec3d vPos3 = gMatrix3.getTrans();
	//��ȡ���֮����˳��ת,��ʵ�Ǵ������̬
	osg::Quat qbuf(osg::DegreesToRadians(60.0), osg::X_AXIS);
	q3    *= qbuf;

	//ʹ�ö���������
	osgGA::AnimationPathManipulator *animationPathMp = new osgGA::AnimationPathManipulator();
	//��������������ӹؼ�֡
	osg::AnimationPath* _animationPath = new osg::AnimationPath;
	_animationPath->insert(0.0, osg::AnimationPath::ControlPoint(vPos1, q1));//��̬��ȷ
	_animationPath->insert(3.0, osg::AnimationPath::ControlPoint(vPos2, q2));//��̬��ȷ
	_animationPath->insert(6.0, osg::AnimationPath::ControlPoint(vPos3, q3));//��̬����
	_animationPath->setLoopMode(osg::AnimationPath::SWING);//����·���ǻذڵ�
	animationPathMp->setAnimationPath(_animationPath);


	/*************************************����������**��*********************************/

	//�������������������ʹ��һ������������ѡ�񣬰����̡�3�����л���·��������������
	osgGA::KeySwitchMatrixManipulator* keyPtr = new osgGA::KeySwitchMatrixManipulator();
	keyPtr->addMatrixManipulator('1', "earthMan", new EarthManipulator());
	keyPtr->addMatrixManipulator('2', "trakerMan", nodeTrack);
	keyPtr->addMatrixManipulator('3', "animationPathMan", animationPathMp);
	keyPtr->addMatrixManipulator('4', "earthWalkMan", new EarthWalkManipulator());
	viewer.setCameraManipulator(keyPtr);
	//viewer.setUpViewOnSingleScreen(0);


	{
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		traits->x = 40;
		traits->y = 40;
		traits->width = 600;
		traits->height = 480;
		traits->windowDecoration = true;
		traits->doubleBuffer = true;
		traits->sharedContext = 0;

		osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

		osg::ref_ptr<osg::Camera> camera = new osg::Camera;
		camera->setGraphicsContext(gc.get());
		camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
		GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
		camera->setDrawBuffer(buffer);
		camera->setReadBuffer(buffer);

		// add this slave camera to the viewer, with a shift left of the projection matrix
		viewer.addSlave(camera.get());
	}

	while(!viewer.done())
		viewer.frame();


	return 1;
}