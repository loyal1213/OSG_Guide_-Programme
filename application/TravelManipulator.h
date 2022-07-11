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
	// ���캯��
	TravelManipulator(void){}

	// ��������
	virtual ~TravelManipulator(void){}

	// �����μ��뵽����֮��
	static TravelManipulator* TravelToScreen(osg::ref_ptr<osgViewer> viewer); 

private:
	osg::ref_ptr<osgViewer::Viewer> m_pHostViewer;

	// �ƶ��ٶ�
	float m_fMoveSpeed;

	osg::Vec3 m_vPosition;

	osg::Vec3 m_vRotation;

public:
	// �������Ƿ���
	bool m_bLeftButtonDown;

	// ��� X , Y
	float m_fpushX;
	float m_fpushY;

	
	// ���þ���
	virtual void setByMatrix(const osg::Matrixd& matrix) = 0;

	// ���������
	virtual void setByInverseMatrix(const osg::Matrixd& matrix) = 0;

	// 	�õ�����
	virtual osg::Matrixd getMatrix() const = 0;

	//  �õ������
	virtual osg::Matrixd getInverseMatrix() const = 0;

	// �¼�������
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

	// ��Ļ�Ƕ�
	float m_fAngle;

	// λ�ñ任����
	float ChangePosition(osg::Vec3& delta);

	// ��ײ����Ƿ���
	bool m_bPeng;

	// �����ٶ�
	float getSpeed();

	void setSpeed(float &);

	// ������ʼλ��
	void SetPositon(osg::Vec3 &position);

	osg::Vec3 GetPosition();

};

