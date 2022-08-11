#include <osgViewer/Viewer>
#include <osgEarth/Notify>
#include <osgEarthUtil/EarthManipulator>
#include <osgDB/ReadFile>
#include <osgViewer/CompositeViewer>
#include <osg/Texture2D>

using namespace osgEarth;
using namespace osgEarth::Util;

osg::ref_ptr<osg::Texture2D> t2d = new osg::Texture2D;
int screenWidth = 1280;
int screenHeight = 1024;

static const char* vs = {
    "#version 120\n"
    "varying vec2 vTexCoord;\n"
    "void main(void)\n"
    "{\n"
    "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "	 vTexCoord = gl_MultiTexCoord0.xy;\n"
    "}\n"
};


/*
 1.浮点算法:Gray=R*0.3+G*0.59+B*0.11
 2.整数方法:Gray=(R*30+G*59+B*11)/100
 3.移位方法:Gray=(R*76+G*151+B*28)>>8;
 4.平均值法:Gray=(R+G+B)/3;
 5.仅取绿色:Gray=G;
 */

static const char* fs = {
    "#version 120\n"
    "uniform sampler2D rtt;\n"
    "varying vec2 vTexCoord;\n"
    "void main(void)\n"
    "{\n"
    "vec3 W = vec3(0.2125, 0.7154, 0.0721);\n"
    "vec4 mask = texture2D(rtt, vTexCoord);\n"
    "float luminance = dot(mask.rgb, W);\n"
    "gl_FragColor = vec4(vec3(luminance), 1.0);\n"
    "}\n"
};

osg::Camera* createHUD()
{
    osg::Camera* camera = new osg::Camera;

    camera->setProjectionMatrix(osg::Matrix::ortho2D(0, screenWidth, 0, screenHeight));
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setViewMatrix(osg::Matrix::identity());
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);
    camera->setRenderOrder(osg::Camera::POST_RENDER);
    camera->setAllowEventFocus(false);

    osg::Geode* geode = new osg::Geode();
    camera->addChild(geode);
    osg::Geometry* geom = new osg::Geometry;
    geode->addDrawable(geom);

    osg::Vec3Array* vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(0, 0, -0.1));
    vertices->push_back(osg::Vec3(screenWidth, 0, -0.1));
    vertices->push_back(osg::Vec3(screenWidth, screenHeight, -0.1));
    vertices->push_back(osg::Vec3(0, screenHeight, -0.1));
    geom->setVertexArray(vertices);

    osg::Vec2Array* coords = new osg::Vec2Array;
    coords->push_back(osg::Vec2(0.0, 0.0));
    coords->push_back(osg::Vec2(1.0, 0.0));
    coords->push_back(osg::Vec2(1.0, 1.0));
    coords->push_back(osg::Vec2(0.0, 1.0));
    geom->setTexCoordArray(0, coords);


    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0f, 1.0, 0.8f, 0.2f));
    geom->setColorArray(colors, osg::Array::BIND_OVERALL);
    geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

    geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF || osg::StateAttribute::PROTECTED);
    geom->getOrCreateStateSet()->setTextureAttributeAndModes(0, t2d, osg::StateAttribute::ON);

    //写个简单shader来对图像变一下色
    osg::Program* program = new osg::Program;
    program->addShader(new osg::Shader(osg::Shader::VERTEX, vs));
    program->addShader(new osg::Shader(osg::Shader::FRAGMENT, fs));
    geom->getOrCreateStateSet()->setAttributeAndModes(program, osg::StateAttribute::ON);
    geom->getOrCreateStateSet()->addUniform(new osg::Uniform("rtt", 0));
    return camera;
}

int Test13()
{
    osgViewer::CompositeViewer viewer;

    osgViewer::View* view = new osgViewer::View;
    view->setName("Single view");
    view->setUpViewOnSingleScreen(0);
    view->setSceneData(osgDB::readNodeFile("E:/the_way_code/osgEarthX/environment/tests/gdal_tiff.earth"));
    view->setCameraManipulator(new EarthManipulator());
    viewer.addView(view);
    t2d->setInternalFormat(GL_RGBA);
    view->getCamera()->attach(osg::Camera::COLOR_BUFFER, t2d);

    //此时view里已经有个主相机了，加个HUD给它
    osgViewer::Viewer::Windows windows;
    viewer.getWindows(windows);
    if (windows.empty()) return 1;

    osg::Camera* hudCamera = createHUD();
    hudCamera->setGraphicsContext(windows[0]);
    hudCamera->setViewport(0, 0, windows[0]->getTraits()->width, windows[0]->getTraits()->height);

    view->addSlave(hudCamera, false);

    return viewer.run();
}