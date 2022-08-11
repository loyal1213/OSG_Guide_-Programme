#include <osgViewer/viewer>
#include <osgDB/ReadFile>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgGA/CameraManipulator>
#include <osgGA/AnimationPathManipulator>


class TravelCameraManipulator : public osgGA::CameraManipulator
{
public:
    TravelCameraManipulator()
    {
        //初始的场景是个20x20的棋盘，中心点在[0,0],xy的坐标范围是从[-10,10]，z=0
        //设置_eye的出生点
        _eye = osg::Vec3(0.0, -8, 1.0);
        //这里很关键，_rotation=(0,0,0)的情况下视点会朝向哪里呢，这是个基准参考量，后面的旋转都是从
        //这里来，所以务必要弄清楚，000时的朝向是Z轴负方向，头顶向Y轴正方向，自然右手边就是X轴正方向
        //在简书的文章里有图，简书搜杨石兴，《osg3.6.5最短的一帧》等找找
        //我们要想让视角转着朝向前方，也即站在(0.0, -8, 1.0）看向(0,0,0)，则只需要看向Y轴
        //正方向就可以，则只需要x轴方向逆时针转90度，则出生就是朝向这里了
        //用户可以自己修改这个值感受一下
        _rotation = osg::Vec3(osg::inDegrees(90.0), 0.0, 0.0);
        
    }

    //这三个纯虚函数本例不会使用
    virtual void setByMatrix(const osg::Matrixd& matrix) {};
    virtual void setByInverseMatrix(const osg::Matrixd& matrix) {};
    virtual osg::Matrixd getMatrix() const { return osg::Matrix::identity(); };

    //最关键的是这个，这个返回的就是ViewMatrix
    virtual osg::Matrixd getInverseMatrix() const
    {
        return osg::Matrix::inverse(osg::Matrix::rotate(_rotation.x(), osg::X_AXIS, _rotation.y(), osg::Y_AXIS,
            _rotation.z(), osg::Z_AXIS) * osg::Matrix::translate(_eye));
    };

    //事件处理，我们要点击A就围着Z轴顺时针转动，点D就逆时针转动，转的时候始终朝0 0 0 点看着
    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
    {
        if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
        {
            //若是A键
            if ((ea.getKey() == 'q') || (ea.getKey() == 'Q'))//左转
            {
                _rotation.z() += osg::inDegrees(2.0);
                if (_rotation.z() > osg::inDegrees(180.0))
                {
                    _rotation.z() -= osg::inDegrees(360.0);
                }
            }
            if ((ea.getKey() == 'E') || (ea.getKey() == 'e'))//右转
            {
                _rotation.z() -= osg::inDegrees(2.0);
                if (_rotation.z() < osg::inDegrees(-180.0))
                {
                    _rotation.z() += osg::inDegrees(360.0);
                }
            }
            if ((ea.getKey() == 'w') || (ea.getKey() == 'W'))//前进
            {
                float stepSize = 0.5;
                float zRot = _rotation.z();//[-180, 180]
                //判断朝向以xy为平面的哪个象限,注意默认是朝各Y轴正方向的，时不时就得提一下
                //第一象限||第四象限
                if (((zRot >= -osg::inDegrees(90.0)) && (zRot <= -osg::inDegrees(0.0)))|| ((zRot <= -osg::inDegrees(90.0)) && (zRot >= osg::inDegrees(-180.0))))
                {
                    _eye.x() += stepSize * std::sin(-zRot);
                    _eye.y() += stepSize * std::cos(-zRot);
                }
                else //二三象限
                {
                    _eye.x() -= stepSize * std::sin(zRot);
                    _eye.y() += stepSize * std::cos(zRot);
                }           
            }
        }
        return false;
    }

    //视点位置
    osg::Vec3d              _eye;
    //视点朝向
    osg::Vec3d              _rotation;
};

osg::Node* createBase(const osg::Vec3 center, float radius)
{
    osg::Group* root = new osg::Group;

    int numTilesX = 10;
    int numTilesY = 10;

    float width = 2 * radius;
    float height = 2 * radius;

    osg::Vec3 v000(center - osg::Vec3(width * 0.5f, height * 0.5f, 0.0f));
    osg::Vec3 dx(osg::Vec3(width / ((float)numTilesX), 0.0, 0.0f));
    osg::Vec3 dy(osg::Vec3(0.0f, height / ((float)numTilesY), 0.0f));

    // fill in vertices for grid, note numTilesX+1 * numTilesY+1...
    osg::Vec3Array* coords = new osg::Vec3Array;
    int iy;
    for (iy = 0; iy <= numTilesY; ++iy)
    {
        for (int ix = 0; ix <= numTilesX; ++ix)
        {
            coords->push_back(v000 + dx * (float)ix + dy * (float)iy);
        }
    }

    //Just two colours - black and white.
    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)); // white
    colors->push_back(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f)); // black

    osg::ref_ptr<osg::DrawElementsUShort> whitePrimitives = new osg::DrawElementsUShort(GL_QUADS);
    osg::ref_ptr<osg::DrawElementsUShort> blackPrimitives = new osg::DrawElementsUShort(GL_QUADS);

    int numIndicesPerRow = numTilesX + 1;
    for (iy = 0; iy < numTilesY; ++iy)
    {
        for (int ix = 0; ix < numTilesX; ++ix)
        {
            osg::DrawElementsUShort* primitives = ((iy + ix) % 2 == 0) ? whitePrimitives.get() : blackPrimitives.get();
            primitives->push_back(ix + (iy + 1) * numIndicesPerRow);
            primitives->push_back(ix + iy * numIndicesPerRow);
            primitives->push_back((ix + 1) + iy * numIndicesPerRow);
            primitives->push_back((ix + 1) + (iy + 1) * numIndicesPerRow);
        }
    }

    // set up a single normal
    osg::Vec3Array* normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));

    osg::Geometry* geom = new osg::Geometry;
    geom->setVertexArray(coords);

    geom->setColorArray(colors, osg::Array::BIND_PER_PRIMITIVE_SET);

    geom->setNormalArray(normals, osg::Array::BIND_OVERALL);

    geom->addPrimitiveSet(whitePrimitives.get());
    geom->addPrimitiveSet(blackPrimitives.get());

    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(geom);
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    root->addChild(geode);

    root->addChild(osgDB::readNodeFile("axes.osgt"));

    return root;
}

int Test09()
{
    osgViewer::Viewer viewer;

    viewer.setCameraManipulator(new TravelCameraManipulator);
    viewer.setSceneData(createBase(osg::Vec3(0.0, 0.0, 0.0), 10.0));

    return viewer.run();
}
