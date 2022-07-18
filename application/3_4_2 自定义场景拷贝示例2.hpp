#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geometry>
#include <osg/Image>

#include <osg/Geode>
#include <osg/Group>
#include <osg/Notify>
#include <osg/Texture>
#include <osg/StateSet>
#include <osg/StateAttribute>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/ReaderWriter>

#include <osgUtil/Optimizer>

#include <iostream>

// �Զ��� Copy �࣬�������������Ϣ
class GraphCopyOp:public osg::CopyOp
{
public:
	inline GraphCopyOp(CopyFlags flags):osg::CopyOp(flags){}
	~GraphCopyOp();

public:
	virtual osg::Referenced*     operator() (const osg::Referenced* ref) const{};
	virtual osg::Object*         operator() (const osg::Object* obj) const{};
	virtual osg::Node*           operator() (const osg::Node* node) const{
		// �ж��Ƿ�Ϊ���
		if (node && _flags && DEEP_COPY_ALL){
			// �жϽڵ�ĸ��ڵ��Ƿ��ж�������ֻ��һ������ֱ�����
			if (node->getNumParents() > 1){
				// �ж��Ƿ��Ѿ�����
				if (_nodeCopyMap.find(node) != _nodeCopyMap.end()){
					std::cout<<"Copy of node "<<node<<", "<<node->getName()<<", "
						<<_nodeCopyMap[node]<<", will be reuserd"<<std::endl;
					// ֱ�ӷ��ؿ�������ĵ�ַ
					return (osg::Node*)(_nodeCopyMap[node]);
				}else{
					// ���п�������ӳ����б��濽������
					osg::Node* newNode = dynamic_cast<osg::Node*>(node->clone(*this));
					_nodeCopyMap[node] = newNode;
					// ���ص�ַ
					return newNode;
				}
			}else{
				// ֱ�ӿ���
				return dynamic_cast<osg::Node*>(node->clone(*this));
			}
		}else{
			// ֱ�ӷ��ص�ַ
			return const_cast<osg::Node*>(node);
		}

	};
	virtual osg::Drawable*       operator() (const osg::Drawable* drawable) const{};
	virtual osg::StateSet*       operator() (const osg::StateSet* stateset) const{};
	virtual osg::StateAttribute* operator() (const osg::StateAttribute* attr) const{};
	virtual osg::Texture*        operator() (const osg::Texture* text) const{};
	virtual osg::Image*          operator() (const osg::Image* image) const{};
	virtual osg::Array*          operator() (const osg::Array* array) const{};
	virtual osg::PrimitiveSet*   operator() (const osg::PrimitiveSet* primitives) const{};
	virtual osg::Shape*          operator() (const osg::Shape* shape) const{};
	virtual osg::Uniform*        operator() (const osg::Uniform* shape) const{};
	virtual osg::Callback*       operator() (const osg::Callback* nodecallback) const{};
	virtual osg::StateAttributeCallback* operator() (const osg::StateAttributeCallback* stateattributecallback) const{};
	virtual osg::UniformCallback*        operator() (const osg::UniformCallback* uniformcallback) const{};

protected:
	// �ڵ�ӳ����������� child �� ����, �ֱ���Ϊ ԭ�� �� ��
	mutable std::map<const osg::Node* , osg::Node *> _nodeCopyMap;
private:
};

void TestCopyOp2(){

	// ���� viewer ���� ���������
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	// ����һ������  ������ڵ㹲��һ���ӽڵ� 
	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("glider.osg");

	// �ӽڵ� pat, ���� node
	osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
	pat->setPosition(osg::Vec3(5.0f,0.0f,0.0f));
	pat->addChild(node.get());

	// �ӽڵ� mat, ���� node
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
	osg::Matrix m;
	m.makeScale(2.0f,2.0f,2.0f);
	mt->setMatrix(m);
	mt->addChild(node.get());

	// ����ӽڵ�
	root->addChild(pat.get());
	root->addChild(mt.get());
	
	// һ�� �ุ�ڵ��һ���ӽڵ㳡�� ��Ĭ�����
	osg::ref_ptr<osg::Node> deepCopy = dynamic_cast<osg::Node*>(node->clone(osg::CopyOp::DEEP_COPY_ALL));
	std::cout<<std::endl<<"���һ�� �ุ�ڵ�һ���ӽڵ� �ĳ�����Ĭ�����"<<std::endl<<std::endl;
	osgDB::writeNodeFile((*deepCopy.get()),"deepCopy.osg");


	// ���һ��  �ุ�ڵ��һ���ӽڵ� ������ǳ����
	osg::ref_ptr<osg::Node> myShallowCopy = dynamic_cast<osg::Node*>(node->clone(GraphCopyOp::Options(osg::CopyOp::SHALLOW_COPY)));
	std::cout<<std::endl<<"���һ�� �ุ�ڵ��һ���ӽڵ� ������ǳ����"<<std::endl<<std::endl;
	osgDB::writeNodeFile((*myShallowCopy.get()),"myShallowCopy.osg");

	// ���һ��  �ุ�ڵ��һ���ӽڵ� ���������
	osg::ref_ptr<osg::Node> myDeepCopy = dynamic_cast<osg::Node*>(node->clone(GraphCopyOp::Options(osg::CopyOp::DEEP_COPY_ALL)));
	std::cout<<std::endl<<"���һ�� �ุ�ڵ��һ���ӽڵ� ���������"<<std::endl<<std::endl;
	osgDB::writeNodeFile((*myDeepCopy.get()),"myDeepCopy.osg");

	// �Ż�����
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();

}