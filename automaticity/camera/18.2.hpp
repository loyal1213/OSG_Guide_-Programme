#include <osgViewer/Viewer>
#include <osgEarth/EarthManipulator>
#include <osgDB/ReadFile>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/LineWidth>
#include <osg/LineStipple>

//全局椭球体，用于经纬度坐标与XYZ坐标互相转换
osg::EllipsoidModel* _em = new osg::EllipsoidModel;

//起点经纬度，终点经纬度，中间最高点，来创建一个简单的曲线
osg::Group* BuildScene(osg::Vec3 fromLLH, osg::Vec3 toLLH, float topH)
{
    osg::Group* sceneRoot = new osg::Group;
    
    //给线前面加一个mt是为了防止大坐标抖动
    osg::MatrixTransform* mtLine = new osg::MatrixTransform;
    sceneRoot->addChild(mtLine);

    osg::Geode* line = new osg::Geode;
    mtLine->addChild(line);

    osg::Geometry* lineGeom = new osg::Geometry;
    line->addDrawable(lineGeom);

    //线的宽度设置成5
    osg::LineWidth* lw = new osg::LineWidth(3.0);
    lineGeom->getOrCreateStateSet()->setAttributeAndModes(lw, osg::StateAttribute::ON);
    lineGeom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    //设置成点画线
    osg::LineStipple* ls = new osg::LineStipple(1, 0x00FF);
    lineGeom->getOrCreateStateSet()->setAttributeAndModes(ls, osg::StateAttribute::ON);

    osg::Vec3Array* vertex = new osg::Vec3Array;
    lineGeom->setVertexArray(vertex);

    //线的颜色设置成高级灰
    osg::Vec4Array* color = new osg::Vec4Array;
    color->push_back(osg::Vec4(0.8, 0.8, 0.8, 1.0));
    lineGeom->setColorArray(color, osg::Array::BIND_OVERALL);

    //从起点到终点的经纬度，采用简单插值算法，插上100个点
    float deltaLat = (toLLH.x() - fromLLH.x()) / 100;
    float deltaLon = (toLLH.y() - fromLLH.y()) / 100;
    //高度变化前半程
    float deltaHF = (topH - fromLLH.z()) / 50;
    //高度变化后半程
    float deltaHT = (toLLH.z() - topH) / 50;

    //防止大坐标抖动，将所有顶点的值都减动一个fromLLH
    osg::Vec3d fromV;
    _em->convertLatLongHeightToXYZ(osg::inDegrees(fromLLH.x()), osg::inDegrees(fromLLH.y()), fromLLH.z(), fromV.x(), fromV.y(), fromV.z());
    mtLine->setMatrix(osg::Matrix::translate(fromV));

    for (int i = 0; i < 100; i++)
    {
        
        osg::Vec3 tempPoint = fromLLH + osg::Vec3(deltaLat*i, deltaLon*i, deltaHF*i);
        if (i > 49)//到中间了要往下了
        {
            tempPoint.z() = topH + deltaHT*(i-49);
        }

        osg::Vec3d tempV;
        _em->convertLatLongHeightToXYZ(osg::inDegrees(tempPoint.x()), osg::inDegrees(tempPoint.y()), tempPoint.z(), tempV.x(), tempV.y(), tempV.z());
        tempV -= fromV; //防止大坐标抖动

        vertex->push_back(tempV);
    }

    lineGeom->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, vertex->size()));

    return sceneRoot;
}


int Test18_2(int argc, char** argv)
{
    osgEarth::initialize();
    osgViewer::Viewer viewer;
    viewer.setCameraManipulator(new osgEarth::Util::EarthManipulator);

    osg::Group* root = new osg::Group;
    root->addChild(BuildScene(osg::Vec3(25.04, 121.50, 100), osg::Vec3(33.22, 131.66, 100), 100000));
    root->addChild(osgDB::readNodeFile("simple.earth"));

    viewer.setSceneData(root);
    return viewer.run();
}