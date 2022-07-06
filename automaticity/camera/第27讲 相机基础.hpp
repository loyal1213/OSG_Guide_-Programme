#include <osgViewer/Viewer>
#include <osgDB/ReadFile>

#include <osg/Group>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>

#include <osg/Camera>
#include <osg/GraphicsContext>

void CreateCamera(osgViewer::Viewer *viewer){

	if (!viewer){

		return ;
	}

	osg::GraphicsContext::WindowingSystemInterface *wsi = osg::GraphicsContext::getWindowingSystemInterface();

	if (!wsi){
		return ;
	}

	unsigned int width = 0, height = 0;

	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0),width,height);

	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
	traits->x = 0;
	traits->y = 0;
	traits->width = width;
	traits->height = height;

	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	osg::ref_ptr<osg::GraphicsContext> gc  = osg::GraphicsContext::createGraphicsContext(traits);
	if (!gc->valid()){
		return ;
	}

	gc->setClearColor(osg::Vec4(0.5,0.5,0.5,1.0));
	gc->setClearMask(GL_COLOR_BUFFER_BIT);

	osg::ref_ptr<osg::Camera> camera = new osg::Camera();
	camera->setGraphicsContext(gc);
	camera->setViewport(0,0, width, height);
	viewer->addSlave(camera.get());


	osg::ref_ptr<osg::Camera> camera_left = new osg::Camera();
	// camera_left->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	// camera_left->setClearColor(osg::Vec4(0.0,0.0,0.0,1.0));
	camera_left->setGraphicsContext(gc);
	camera_left->setViewport(0,0, width/4, height/4);
	viewer->addSlave(camera_left.get(), osg::Matrix::scale(5 , 5.0, 5.0), osg::Matrix::rotate(osg::RadiansToDegrees(0.0), 0.0, 1.0, 0.0));

	/*osg::ref_ptr<osg::Camera> front_camera = new osg::Camera();
	front_camera->setGraphicsContext(gc);
	front_camera->setViewport(width * 1/4,0, width/4, height/4);
	viewer->addSlave(front_camera.get());


	osg::ref_ptr<osg::Camera> right_camera = new osg::Camera();
	right_camera->setGraphicsContext(gc);
	right_camera->setViewport(width * 2/4,0, width/4, height/4);
	viewer->addSlave(right_camera.get());


	osg::ref_ptr<osg::Camera> back_camera = new osg::Camera();
	camera->setGraphicsContext(gc);
	camera->setViewport(0,0, width * 3/4, height/4);
	viewer->addSlave(camera.get());*/


}


void TestCamera(){


	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	osg::ref_ptr<osg::Group> group = new osg::Group();
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("glider.osg");

	/*osg::ref_ptr<osg::Camera> camera = new osg::Camera();


	camera->addChild(node.get());
	camera->setViewport(0,0,400,200);
	group->addChild(camera.get());

	*/

	group->addChild(node.get());

	CreateCamera(viewer);

	viewer->setSceneData(group.get());

	viewer->realize();

	viewer->run();
}