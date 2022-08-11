#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgGA/GUIEventHandler>

class MyEventHandler : public osgGA::GUIEventHandler
{
public:
    MyEventHandler() 
    {
        _isPerspective = true;
    }

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
    {
        osgViewer::Viewer* viewer = (osgViewer::Viewer*)(&aa);

        if (ea.getEventType() == ea.DOUBLECLICK)
        {
            if (_isPerspective)
            {
                viewer->getCamera()->setProjectionMatrixAsOrtho(-20*1.3, 20*1.3, -20, 20, 0, 1000);
            }
            else
            {
                viewer->getCamera()->setProjectionMatrixAsPerspective(30, 1.3, 0.1, 100);
            }

            _isPerspective = !_isPerspective;
      
        }

        return false;
    }

    bool _isPerspective;
};


int Test25(int argc, char** argv) 
{

    osgViewer::Viewer viewer;
    viewer.setSceneData(osgDB::readNodeFile("cow.osg"));
    viewer.addEventHandler(new MyEventHandler());
    viewer.run();

    return 0;
}