#pragma once
#include <osgViewer/Viewer>

#include <osg/LineSegment>
#include <osg/Point>
#include <osg/Geometry>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>

#include <osgGA/CameraManipulator>

#include <osgUtil/IntersectVisitor>

class TravelManipulator : public osgGA::CameraManipulator
{
public:
	// 构造函数
	TravelManipulator(void){}

	// 析构函数
	virtual ~TravelManipulator(void){}

	// 把漫游加入到场景之中
	static TravelManipulator* TravelToScreen(osg::ref_ptr<osgViewer> viewer); 

private:
	osg::ref_ptr<osgViewer::Viewer> m_pHostViewer;

	// 移动速度
	float m_fMoveSpeed;

	osg::Vec3 m_vPosition;

	osg::Vec3 m_vRotation;

public:
	// 鼠标左键是否按下
	bool m_bLeftButtonDown;

	// 鼠标 X , Y
	float m_fpushX;
	float m_fpushY;

	
	// 设置矩阵
	virtual void setByMatrix(const osg::Matrixd& matrix) = 0;

	// 设置逆矩阵
	virtual void setByInverseMatrix(const osg::Matrixd& matrix) = 0;

	// 	得到矩阵
	virtual osg::Matrixd getMatrix() const = 0;

	//  得到逆矩阵
	virtual osg::Matrixd getInverseMatrix() const = 0;

	// 事件处理函数
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

	// 屏幕角度
	float m_fAngle;

	// 位置变换函数
	float ChangePosition(osg::Vec3& delta);

	// 碰撞检测是否开启
	bool m_bPeng;

	// 设置速度
	float getSpeed();

	void setSpeed(float &);

	// 设置起始位置
	void SetPositon(osg::Vec3 &position);

	osg::Vec3 GetPosition();

};

