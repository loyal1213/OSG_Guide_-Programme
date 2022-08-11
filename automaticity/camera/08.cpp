#include <osgViewer/viewer>
#include <osgDB/ReadFile>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgGA/CameraManipulator>

class MyCameraManipulator : public osgGA::CameraManipulator
{
public:
    MyCameraManipulator() 
    {
        _theta = 0.0;
        _center = osg::Vec3(0.0, 0.0, 0.0);
        _up = osg::Vec3(0.0, 0.0, 1.0);
        _r = std::sqrt(5 * 5 + 5 * 5);
        _eye = osg::Vec3(_r*std::cos(osg::DegreesToRadians(_theta)), _r * std::sin(osg::DegreesToRadians(_theta)), 5.0);
    }

    //这三个纯虚函数本例不会使用
    virtual void setByMatrix(const osg::Matrixd& matrix) {};
    virtual void setByInverseMatrix(const osg::Matrixd& matrix) {};
    virtual osg::Matrixd getMatrix() const { return osg::Matrix::identity(); };

    //最关键的是这个，这个返回的就是ViewMatrix
    virtual osg::Matrixd getInverseMatrix() const
    {
        return osg::Matrix::lookAt(_eye, _center, _up);
    };

    //事件处理，我们要点击A就围着Z轴顺时针转动，点D就逆时针转动，转的时候始终朝0 0 0 点看着
    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
    {
        if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
        {
            //若是A键
            if ((ea.getKey() == 'A') || (ea.getKey() == 'a'))
            {
                _theta += 1.0;
            }
            if ((ea.getKey() == 'D') || (ea.getKey() == 'd'))
            {
                _theta -= 1.0;
            }

            _eye = osg::Vec3(_r * std::cos(osg::DegreesToRadians(_theta)), _r * std::sin(osg::DegreesToRadians(_theta)), 5.0);
        }
        return false;
    }

    //视点位置
    osg::Vec3d              _eye;
    //视点看向哪里
    osg::Vec3d              _center;
    //头顶的朝向
    osg::Vec3d              _up;

    //视点看向0 0 0的角度
    float              _theta;
    //视点离0 0 0的距离
    float              _r;
};

osg::Node* createScene()
{
    osg::Group* root = new osg::Group();
    root->addChild(osgDB::readNodeFile("axes.osgt"));

    osg::Geode* gnode = new osg::Geode;
    root->addChild(gnode);

    osg::Geometry* geom = new osg::Geometry;
    gnode->addChild(geom);

    osg::Vec3Array* vertex = new osg::Vec3Array;
    geom->setVertexArray(vertex);
    //沿xy平面画线，间隔0.5米，从-5，画到5
    for (float i = -5; i <= 5; i += 0.5)
    {
        vertex->push_back(osg::Vec3(i, -5, 0));
        vertex->push_back(osg::Vec3(i, 5, 0));
        vertex->push_back(osg::Vec3(-5, i, 0));
        vertex->push_back(osg::Vec3(5, i, 0));
    }
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertex->size()));

    osg::Vec4Array* color = new osg::Vec4Array();
    color->push_back(osg::Vec4(0.7, 0.7, 0.7, 1.0));
    geom->setColorArray(color, osg::Array::BIND_OVERALL);
    geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    return root;
}


int main()
{
    osgViewer::Viewer viewer;

    viewer.setCameraManipulator(new MyCameraManipulator());
    viewer.setSceneData(createScene());

    return viewer.run();
}
