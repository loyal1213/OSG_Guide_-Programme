#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgGA/EventHandler>

osg::Node* _base = nullptr;
osg::Group* _root = new osg::Group;

class MyEventHandler : public osgGA::GUIEventHandler
{
public:
    MyEventHandler()
    {}

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
    {
        if (ea.getEventType() == ea.KEYDOWN)
        {
            if ((ea.getKey() == 'A') || (ea.getKey() == 'a'))
            {
                if (ea.getModKeyMask() == ea.MODKEY_LEFT_SHIFT)
                {
                    std::cout << "按下了左SHIFT+A" << std::endl;
                }

                if (ea.getButtonMask() == ea.LEFT_MOUSE_BUTTON)
                {
                    std::cout << "按下了鼠标左键+A" << std::endl;
                }
            }
        }
        return false;
    }
};


int Test14()
{
    osgViewer::Viewer viewer;
    viewer.setSceneData(osgDB::readNodeFile("glider.osg"));
    viewer.addEventHandler(new MyEventHandler());
    return viewer.run();
}