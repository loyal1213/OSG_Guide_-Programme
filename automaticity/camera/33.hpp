#include <osgViewer/Viewer>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PrimitiveSet>
#include <osgGA/GUIEventHandler>
#include <osgGA/CameraManipulator>
#include <osgDB/ReadFile>
#include <osg/Geode>
#include <osg/ShapeDrawable>

osg::Group* _root = new osg::Group;

osg::Node* createSphere(osg::Vec3 center, float r)
{
    osg::Geode* gnode = new osg::Geode;
    gnode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(center, r)));
    return gnode;
}

class MyEventHandler :public osgGA::GUIEventHandler
{
public:
    MyEventHandler() {};

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
    {
        if (ea.getEventType() == ea.KEYDOWN)
        {
            if (ea.getKey() == 'p')
            {
                //获取当前视口的分辨率
                osgViewer::Viewer *viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
                if (viewer)
                {
                    osg::Viewport* vp = viewer->getCamera()->getViewport();
                    int width = vp->width();
                    int height = vp->height();
                    //每隔20画个球
                    for (int i = 0; i < width; i += 100)
                    {
                        for (int j = 0; j < height; j += 100)
                        {
                            for (float k = 0; k <1; k+=0.1)
                            {
                                osg::Matrix inverseVPW = osg::Matrix::inverse(viewer->getCamera()->getViewMatrix() *
                                    viewer->getCamera()->getProjectionMatrix()*
                                    viewer->getCamera()->getViewport()->computeWindowMatrix());
                                osg::Vec3 center = osg::Vec3(i, j, k) * inverseVPW;
                                _root->addChild(createSphere(center, 0.1));
                            }
                        }
                    }

                }
            }
        }

        return false;
    }
};

int Test33()
{
	osgViewer::Viewer viewer;

    _root->addChild(osgDB::readNodeFile("cow.osg"));
	viewer.setSceneData(_root);
    viewer.addEventHandler(new MyEventHandler);
	return viewer.run();
}