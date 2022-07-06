#include <osg/Group>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>

#include <osgViewer/Viewer>

#include <osgDB/ReadFile>
#include <osgDB/ReaderWriter>

#include <osg/MatrixTransform>
#include <osg/AnimationPath>

osg::ref_ptr<osg::Node> MatrixOperation(){

	osg::ref_ptr<osg::Group> group = new osg::Group();
	osg::ref_ptr<osg::MatrixTransform> matrix_transform = new osg::MatrixTransform();
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("glider.osg");


	matrix_transform->addChild(node.get());
	matrix_transform->setMatrix(osg::Matrix::translate(5.0,0.0,0.0));
	matrix_transform->setUpdateCallback(new osg::AnimationPathCallback(osg::Vec3(0.0, 2.0 ,0.0), osg::Y_AXIS, 1.0));


	// group->addChild(osgDB::readNodeFile("glider.osg"));
	group->addChild(matrix_transform);
	//group->addChild(node.get());

	return group;

}




void TestMatrix(){

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> group = new osg::Group();



	viewer->setSceneData(MatrixOperation().get());

	viewer->run();

}
