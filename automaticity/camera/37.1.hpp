#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/Camera>
#include <osgGA/GUIEventHandler>

//结点的掩码，显示与隐藏
#define NODE_SHOW ~0x0
#define NODE_HIDE 0x0

//选择框做为一个全局变量，使用起来方便
osg::Geometry* g_geomSelectBox = new osg::Geometry;

//
osg::Camera* createHUD(osg::Viewport* vp)
{
    osg::Camera* camera = new osg::Camera;

    //设置投影矩阵为正交投影
    camera->setProjectionMatrix(osg::Matrix::ortho2D(0, vp->width(), 0, vp->height()));

    //设置其观察矩阵为单位矩阵，且不改变，该相机永远显示，也不用参与拣选
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setViewMatrix(osg::Matrix::identity());

    //只清空深度缓存，使得其显示内容可以以主相机为背景
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);

    //最后渲染，因为需要以主相机显示的内容为背景
    camera->setRenderOrder(osg::Camera::POST_RENDER);

    //不需要响应事件
    camera->setAllowEventFocus(false);

    //绘制选择框
    osg::Geode* gnode = new osg::Geode;
    camera->addChild(gnode);

    gnode->addDrawable(g_geomSelectBox);
    //设置透明
    osg::StateSet* ss = gnode->getOrCreateStateSet();
    ss->setMode(GL_BLEND, osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);

    //设置顶点
    osg::Vec3Array* vertices = new osg::Vec3Array;
    float depth = - 0.1;
    vertices->push_back(osg::Vec3(0, 0, depth));
    vertices->push_back(osg::Vec3(100, 0, depth));
    vertices->push_back(osg::Vec3(100, 100, depth));
    vertices->push_back(osg::Vec3(0, 100, depth));
    g_geomSelectBox->setVertexArray(vertices);

    //设置颜色
    osg::Vec4Array* color = new osg::Vec4Array;
    color->push_back(osg::Vec4(0.8, 0.8, 0.8, 0.2));
    g_geomSelectBox->setColorArray(color, osg::Array::BIND_OVERALL);

    //绘制盒子
    g_geomSelectBox->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

    return camera;
}

class MyEvent : public osgGA::GUIEventHandler
{
public:
    MyEvent() :osgGA::GUIEventHandler(),
        _xStart(0),
        _yStart(0)
    {
    }

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
    {
        //左ctrl按着，又鼠标点击，则进入绘制状态
        if (ea.getEventType() == ea.PUSH) //
        {
            //判断左CTRL键是否按下
            if (ea.getModKeyMask() == ea.MODKEY_LEFT_CTRL)
            {
                _xStart = ea.getX();
                _yStart = ea.getY();
                //清空之前绘制的结果，这里仅隐藏即可
                g_geomSelectBox->setNodeMask(NODE_HIDE);

                //返回真代表后续事件处理器包括操作器不再处理该事件，就实现了拖动时场景不动
                return true;
            }
        }


        //左ctrl点下时，进入到选择状态，开始绘制选择框，且操作器不再处理鼠标拖动事件
        if (ea.getEventType() == ea.DRAG) //鼠标拖动，拖动是鼠标按键按下的时候移动鼠标
        {
            //判断左CTRL键是否按下
            if (ea.getModKeyMask() == ea.MODKEY_LEFT_CTRL)
            {
                //开始绘制，调整顶点参数就可以
                g_geomSelectBox->setNodeMask(NODE_SHOW);

                //获取顶点、更新顶点
                osg::Vec3Array* vertices = (osg::Vec3Array*)g_geomSelectBox->getVertexArray();
                int xEnd = ea.getX();
                int yEnd = ea.getY();
                float depth = -0.1;
                vertices->at(0).set(_xStart, _yStart, depth);
                vertices->at(1).set(xEnd, _yStart, depth);
                vertices->at(2).set(xEnd, yEnd, depth);
                vertices->at(3).set(_xStart, yEnd, depth);

                //重绘
                g_geomSelectBox->dirtyDisplayList();

                //返回真代表后续事件处理器包括操作器不再处理该事件，就实现了拖动时场景不动
                return true;
            }
        }

        return false;
    }

    int _xStart, _yStart;
};


int Test37_1()
{
    osgViewer::Viewer viewer;
   
    osg::Group* root = new osg::Group;

    root->addChild(osgDB::readNodeFile("cow.osg"));

    viewer.setSceneData(root);
    viewer.realize();

    //realize之后，上下文已经被初始化，可以获得视口大小
    //在此处获得视口大小是为了创建HUD时使其视口大小与创建的一致
    osg::Viewport* vp = viewer.getCamera()->getViewport();
    root->addChild(createHUD(vp));

    viewer.addEventHandler(new MyEvent);

    return viewer.run();
}