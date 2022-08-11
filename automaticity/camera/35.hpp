#include <osgViewer/Viewer>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgText/Font>
#include <osgText/Text>
#include <osgDB/ReadFile>

int Test35()
{
    osgViewer::Viewer viewer;

    osg::Group* root = new osg::Group;

    osg::Geode* geode = new osg::Geode;
    root->addChild(geode);
    osgText::Text* text1 = new osgText::Text;
    text1->setFont("fonts/simhei.ttf");
    text1->setCharacterSize(100.0);
    text1->setPosition(osg::Vec3(0.0, 0.0, 0.0));
    text1->setAxisAlignment(osgText::Text::SCREEN);
    text1->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
    text1->setText(L"中国");
    geode->addDrawable(text1);

    viewer.setSceneData(root);

    return viewer.run();
}