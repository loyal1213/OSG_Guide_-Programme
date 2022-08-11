// c.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/NodeVisitor>

//为模型生成法线，法线放在同一个几何体当中
class BuildNormal : public osg::NodeVisitor
{
public:
    BuildNormal():osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) {}

	virtual void apply(osg::Geode& gnode)
	{
        //先看其里有多少个geometry，给每个geometry的每个顶点的法线生成一条实体线，再添加到gnode中
        //因为添加之后gnode的子结点就会增多，为了避免给法线的实体线再生成法线，因此这里先获取
        int geomNum = gnode.getNumDrawables();
        for (int i = 0; i < geomNum; i++)
        {
            osg::Geometry* geom = gnode.getDrawable(i)->asGeometry();
            if (nullptr != geom)
            {
                //先判断有法线而且是逐顶点绑定
                osg::Vec3Array* normal = dynamic_cast<osg::Vec3Array*>(geom->getNormalArray());
                if ((nullptr != normal) && (geom->getNormalBinding() == osg::Array::BIND_PER_VERTEX))
                {
                    //给法线生成Geometry
                    osg::Geometry* geomNormalLine = new osg::Geometry();
                    gnode.addDrawable(geomNormalLine);
                    //关闭光照
                    geomNormalLine->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);

                    //申请顶点
                    osg::Vec3Array* nVertex = new osg::Vec3Array;
                    geomNormalLine->setVertexArray(nVertex);
                    //申请颜色数组
                    osg::Vec4Array* nColor = new osg::Vec4Array;
                    geomNormalLine->setColorArray(nColor, osg::Array::BIND_PER_VERTEX);

                    //为每个顶点生成一条线
                    {
                        osg::Vec3Array* vertex = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
                        if (nullptr != vertex)
                        {
                            for (int j = 0; j < vertex->size(); j++)
                            {
                                //压入当前顶点为第一个顶点
                                nVertex->push_back(vertex->at(j));
                                //压入颜色
                                osg::Vec3 colorTemp = vertex->at(j);
                                colorTemp.normalize();
                                nColor->push_back(osg::Vec4(colorTemp.x(), colorTemp.y(), colorTemp.z(), 1.0));

                                //得到法线
                                osg::Vec3 normalTemp = normal->at(j);
                                nColor->push_back(osg::Vec4(normalTemp.x(), normalTemp.y(), normalTemp.z(), 1.0));

                                //得到顶点，为在法线方向上延长当前包围盒的半径的0.1倍
                                float n = gnode.getBound().radius()*0.1;
                                nVertex->push_back(vertex->at(j)+ normalTemp* n);
                            }
                        }
                    }
                    geomNormalLine->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, nVertex->size()));
                }
            }

        }
        traverse(gnode);
	}
};


int Test39()
{
    osgViewer::Viewer viewer;

    //读一个模型
    osg::Node* pNode = osgDB::readNodeFile("glider.osg");

    //为其生成法线
    BuildNormal bn;
    pNode->accept(bn);

    viewer.setSceneData(pNode);
    viewer.run();
    return 0;
}
