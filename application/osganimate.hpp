#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>

#include <osgUtil/Optimizer>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/ReaderWriter>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>

#include <osgSim/OverlayNode>

#include <osgViewer/Viewer>
#include <iostream>
#include <osgEarthSymbology/LineSymbol>
#include <osgEarthSymbology/AltitudeSymbol>
#include <osgEarthSymbology/Geometry>
#include <osgEarthFeatures/Feature>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/FeatureEditing>


osg::ref_ptr<osg::Group> root;
osg::ref_ptr<osgEarth::MapNode> m_pMapNode;

osg::AnimationPath* createAnimationPath(const osg::Vec3& center,float radius,double looptime)
{
	// set up the animation path
	osg::AnimationPath* animationPath = new osg::AnimationPath;
	animationPath->setLoopMode(osg::AnimationPath::LOOP);

	int numSamples = 40;
	float yaw = 0.0f;
	float yaw_delta = 2.0f*osg::PI/((float)numSamples-1.0f);
	float roll = osg::inDegrees(30.0f);

	double time=0.0f;
	double time_delta = looptime/(double)numSamples;
	for(int i=0;i<numSamples;++i)
	{
		osg::Vec3 position(center+osg::Vec3(sinf(yaw)*radius,cosf(yaw)*radius,0.0f));
		osg::Quat rotation(osg::Quat(roll,osg::Vec3(0.0,1.0,0.0))*osg::Quat(-(yaw+osg::inDegrees(90.0f)),osg::Vec3(0.0,0.0,1.0)));

		animationPath->insert(time,osg::AnimationPath::ControlPoint(position,rotation));

		yaw += yaw_delta;
		time += time_delta;

	}
	return animationPath;
}

osg::Node* createBase(const osg::Vec3& center,float radius)
{
	int numTilesX = 10;
	int numTilesY = 10;

	float width = 2*radius;
	float height = 2*radius;

	osg::Vec3 v000(center - osg::Vec3(width*0.5f,height*0.5f,0.0f));
	osg::Vec3 dx(osg::Vec3(width/((float)numTilesX),0.0,0.0f));
	osg::Vec3 dy(osg::Vec3(0.0f,height/((float)numTilesY),0.0f));

	// fill in vertices for grid, note numTilesX+1 * numTilesY+1...
	osg::Vec3Array* coords = new osg::Vec3Array;
	int iy;
	for(iy=0;iy<=numTilesY;++iy)
	{
		for(int ix=0;ix<=numTilesX;++ix)
		{
			coords->push_back(v000+dx*(float)ix+dy*(float)iy);
		}
	}

	//Just two colours - black and white.
	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f,1.0f,1.0f,1.0f)); // white
	colors->push_back(osg::Vec4(0.0f,0.0f,0.0f,1.0f)); // black

	osg::ref_ptr<osg::DrawElementsUShort> whitePrimitives = new osg::DrawElementsUShort(GL_QUADS);
	osg::ref_ptr<osg::DrawElementsUShort> blackPrimitives = new osg::DrawElementsUShort(GL_QUADS);

	int numIndicesPerRow=numTilesX+1;
	for(iy=0;iy<numTilesY;++iy)
	{
		for(int ix=0;ix<numTilesX;++ix)
		{
			osg::DrawElementsUShort* primitives = ((iy+ix)%2==0) ? whitePrimitives.get() : blackPrimitives.get();
			primitives->push_back(ix    +(iy+1)*numIndicesPerRow);
			primitives->push_back(ix    +iy*numIndicesPerRow);
			primitives->push_back((ix+1)+iy*numIndicesPerRow);
			primitives->push_back((ix+1)+(iy+1)*numIndicesPerRow);
		}
	}

	// set up a single normal
	osg::Vec3Array* normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f,0.0f,1.0f));

	osg::Geometry* geom = new osg::Geometry;
	geom->setVertexArray(coords);

	geom->setColorArray(colors, osg::Array::BIND_PER_PRIMITIVE_SET);

	geom->setNormalArray(normals, osg::Array::BIND_OVERALL);

	geom->addPrimitiveSet(whitePrimitives.get());
	geom->addPrimitiveSet(blackPrimitives.get());

	osg::Geode* geode = new osg::Geode;
	geode->addDrawable(geom);

	return geode;
}

