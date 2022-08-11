#include <osgViewer/viewer>
#include <osgDB/ReadFile>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgGA/CameraManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osg/ShapeDrawable>

//定义全局变量用着方便
osgGA::AnimationPathManipulator* _apm = new osgGA::AnimationPathManipulator;
osg::Group* _root = nullptr;
osg::Node* _base = nullptr;

osg::AnimationPath* createAnimationByTwoPoints(osg::Vec3 from, osg::Quat fromRotate, osg::Vec3 to)
{
    //防止视点贴地面太近
    if (from.z() < 1.0) from.z() = 1.0;
    if (to.z() < 1.0) to.z() = 1.0;

    osg::AnimationPath* animationPath = new osg::AnimationPath;
    animationPath->setLoopMode(osg::AnimationPath::NO_LOOPING);
    animationPath->insert(0, osg::AnimationPath::ControlPoint(from, fromRotate));
    osg::Quat rotate1(osg::inDegrees(90.0), osg::X_AXIS);

    osg::Vec3 tf = to - from;
    //经X旋转90度以后，视点已经朝向y轴正方向
    //当x,y都大于0在第一四象限，要顺时针旋转theta
    float theta = 0;

    if (tf.x() > 0.000001)//一四象限
    {
        rotate1 *= osg::Quat(std::atan(tf.y() / tf.x()) - osg::inDegrees(90.0), osg::Z_AXIS);
    }
    else if (tf.x() < -0.000001)//二三象限
    {
        rotate1 *= osg::Quat(osg::inDegrees(90.0) + std::atan(tf.y() / tf.x()), osg::Z_AXIS);
    }
    else //x=0
    {
        if (tf.y() > 0)
        {
            //啥也不用做，默认朝y轴正方向
        }
        else
        {
            //转180，朝y轴负方向
            rotate1 *= osg::Quat(osg::PI, osg::Z_AXIS);
        }
    }

    if (fromRotate != rotate1)//代表不需要给时间转方向，当起点的方向和终点的方向是一致的时候，就不需要转方向
    {
        //给2秒钟转方向，起点不变，只有方向变
        animationPath->insert(2, osg::AnimationPath::ControlPoint(from, rotate1));
    }

    animationPath->insert(5, osg::AnimationPath::ControlPoint(to, rotate1));
    return animationPath;
}

//画一个小球
osg::Geode* CreateSphere(osg::Vec3 center)
{
    osg::Geode* gnode = new osg::Geode;
    osg::ShapeDrawable* sd = new osg::ShapeDrawable(new osg::Sphere(center, 0.1));
    gnode->addDrawable(sd);
    return gnode;
}

class MyEventHandler : public osgGA::GUIEventHandler
{
public:
    MyEventHandler() {}

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
    {
        //双击则加个球在击处，然后寻路过去
        if (ea.getEventType() == ea.DOUBLECLICK)
        {
            if (ea.getButton() == ea.LEFT_MOUSE_BUTTON)
            {
                //与地板求交
                osgUtil::LineSegmentIntersector::Intersections intersections;
                osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
                if (view->computeIntersections(ea, intersections))
                {
                    //有交点，先画个球
                    osg::Vec3 hitPoint = intersections.begin()->getWorldIntersectPoint();
                    _root->addChild(CreateSphere(hitPoint));

                    //操作器切换成animationPath操作器，创建animationPath直接跑过去
                    //获取当前视点信息
                    osg::Vec3 eye, center, up;
                    view->getCamera()->getViewMatrixAsLookAt(eye, center, up);

                    //得到当前视点的朝向
                    osg::Matrix viewMatrix = view->getCamera()->getInverseViewMatrix();
                    _apm->setAnimationPath(createAnimationByTwoPoints(eye, viewMatrix.getRotate(),hitPoint));
                    //减去当前仿真时间重头开始播,停个timeoffset设置的不太合理
                    //按说设置成0就应该是从头播了，但是其实现加了当前仿真时间，有点怪怪的
                    _apm->setTimeOffset(-ea.getTime());
                }
            }
        }

        return false;
    }
};

osg::Node* createBase(const osg::Vec3 center, float radius)
{
    osg::Group* root = new osg::Group;
    _root = root;

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
    _base = geode;

    geode->addDrawable(geom);
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    root->addChild(geode);

    root->addChild(osgDB::readNodeFile("axes.osgt"));

    return root;
}

int Test10()
{
    osgViewer::Viewer viewer;

    viewer.addEventHandler(new MyEventHandler());

    osg::Quat rotate0(osg::inDegrees(90.0), osg::X_AXIS);
    _apm->setAnimationPath(createAnimationByTwoPoints(osg::Vec3(0.0, -18, 1.0), rotate0, osg::Vec3(0.0, -8, 1.0)));

    viewer.setCameraManipulator(_apm);
    viewer.setSceneData(createBase(osg::Vec3(0.0, 0.0, 0.0), 20.0));

    return viewer.run();
}
