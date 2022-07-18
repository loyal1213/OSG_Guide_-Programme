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

// 自定义 Copy 类，用于输出拷贝信息
class GraphCopyOp:public osg::CopyOp
{
public:
	inline GraphCopyOp(CopyFlags flags):osg::CopyOp(flags){}
	~GraphCopyOp();

public:
	virtual osg::Referenced*     operator() (const osg::Referenced* ref) const{};
	virtual osg::Object*         operator() (const osg::Object* obj) const{};
	virtual osg::Node*           operator() (const osg::Node* node) const{
		// 判断是否为深拷贝
		if (node && _flags && DEEP_COPY_ALL){
			// 判断节点的父节点是否有多个，如果只有一个，则直接深拷贝
			if (node->getNumParents() > 1){
				// 判断是否已经拷贝
				if (_nodeCopyMap.find(node) != _nodeCopyMap.end()){
					std::cout<<"Copy of node "<<node<<", "<<node->getName()<<", "
						<<_nodeCopyMap[node]<<", will be reuserd"<<std::endl;
					// 直接返回拷贝对象的地址
					return (osg::Node*)(_nodeCopyMap[node]);
				}else{
					// 进行拷贝并在映射表中保存拷贝对象
					osg::Node* newNode = dynamic_cast<osg::Node*>(node->clone(*this));
					_nodeCopyMap[node] = newNode;
					// 返回地址
					return newNode;
				}
			}else{
				// 直接拷贝
				return dynamic_cast<osg::Node*>(node->clone(*this));
			}
		}else{
			// 直接返回地址
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
	// 节点映射表，用来保存 child 和 拷贝, 分别作为 原像 和 像
	mutable std::map<const osg::Node* , osg::Node *> _nodeCopyMap;
private:
};

void TestCopyOp2(){

	// 创建 viewer 对象， 场景浏览器
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	// 创建一个场景  多个父节点共享一个子节点 
	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("glider.osg");

	// 子节点 pat, 共享 node
	osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
	pat->setPosition(osg::Vec3(5.0f,0.0f,0.0f));
	pat->addChild(node.get());

	// 子节点 mat, 共享 node
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
	osg::Matrix m;
	m.makeScale(2.0f,2.0f,2.0f);
	mt->setMatrix(m);
	mt->addChild(node.get());

	// 添加子节点
	root->addChild(pat.get());
	root->addChild(mt.get());
	
	// 一个 多父节点和一个子节点场景 的默认深拷贝
	osg::ref_ptr<osg::Node> deepCopy = dynamic_cast<osg::Node*>(node->clone(osg::CopyOp::DEEP_COPY_ALL));
	std::cout<<std::endl<<"完成一个 多父节点一个子节点 的场景的默认深拷贝"<<std::endl<<std::endl;
	osgDB::writeNodeFile((*deepCopy.get()),"deepCopy.osg");


	// 完成一个  多父节点和一个子节点 场景的浅拷贝
	osg::ref_ptr<osg::Node> myShallowCopy = dynamic_cast<osg::Node*>(node->clone(GraphCopyOp::Options(osg::CopyOp::SHALLOW_COPY)));
	std::cout<<std::endl<<"完成一个 多父节点和一个子节点 场景的浅拷贝"<<std::endl<<std::endl;
	osgDB::writeNodeFile((*myShallowCopy.get()),"myShallowCopy.osg");

	// 完成一个  多父节点和一个子节点 场景的深拷贝
	osg::ref_ptr<osg::Node> myDeepCopy = dynamic_cast<osg::Node*>(node->clone(GraphCopyOp::Options(osg::CopyOp::DEEP_COPY_ALL)));
	std::cout<<std::endl<<"完成一个 多父节点和一个子节点 场景的深拷贝"<<std::endl<<std::endl;
	osgDB::writeNodeFile((*myDeepCopy.get()),"myDeepCopy.osg");

	// 优化场景
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();
	viewer->run();

}