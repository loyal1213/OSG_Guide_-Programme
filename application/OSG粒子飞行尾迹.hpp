#include <iostream>
#include <osgViewer/Viewer>
#include <osgViewer/GraphicsWindow>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Camera>
#include <osg/Image>
#include <osg/BufferObject>
#include <osg/GraphicsContext>

#include <osg/MatrixTransform>
#include <osg/NodeCallback>
#include <osg/NodeVisitor>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgGA/AnimationPathManipulator>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIActionAdapter>

#include <osgUtil/Optimizer>

#include <osgParticle/FireEffect>

//设置飞机动态移动
class transCallback : public osg::NodeCallback
{
public:
	transCallback():delta(0.0){}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>(node);
		if(mt)
		{
			mt->setMatrix(osg::Matrix::translate(0,delta,0));
			delta += 0.1;
			traverse(node,nv);
		}
	}
private:
	float delta;
};


//设置飞机尾焰的位置
class firePosCallback : public osgGA::GUIEventHandler 
{
public:
	firePosCallback(osg::ref_ptr<osg::MatrixTransform> mt,osg::ref_ptr<osgParticle::FireEffect> _fire):mat(mt),fire(_fire){}
	~firePosCallback(){}

	bool handle(const osgGA::GUIEventAdapter &ea   ,osgGA::GUIActionAdapter &aa)  
	{
		osgViewer::View *viewer = dynamic_cast<osgViewer::View*>(&aa);  
		switch (ea.getEventType())
		{  
		case osgGA::GUIEventAdapter::FRAME:
			{
				osg::Vec3 position = mat->getMatrix().getTrans();
				fire->setPosition(position);
			}
			break;
		default:
			return false;
		}
		return false;
	}

private:
	osg::ref_ptr<osgParticle::FireEffect> fire;
	osg::ref_ptr<osg::MatrixTransform> mat;
};

//函数入口
int TestTrail()
{
	osg::ref_ptr<osg::Group> root = new osg::Group;

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("cessna.osg");
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	mt->setUpdateCallback(new transCallback);
	mt->addChild(node);

	osg::ref_ptr<osgParticle::FireEffect> fire = new osgParticle::FireEffect(osg::Vec3(0,0,0),50,1);
	fire->setWind(osg::Vec3(1.0f,0.0f,0.0f));

	osg::ref_ptr<osg::MatrixTransform> mtfire = new osg::MatrixTransform;
	mtfire->setMatrix(osg::Matrix::rotate(osg::PI_2,osg::X_AXIS));
	mtfire->addChild(fire);

	root->addChild(mt);
	root->addChild(node);
	root->addChild(mtfire);

	osgViewer::Viewer viewer;
	viewer.addEventHandler(new firePosCallback(mt,fire));
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.setUpViewInWindow(50,50,800,600);
	viewer.setSceneData( root.get() );
	return viewer.run();
}
