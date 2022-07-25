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


// 节点纹理访问器
class TextureVisitor:public osg::NodeVisitor{

public:
	// 构造函数
	TextureVisitor();
	osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN){}

	// 重载 apply() 方法
	virtual void apply(osg::Node& node){
		if (node.getStateSet()){
			apply(node.getStateSet());
		}
	// 实现继续遍历节点



	}



};