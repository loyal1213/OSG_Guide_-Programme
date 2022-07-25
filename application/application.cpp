// application.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "第3讲 osgviewer 应用基础 01.hpp"
#include "第5讲 基本图元(上).hpp"
#include "第6讲 基本图元(下).hpp"
#include "第7讲 矩阵操作(上).hpp"
#include "3_2_2 布告板示例.hpp"
#include "3_3_1位置变换节点.hpp"
#include "3_3_4 矩阵变换节点示例.hpp"
#include "3_3_5 自动对齐节点.hpp"
#include "3_3_7 开关节点.hpp"
#include "3_3_10 细节层次节点示例.hpp"
#include "3_3_12 分页细节层次节点示例.hpp"
#include "3_3_13 替代节点.hpp"
#include "3_3_18 坐标系节点示例.hpp"
#include "7_3_3 事件回调示例.hpp"
#include "7_3_2 节点回调示例.hpp"
#include "8_2_2 键盘事件消息处理.hpp"
#include "8_2_4 场景漫游.hpp"
#include "OSG粒子飞行尾迹.hpp"
#include "基于OE自定义自由飞行漫游器(初级版).hpp"
#include "osganimate.hpp"

#include "osgearth 读取飞机模型.hpp"
#include "9_1_3 显示汉字示例.hpp"
#include "9_1_4 各种文字效果(边框;阴影;颜色;倾斜)示例.hpp"
#include "9_1_5 HUD 显示汉字示例.hpp"
#include "9_1_7 渐变文字示例.hpp"
#include "9_2_2 3D汉字显示示例.hpp"
#include "3_3_16 遮挡裁剪节点示例.hpp"

#include "4_2_2 基本几何体绘制.hpp"
#include "4_2_3 索引绑定几何体绘制示例.hpp"
#include "4_3_3 网格化类.hpp"
#include "4_5_8 生成顶点法向量示例.hpp"
#include "4_4 多边形分格化.hpp"
#include "4_5_1 简化几何体.hpp"
#include "4_5_4 三角形网绘制示例.hpp"
#include "4_5_5 三角带绘制.hpp"
#include "5_3_4 简单光源示例.hpp"
#include "5_1_4 渲染状态示例.hpp"
#include "5_2_2 二维纹理映射示例.hpp"
#include "5_2_4 多重纹理映射示例.hpp"
#include "5_2_3 多重纹理映射.hpp"
#include "5_2_17 一维纹理.hpp"
#include "5_2_20 三维纹理映射示例.hpp"
#include "5_3_5 聚光灯示例.hpp"
#include "5_4_2 材质类示例.hpp"
#include "5_2_13 立方体图纹理.hpp"
#include "5_2_9 自动生成纹理坐标.hpp"
#include "5_2_11 计算纹理坐标.hpp"
#include "6_2_4 文件读写进度.hpp"
#include "7_2_3 顶点访问器示例.h"

int _tmain(int argc, char* argv[])
{
	// test_osgviewer();
	// TestGeode();
	// TestGeom();
	// TestMatrix();
	// TestBillboard();
	// TestPositionTransform();
	// TestMatrixTransform();
	// TestAutoTransform();
	// TestSwitch();
	// TestLod();
	// TestPageLod();
	// TestImpostor();
	// TestEventCallback();
	// TestRotateCallBack();
	// TestEventHandle();
	// TestTravelManipulator();

	// TestTrail();

	// TestEarthWalk(argc,argv);
	// TestOsgAnimate();
	// TestPositionTransform();
	// TestMatrixTransform();
	// TestOsgearthPlane();
	// TestOsgText();
	// TestAllKindText();
	// TestHUDText();
	// TestFadeText();
	// Test3DText();
	// TestCoordinateSystemNode();

	// TestTessellator();
	// TestSimplifier();
	// TestDelaunayTriangulator();
	// TestQuad1();

	// TestTriStripVisitor();
	// TestRotateCallBack();
	// TestLight();
	// TestClipNode();
	// TestTextureMultiple();
	// TestTexture1DOr2DOrMap();
	// TestTextureMultiple2();
	// TestTexture1D();
	// Test3DTextureMap();
	// TestSpotLight();
	// TestMaterial();
	// TestCubeMap();
	// TestTexGen();
	// TestComputingTextureCoordinates();
	// TestProgress();
	TestVertexNodeVisitor();
	return 0;
}

