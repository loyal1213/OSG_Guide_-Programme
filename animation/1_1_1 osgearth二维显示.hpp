#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/cache_filesystem/FileSystemCache>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/GeodeticGraticule>
#include <osgEarthUtil/LatLongFormatter>
#include <osgEarthUtil/Controls>
#include <osgEarthUtil/MouseCoordsTool>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osg/PositionAttitudeTransform>
#include <osg/Group>
#include <osg/Node>
#include <osgDB/ReadFile>

#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>
#include <Windows.h>

#include <osgEarth/MapNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>

#include <osgViewer/Viewer>
#include <osgDB/WriteFile>
using namespace std;
using namespace osgEarth;
using namespace osgEarth::Util;

void TestOsgEarth2D(){
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	//定义根节点
	//osg::Group* root = new osg::Group();
	osg::ref_ptr<osg::Group> root = new osg::Group();	//智能指针方式
	osg::ref_ptr<osg::Node> earthNode = osgDB::readNodeFile("../data/earth_file/china_simple.earth");

	if(!earthNode){
		OE_NOTICE << "Unable to load earth model" << std::endl;
		return ;
	}
	root->addChild(earthNode);
	//查询地图节点
	osgEarth::MapNode* mapNode = osgEarth::MapNode::findMapNode(earthNode);
	if (!mapNode){
		OE_NOTICE << "Could not find MapNode " << std::endl;
		return ;
	}


	//添加到场景
	viewer->setSceneData(root.get());
	viewer->realize();


	// 设置时间;
	osgEarth::DateTime dateTime(2019, 5, 8, 9);
	osgEarth::Util::Ephemeris* ephemeris = new osgEarth::Util::Ephemeris;

	osgEarth::Util::SkyNode* m_pSkyNode = osgEarth::Util::SkyNode::create(mapNode);
	m_pSkyNode->setName("SkyNode");
	m_pSkyNode->setEphemeris(ephemeris);
	m_pSkyNode->setDateTime(dateTime);
	m_pSkyNode->attach(viewer, 0);
	m_pSkyNode->setLighting(true);
	m_pSkyNode->addChild(mapNode);
	root->addChild(m_pSkyNode);

	viewer->run();
}