#include <osgViewer/viewer>
#include <osgDB/ReadFile>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgUtil/IntersectionVisitor>

//定义全局变量用着方便
osg::Group* _root = nullptr;
osg::Node* _box = nullptr;
osg::Group* _shiyi = nullptr;

osg::Geode* CreateBox(osg::Vec3 from, osg::Vec3 to, float stepSize)
{
    osg::Geode* gnode = new osg::Geode;
    gnode->addDrawable(new osg::ShapeDrawable(new osg::Box((from+to)/2, stepSize, stepSize, std::abs(to.z()-from.z()))));
    return gnode;
}

//画一个盒子
osg::Node* CreateScene()
{
    osg::Group* root = new osg::Group;
    _root = root;

    osg::MatrixTransform* mt = new osg::MatrixTransform;
    _box = mt;
    root->addChild(mt);
    mt->setMatrix(osg::Matrix::rotate(osg::inDegrees(30.0), osg::Vec3(1.0, 1.0, 1.0)));

    osg::Geode* gnode = new osg::Geode;
    osg::ShapeDrawable* sd = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0, 0.0, 0.0), 10, 8, 7));
    gnode->addDrawable(sd);

    mt->addChild(gnode);

    return root;
}

class MyEventHandler : public osgGA::GUIEventHandler
{
public:
    MyEventHandler()
    {
        _jiange = 0.5;
    }

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
    {
        if (ea.getEventType() == ea.KEYDOWN)
        {
            bool _push = false;
            if ((ea.getKey() == 'w') || (ea.getKey() == 'W'))
            {
                _push = true;
                _jiange /= 1.5;
            }
            
            if ((ea.getKey() == 's') || (ea.getKey() == 'S'))
            {
                _push = true;
                _jiange *= 1.5;
            }

            if (_push)
            {
                //清空上一次采样结果
                if (_shiyi != nullptr)
                {
                    _root->removeChild(_shiyi);
                    _shiyi = nullptr;
                }

                _shiyi = new osg::Group();
                _root->addChild(_shiyi);

                std::cout << "采样间隔："<<_jiange << std::endl;
                //求要求的场景的boudingbox
                osg::BoundingSphere bs = _box->getBound();
                //求出xyz的最小值和最大值，然后根据间隔来求
                osg::Vec3 center = bs.center();
                float r = bs.radius();
                float areas = 0.0;
                for (float fromx = center.x() - r; fromx <= center.x() + r; fromx += _jiange)
                {
                    for (float fromy = center.y() - r; fromy <= center.y() + r; fromy += _jiange)
                    {
                        osg::Vec3 from = osg::Vec3(fromx, fromy, center.z() - r);
                        osg::Vec3 to = osg::Vec3(fromx, fromy, center.z() + r);
                        osgUtil::LineSegmentIntersector::Intersections intersections;
                        osg::ref_ptr<osgUtil::LineSegmentIntersector> ls = new osgUtil::LineSegmentIntersector(from, to);
                        osg::ref_ptr<osgUtil::IntersectionVisitor> iv = new osgUtil::IntersectionVisitor(ls);
                        _box->accept(*iv.get());

                        if (ls->containsIntersections())
                        {
                            intersections = ls->getIntersections();
                            //判断当前交点数
                            if (intersections.size() == 2)
                            {
                                osgUtil::LineSegmentIntersector::Intersections::iterator iter = intersections.begin();
                                osg::Vec3 firstInter = iter->getWorldIntersectPoint();
                                iter++;
                                osg::Vec3 secondInter = iter->getWorldIntersectPoint();
                                _shiyi->addChild(CreateBox(firstInter, secondInter, _jiange));
                                areas += (_jiange*_jiange*std::abs(firstInter.z() - secondInter.z()));
                            }
                        }
                    }
                }

                std::cout << "实际面积：560 计算面积：" << areas << std::endl;
            }
        }

        return false;
    }

    float _jiange;
};


int Test12()
{
    osgViewer::Viewer viewer;

    viewer.addEventHandler(new MyEventHandler());

    viewer.setSceneData(CreateScene());

    return viewer.run();
}
