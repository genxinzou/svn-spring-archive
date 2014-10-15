// Unit3DLoader.h: interface for the CUnit3DLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNIT3DLOADER_H__124A4FFE_7946_4E48_8823_CD79C234E677__INCLUDED_)
#define AFX_UNIT3DLOADER_H__124A4FFE_7946_4E48_8823_CD79C234E677__INCLUDED_

#pragma warning(disable:4786)

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>
#include <vector>

#include <map>

class CFileHandler;
using namespace std;

class CUnit3DLoader  
{
public:
	CUnit3DLoader();
	virtual ~CUnit3DLoader();

	struct Quad{
		int verteces[4];
		float3 normal;
		int normalType;
	};

	struct QuadTex{
		float texPos[4][2];
		string texName;
		int teamTex;
	};

	struct Tri{
		int verteces[3];
		float3 normal;
		int normalType;
	};
	
	struct TriTex{
		float texPos[3][2];
		string texName;
		int teamTex;
	};

	struct Propeller{
		float3 pos;
		float size;
	};

	struct AnimFrame{
		vector<float3> vertex;
		float length;
	};

	struct Animation{
		vector<AnimFrame> frames;
	};

	static struct ArrayVertexSub {
		float texCoord[2];
		int num;
	};
	static struct ArrayVertex {
		vector<ArrayVertexSub> sub;
	};

	struct UnitModelGeometry{
		float height;
		float radius;

		float3 offset;
		float3 rotVector;

		vector<float3> vertex;
		vector<float3> vertexNormal;
		vector<Quad> quad;
		vector<Tri> tri;

		vector<Propeller> propellers;
		map<string,Animation*> animations;

		unsigned int indexBuffer;
		unsigned int normalBuffer;
		int numIndeces;
		int numVerteces;

		bool isAnimated;

		vector<ArrayVertex> vertexSplitting;
	};

	struct UnitModel{
		int team;
		int farTextureNum;
		unsigned int displist;
		unsigned int displistAim;
		vector<QuadTex> quadTex;
		vector<TriTex> triTex;
		string name;
		vector<UnitModel*> subModels;
		
		unsigned int texCoordBuffer;
		UnitModelGeometry* geometry;
	};

	UnitModel* GetModel(string name,int team);
	void CreateNormals(UnitModel& model);
	unsigned int farTexture;
private:
	void CreateDispList(UnitModel& model);
	void CreateArrays(UnitModel& model);
	void CreateFarTexture(UnitModel &model);
	int Parse(const string& filename, UnitModel &model);
	int ParseSub(CFileHandler& fh, UnitModel &model,const string& filename,float3 offset,const string& treename);
	string GetWord(CFileHandler& fh);
	string GetLine(CFileHandler& fh);
	void MakeLow(std::string &s);
	void DeleteModel(UnitModel* model);
	void SplitArray(vector<float3>& vert, vector<ArrayVertex> va);
	unsigned int CreateDisplistFromVector(UnitModel& model, vector<float3>& vertex,bool fixTexCoords=true);

	map<string,UnitModel*> models;
	map<string,UnitModelGeometry*> geometryModels;
	unsigned char* farTextureMem;
	int usedFarTextures;
public:
};

extern CUnit3DLoader* unitModelLoader;
#endif // !defined(AFX_UNIT3DLOADER_H__124A4FFE_7946_4E48_8823_CD79C234E677__INCLUDED_)
