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

// ������������̳��� osg::NodeVisitor ��
class VertexVisitor:public osg::NodeVisitor{

public:
	// ���涥������
	osg::ref_ptr<osg::Vec3Array> extracted_verts_;

public:
	// ���캯������ʼ��������Ϊ���������ӽڵ�
	VertexVisitor():osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN){extracted_verts_  = new osg::Vec3Array();}

	// ���� apply() ����
	virtual void apply(osg::Geode& geode) override{
		// �õ�ÿһ�� drawable
		for (unsigned int i = 0; i< geode.getNumDrawables(); ++i){

			// �õ�������
			osg::Geometry* geom = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
			if (!geom){
				std::cout<<"���������!"<<std::endl;
				continue;
			}

			// �õ���������
			osg::Vec3Array* verts = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
			if (!verts){
				std::cout<<"�����������!"<<std::endl;
				continue;
			}

			// ��ӵ�extered_verts
			extracted_verts_->insert(extracted_verts_->end(),verts->begin(),verts->end());
		}

	};

};

void TestVertexNodeVisitor(){


	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("glider.osg");

	root->addChild(node.get());

	// �����������������
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

	
	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();
}