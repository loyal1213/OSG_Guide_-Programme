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
        //��ʼ�ĳ����Ǹ�20x20�����̣����ĵ���[0,0],xy�����귶Χ�Ǵ�[-10,10]��z=0
        //����_eye�ĳ�����
        _eye = osg::Vec3(0.0, -8, 1.0);
        //����ܹؼ���_rotation=(0,0,0)��������ӵ�ᳯ�������أ����Ǹ���׼�ο������������ת���Ǵ�
        //���������������ҪŪ�����000ʱ�ĳ�����Z�Ḻ����ͷ����Y����������Ȼ���ֱ߾���X��������
        //�ڼ������������ͼ����������ʯ�ˣ���osg3.6.5��̵�һ֡��������
        //����Ҫ�����ӽ�ת�ų���ǰ����Ҳ��վ��(0.0, -8, 1.0������(0,0,0)����ֻ��Ҫ����Y��
        //������Ϳ��ԣ���ֻ��Ҫx�᷽����ʱ��ת90�ȣ���������ǳ���������
        //�û������Լ��޸����ֵ����һ��
        _rotation = osg::Vec3(osg::inDegrees(90.0), 0.0, 0.0);
        
    }

    //���������麯����������ʹ��
    virtual void setByMatrix(const osg::Matrixd& matrix) {};
    virtual void setByInverseMatrix(const osg::Matrixd& matrix) {};
    virtual osg::Matrixd getMatrix() const { return osg::Matrix::identity(); };

    //��ؼ����������������صľ���ViewMatrix
    virtual osg::Matrixd getInverseMatrix() const
    {
        return osg::Matrix::inverse(osg::Matrix::rotate(_rotation.x(), osg::X_AXIS, _rotation.y(), osg::Y_AXIS,
            _rotation.z(), osg::Z_AXIS) * osg::Matrix::translate(_eye));
    };

    //�¼���������Ҫ���A��Χ��Z��˳ʱ��ת������D����ʱ��ת����ת��ʱ��ʼ�ճ�0 0 0 �㿴��
    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
    {
        if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
        {
            //����A��
            if ((ea.getKey() == 'q') || (ea.getKey() == 'Q'))//��ת
            {
                _rotation.z() += osg::inDegrees(2.0);
                if (_rotation.z() > osg::inDegrees(180.0))
                {
                    _rotation.z() -= osg::inDegrees(360.0);
                }
            }
            if ((ea.getKey() == 'E') || (ea.getKey() == 'e'))//��ת
            {
                _rotation.z() -= osg::inDegrees(2.0);
                if (_rotation.z() < osg::inDegrees(-180.0))
                {
                    _rotation.z() += osg::inDegrees(360.0);
                }
            }
            if ((ea.getKey() == 'w') || (ea.getKey() == 'W'))//ǰ��
            {
                float stepSize = 0.5;
                float zRot = _rotation.z();//[-180, 180]
                //�жϳ�����xyΪƽ����ĸ�����,ע��Ĭ���ǳ���Y��������ģ�ʱ��ʱ�͵���һ��
                //��һ����||��������
                if (((zRot >= -osg::inDegrees(90.0)) && (zRot <= -osg::inDegrees(0.0)))|| ((zRot <= -osg::inDegrees(90.0)) && (zRot >= osg::inDegrees(-180.0))))
                {
                    _eye.x() += stepSize * std::sin(-zRot);
                    _eye.y() += stepSize * std::cos(-zRot);
                }
                else //��������
                {
                    _eye.x() -= stepSize * std::sin(zRot);
                    _eye.y() += stepSize * std::cos(zRot);
                }           
            }
        }
        return false;
    }

    //�ӵ�λ��
    osg::Vec3d              _eye;
    //�ӵ㳯��
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
