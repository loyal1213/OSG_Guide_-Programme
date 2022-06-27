#include <osg/Node>
#include <osg/Geode>
#include <osgViewer/Viewer>
#include <osg/Geometry>

osg::ref_ptr<osg::Geode> CreateQuad(){
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;


	//首先定义四个点



	return geode;
}


void TestGeometry(){
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();


	viewer->setSceneData(CreateQuad().get());

	viewer->realize();
	viewer->run();

}