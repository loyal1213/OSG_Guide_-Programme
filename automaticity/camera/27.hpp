#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgGA/GUIEventHandler>
#include <osg/Texture2D>
#include <osgDB/WriteFile>

osg::ref_ptr<osg::Image> _t2d = new osg::Image;


class MyEventHandler : public osgGA::GUIEventHandler
{
public:
    MyEventHandler() 
    {
    }

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
    {
        //µã»÷p¼ü
        if ((ea.getEventType() == ea.KEYDOWN)&&((ea.getKey() == 'P')|| (ea.getKey() == 'p')))
        {
            osgDB::writeImageFile(*_t2d, "a.png");
        }

        return false;
    }
};


int Test27(int argc, char** argv) 
{
    osgViewer::Viewer viewer;

    viewer.setSceneData(osgDB::readNodeFile("cow.osg"));
    viewer.addEventHandler(new MyEventHandler());

    //ÉèÖÃrtt
    viewer.getCamera()->attach(osg::Camera::COLOR_BUFFER, _t2d);
    //

    viewer.run();
    return 0;
}