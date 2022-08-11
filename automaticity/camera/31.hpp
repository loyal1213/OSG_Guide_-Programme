#include <osgViewer/Viewer>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PrimitiveSet>
#include <osgGA/CameraManipulator>


class MyCameraManipulator : public osgGA::CameraManipulator
{
public:
	MyCameraManipulator()
	{
		_theta = 0.0;
		_center = osg::Vec3(0.0, 0.0, 0.0);
		_up = osg::Vec3(0.0, 0.0, 1.0);
		_eye = osg::Vec3(0.0, -10.0, 0.0);

		_deltaEye = 0.3;

		_l = -2.0;
		_r = 2.0;
		_b = -2.0;
		_t = 2.0;
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
		if (ea.getEventType() == osgGA::GUIEventAdapter::FRAME)
		{
			us.asView()->getCamera()->setProjectionMatrixAsOrtho2D(_l, _r, _b, _t);
		}

		if (ea.getEventType() == osgGA::GUIEventAdapter::SCROLL)
		{
			//判断滚动的方向
			osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();

			if (sm == osgGA::GUIEventAdapter::SCROLL_DOWN)
			{
				_deltaEye *= 1.1;

				_l *= 1.1;
				_t *= 1.1;
				_b *= 1.1;
				_r *= 1.1;

			}
			else
			{
				//范围变小0.1，正好与变大相反，左右同时往里缩
				_deltaEye *= 0.9;

				_l *= 0.9;
				_t *= 0.9;
				_b *= 0.9;
				_r *= 0.9;
			}
		}


		if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
		{

			//旋转视角
			if ((ea.getKey() == 'Q') || (ea.getKey() == 'q'))
			{
				//点q变1度
				_theta += 1;

				_up = osg::Vec3(0.0, 0.0, 1.0)*osg::Matrix::rotate(osg::inDegrees(_theta), osg::Y_AXIS);
			}

			//若是A键
			if ((ea.getKey() == 'A') || (ea.getKey() == 'a'))
			{

				//_eye += osg::Vec3(-_deltaEye, 0.0, 0.0);
				_eye += osg::Vec3(-_deltaEye*std::cos(osg::inDegrees(_theta)), 0.0, _deltaEye*std::sin(osg::inDegrees(_theta)));


				_center = osg::Vec3(_eye.x(), 0.0, _eye.z());
			}
			if ((ea.getKey() == 'D') || (ea.getKey() == 'd'))
			{
				//_eye += osg::Vec3(_deltaEye, 0.0, 0.0);

				_eye -= osg::Vec3(-_deltaEye*std::cos(osg::inDegrees(_theta)), 0.0, _deltaEye*std::sin(osg::inDegrees(_theta)));

				_center = osg::Vec3(_eye.x(), 0.0, _eye.z());
			}

		}
		return false;
	}

	//视点位置
	osg::Vec3d              _eye;

	//点击鼠标a键向左移的量度，随着鼠标滚轮的放大缩小，这个量度也在变化
	double _deltaEye; 


	//视点看向哪里
	osg::Vec3d              _center;
	//头顶的朝向
	osg::Vec3d              _up;

	//视点看向0 0 0的角度
	float              _theta;


	//二维投影参数, left, right, bottom, top
	double _l, _r, _b, _t;
};


osg::Geode* createNet()
{
	osg::Geode* gnode = new osg::Geode;

	osg::Geometry* geom = new osg::Geometry;
	gnode->addDrawable(geom);

	//设置线的颜色为白色
	osg::Vec4Array* color = new osg::Vec4Array;
	color->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	geom->setColorArray(color, osg::Array::BIND_OVERALL);

	osg::Vec3Array* vertex = new osg::Vec3Array;
	geom->setVertexArray(vertex);

	//间隔1米，横20条，竖20条
	for (int i = 0; i < 20; i++)
	{
		//x方向
		vertex->push_back(osg::Vec3(0, 0, i));
		vertex->push_back(osg::Vec3(17, 0, i)); //设置为17，不画满

		//z方向
		vertex->push_back(osg::Vec3(i, 0, 0));
		vertex->push_back(osg::Vec3(i, 0, 17));
	}

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertex->size()));
	geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	return gnode;
}


int Test31()
{
	osgViewer::Viewer viewer;
	viewer.setSceneData(createNet());
	viewer.setCameraManipulator(new MyCameraManipulator());
	return viewer.run();
}