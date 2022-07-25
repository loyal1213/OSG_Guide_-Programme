#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/NodeVisitor>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

#include <fstream>
#include <iostream>

// 顶点访问器，继承自 osg::NodeVisitor 类
class VertexVisitor:public osg::NodeVisitor{

public:
	// 保存顶点数据
	osg::ref_ptr<osg::Vec3Array> extracted_verts_;

public:
	// 构造函数，初始化并设置为遍历所有子节点
	VertexVisitor():osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN){extracted_verts_  = new osg::Vec3Array();}

	// 重载 apply() 方法
	virtual void apply(osg::Geode& geode) override{
		// 得到每一个 drawable
		for (unsigned int i = 0; i< geode.getNumDrawables(); ++i){

			// 得到几何体
			osg::Geometry* geom = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
			if (!geom){
				std::cout<<"几何体错误!"<<std::endl;
				continue;
			}

			// 得到顶点数组
			osg::Vec3Array* verts = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
			if (!verts){
				std::cout<<"顶点数组错误!"<<std::endl;
				continue;
			}

			// 添加到extered_verts
			extracted_verts_->insert(extracted_verts_->end(),verts->begin(),verts->end());
		}

	};

};

void TestVertexNodeVisitor(){


	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("glider.osg");

	root->addChild(node.get());

	// 创建顶点访问器对象
	VertexVisitor vtea;

	node->accept(vtea);

	std::ofstream fout("glider.vertexs");

	int size_tt = vtea.extracted_verts_->size();

	std::vector<osg::Vec3>::iterator iter = vtea.extracted_verts_->begin();

	for (int i =0; i < size_tt; i++) {
		fout<<iter->x()<<"	"<<iter->y()<<"	"<<iter->z()<<std::endl;
		std::cout<<iter->x()<<"	"<<iter->y()<<"	"<<iter->z()<<std::endl;
		iter++;
	}

	
	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
}