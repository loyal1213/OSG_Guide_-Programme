#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/NodeVisitor>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

#include <iostream>
#include <string>
#include <map>


// �ڵ����������
class TextureVisitor:public osg::NodeVisitor{

public:
	// ���캯��
	TextureVisitor();
	osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN){}

	// ���� apply() ����
	virtual void apply(osg::Node& node){
		if (node.getStateSet()){
			apply(node.getStateSet());
		}
	// ʵ�ּ��������ڵ�



	}



};