osg::Node* createMovingModel(const osg::Vec3& center, float radius)
{
	float animationLength = 10.0f;

	osg::AnimationPath* animationPath = createAnimationPath(center,radius,animationLength);

	osg::ref_ptr<osg::Group> model = new osg::Group;

	osg::ref_ptr<osg::Node> glider = osgDB::readRefNodeFile("glider.osgt");

	m_pMapNode = dynamic_cast<osgEarth::MapNode*>(glider.get());

	if (glider)
	{
		const osg::BoundingSphere& bs = glider->getBound();

		float size = radius/bs.radius()*0.3f;
		osg::MatrixTransform* positioned = new osg::MatrixTransform;
		positioned->setDataVariance(osg::Object::STATIC);
		positioned->setMatrix(osg::Matrix::translate(-bs.center())*

			osg::Matrix::scale(size,size,size)*
			osg::Matrix::rotate(osg::inDegrees(-90.0f),0.0f,0.0f,1.0f));

		positioned->addChild(glider);

		osg::PositionAttitudeTransform* xform = new osg::PositionAttitudeTransform;
		xform->setUpdateCallback(new osg::AnimationPathCallback(animationPath,0.0,1.0));
		xform->addChild(positioned);

		model->addChild(xform);
	}

	osg::ref_ptr<osg::Node> cessna = osgDB::readRefNodeFile("cessna.osgt");
	if (cessna)
	{
		const osg::BoundingSphere& bs = cessna->getBound();

		float size = radius/bs.radius()*0.3f;
		osg::MatrixTransform* positioned = new osg::MatrixTransform;
		positioned->setDataVariance(osg::Object::STATIC);
		positioned->setMatrix(osg::Matrix::translate(-bs.center())*
			osg::Matrix::scale(size,size,size)*
			osg::Matrix::rotate(osg::inDegrees(180.0f),0.0f,0.0f,1.0f));

		positioned->addChild(cessna);

		osg::ref_ptr<osg::MatrixTransform> xform = new osg::MatrixTransform;
		xform->setUpdateCallback(new osg::AnimationPathCallback(animationPath,0.0f,2.0));
		xform->addChild(positioned);

		model->addChild(xform);
	}

#ifndef OSG_GLES2_AVAILABLE
	model->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
#endif

	return model.release();
}

osg::ref_ptr<osg::Group> createModel(bool overlay, osgSim::OverlayNode::OverlayTechnique technique)
{
	osg::Vec3 center(0.0f,0.0f,0.0f);
	float radius = 100.0f;

	root = new osg::Group;

	float baseHeight = center.z()-radius*0.5;
	osg::ref_ptr<osg::Node> baseModel = createBase(osg::Vec3(center.x(), center.y(), baseHeight),radius);
	osg::ref_ptr<osg::Node> movingModel = createMovingModel(center,radius*0.8f);

	if (overlay)
	{
		osgSim::OverlayNode* overlayNode = new osgSim::OverlayNode(technique);
		overlayNode->setContinuousUpdate(true);
		overlayNode->setOverlaySubgraph(movingModel);
		overlayNode->setOverlayBaseHeight(baseHeight-0.01);
		overlayNode->addChild(baseModel);
		root->addChild(overlayNode);
	}
	else
	{

		root->addChild(baseModel);
	}

	root->addChild(movingModel);

	/*********���ƹ켣**********/
	std::vector<osg::Vec3d> m_vecPoint;
	m_vecPoint.push_back(osg::Vec3d(116.7176, 20.6994, 100000.0));
	m_vecPoint.push_back(osg::Vec3d(116.3, 39.9, 100000.0));
	// createLine(m_vecPoint, movingModel);




	return root;
}


int TestOsgAnimate()
{

	bool overlay = false;

	osgSim::OverlayNode::OverlayTechnique technique = osgSim::OverlayNode::OBJECT_DEPENDENT_WITH_ORTHOGRAPHIC_OVERLAY;

	// initialize the viewer.
	osgViewer::Viewer viewer;

	// load the nodes from the commandline arguments.
	osg::ref_ptr<osg::Group> model = createModel(overlay, technique);
	if (!model)
	{
		return 1;
	}

	// tilt the scene so the default eye position is looking down on the model.
	osg::ref_ptr<osg::MatrixTransform> rootnode = new osg::MatrixTransform;
	rootnode->setMatrix(osg::Matrix::rotate(osg::inDegrees(30.0f),1.0f,0.0f,0.0f));
	rootnode->addChild(model);

	// run optimization over the scene graph
	osgUtil::Optimizer optimzer;
	optimzer.optimize(rootnode);

	std::string filename;


	// set the scene to render
	viewer.setSceneData(rootnode);

	viewer.setCameraManipulator(new osgGA::TrackballManipulator());

	// viewer.setUpViewOnSingleScreen(1);

#if 0

	// use of custom simulation time.

	viewer.realize();

	double simulationTime = 0.0;

	while (!viewer.done())
	{
		viewer.frame(simulationTime);
		simulationTime += 0.001;
	}

	return 0;
#else

	// normal viewer usage.
	return viewer.run();

#endif
}
