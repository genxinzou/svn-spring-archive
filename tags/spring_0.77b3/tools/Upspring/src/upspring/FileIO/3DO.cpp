//-----------------------------------------------------------------------
//  Upspring model editor
//  Copyright 2005 Jelmer Cnossen
//  This code is released under GPL license, see LICENSE.HTML for info.
//-----------------------------------------------------------------------
#include "EditorIncl.h"
#include "EditorDef.h"
#include "Util.h"
#include "Model.h"


class CTAPalette  
{
public:
	CTAPalette() {
		loaded=false;
		error=false;
	}

	void Init() {
		string fn=applicationPath + "palette.pal";
		FILE *f = fopen (fn.c_str(), "rb");
		if (!f) {
			if (!error)	logger.Trace (NL_Error, "Failed to load palette.pal");
			error=true;
		} else {
			for(int c=0;c<256;c++){
				for(int c2=0;c2<4;c2++)
					p[c][c2]=fgetc(f);
				p[c][3]=255;
			}
			fclose (f);
		}
	}

	int FindIndex(Vector3 color)
	{
		if (!loaded) Init();
		int r=color.x*255,g=color.y*255,b=color.z*255;
		int best=-1, bestdif;
		for (int a=0;a<256;a++) {
			int dif=abs(r-p[a][0])+abs(g-p[a][1])+abs(b-p[a][2]);
			if (best<0 || bestdif>dif) {
				bestdif = dif;
				best = a;
			}
		}
		return best;
	}

	Vector3 GetColor(int index) {
		if (!loaded) Init();
		if (index<0 || index>=256)
			return Vector3();
		return Vector3(p[index][0]/255, p[index][1]/255, p[index][2]/255);
	}

	inline unsigned char* operator[] (int a){
		return p[a];
	}
	unsigned char p[256][4];
	bool loaded,error;
};
CTAPalette palette;

const float scaleFactor=1/(65536.0f);

#define FROM_TA(c)  (float(c) * scaleFactor)
#define TO_TA(c) ((c)/scaleFactor)

// ------------------------------------------------------------------------------------------------
// 3DO support
// ------------------------------------------------------------------------------------------------

typedef struct
{
	long VersionSignature;
	long NumberOfVertexes;
	long NumberOfPrimitives;
	long NoSelectionRect;
	long XFromParent;
	long YFromParent;
	long ZFromParent;
	long OffsetToObjectName;        // 28
	long Always_0;                  // 32
	long OffsetToVertexArray;       // 36
	long OffsetToPrimitiveArray;    // 40
	long OffsetToSiblingObject;     // 44
	long OffsetToChildObject;       // 48
} TA_Object;

typedef struct
{
	long PaletteIndex;
	long VertNum;
	long Always_0;
	long VertOfs;
	long TexnameOfs;
	long Unknown_1; 
	long Unknown_2;
	long Unknown_3; 
} TA_Polygon;


static MdlObject* load_object(int ofs, FILE *f, MdlObject *parent, int r=0)
{
	MdlObject *n = new MdlObject;
	TA_Object obj;
	int org=ftell(f);

	fseek(f,ofs,SEEK_SET);
	fread(&obj, sizeof(TA_Object),1,f);

	if(obj.VersionSignature != 1)
	{
		logger.Trace (NL_Error,"Wrong version. Only version 1 is supported");;
		return 0;
	}

	long ipos[3];
	PolyMesh *pm = new PolyMesh;
	n->geometry = pm;
	pm->verts.resize (obj.NumberOfVertexes);
	
	fseek (f,obj.OffsetToVertexArray, SEEK_SET);
	for(int a=0;a<obj.NumberOfVertexes;a++)
	{
		fread(ipos, sizeof(long),3,f);
		for (int b=0;b<3;b++)
			pm->verts[a].pos.v[b] = FROM_TA(ipos[b]);
	}

	std::vector<TA_Polygon> tapl;
	tapl.resize (obj.NumberOfPrimitives);

	fseek (f, obj.OffsetToPrimitiveArray, SEEK_SET);
	if (obj.NumberOfPrimitives > 0)
		fread (&tapl[0], sizeof(TA_Polygon), obj.NumberOfPrimitives, f);
	for (int a=0;a<obj.NumberOfPrimitives;a++)
	{
		fseek (f, tapl[a].VertOfs,SEEK_SET);

		Poly *p = new Poly;
		p->verts.resize (tapl[a].VertNum);

		for (int b=0;b<tapl[a].VertNum;b++)
		{
			short vindex;
			fread (&vindex, sizeof(short), 1, f);
			p->verts[b] = vindex;
		}

		p->taColor = tapl[a].PaletteIndex;
		p->color = palette.GetColor(tapl[a].PaletteIndex);

		fseek (f, tapl[a].TexnameOfs, SEEK_SET);
		p->texname = ReadZStr(f);

		pm->poly.push_back (p);
	}

	fseek (f, obj.OffsetToObjectName, SEEK_SET);
	n->name = ReadZStr (f);

	n->position.v[0] = FROM_TA(obj.XFromParent);
	n->position.v[1] = FROM_TA(obj.YFromParent);
	n->position.v[2] = FROM_TA(obj.ZFromParent);

	if (obj.OffsetToSiblingObject)
	{
		if (!parent)
		{
			logger.Trace (NL_Error,"Error: Root object can not have sibling nodes.\n");
		}
		else {
			MdlObject *sibling = load_object (obj.OffsetToSiblingObject,f, parent);

			if (sibling) {
				parent->childs.push_back (sibling);
				sibling->parent = parent;
			}
		}
	}

	if (obj.OffsetToChildObject)
	{
		MdlObject *ch = load_object (obj.OffsetToChildObject, f, n, r+1);
		if (ch) {
			n->childs.push_back (ch);
			ch->parent = n;
		}
	}

	fseek(f,org,SEEK_SET);
	return n;
}

bool Model::Load3DO (const char *filename, IProgressCtl& progctl)
{
	FILE *f=0;

	f = fopen( filename, "rb" );
	if( !f )
		return false;

	root = load_object( 0, f, 0 );
	if(!root)
	{
		fclose(f);
		return false;
	}

	mapping = MAPPING_3DO;

	fclose(f);
	return true;
}


static void save_object(FILE *f, MdlObject *parent, std::vector<MdlObject*>::iterator cur)
{
	TA_Object n;
	MdlObject *obj = *cur++;
	PolyMesh* pm = obj->ToPolyMesh();
	if (!pm) pm=new PolyMesh;

	memset (&n, 0, sizeof(TA_Object));

	int header = ftell(f);
	fseek (f, sizeof(TA_Object), SEEK_CUR);
	
	n.VersionSignature = 1;
	n.NumberOfPrimitives = pm->poly.size();
	n.XFromParent = TO_TA(obj->position.v[0]);
	n.YFromParent = TO_TA(obj->position.v[1]);
	n.ZFromParent = TO_TA(obj->position.v[2]);
	n.NumberOfVertexes = pm->verts.size();

	n.OffsetToObjectName = ftell(f);
	WriteZStr (f, obj->name);

	n.OffsetToVertexArray = ftell(f);
	for (int a=0;a<pm->verts.size();a++)
	{
		long v[3];
		Vector3 *p = &pm->verts[a].pos;
		for (int i=0;i<3;i++) v[i] = TO_TA(p->v[i]);
		fwrite (v, sizeof(long), 3, f);
	}

	n.OffsetToPrimitiveArray = ftell(f);
	TA_Polygon *tapl = new TA_Polygon [pm->poly.size()];
	fseek (f, sizeof(TA_Polygon) * pm->poly.size(), SEEK_CUR);
	memset (tapl,0,sizeof(TA_Polygon)*pm->poly.size());

	for (int a=0;a<pm->poly.size();a++)
	{
		Poly *pl = pm->poly [a];

		if (pl->taColor >= 0)
			tapl[a].PaletteIndex = pl->taColor;
		else
			tapl[a].PaletteIndex = palette.FindIndex(pl->color);

		tapl[a].TexnameOfs = ftell(f);
		WriteZStr (f,pl->texname);
		tapl[a].VertOfs = ftell(f);
		for (int b=0;b<pl->verts.size();b++)
		{
			unsigned short v;
			v = pl->verts[b];
			fwrite (&v, sizeof(short), 1, f);
		}
		tapl[a].VertNum = pl->verts.size();
		tapl[a].Unknown_1 = 0;
		tapl[a].Unknown_2 = 0;
		tapl[a].Unknown_3 = 0;
	}
	
	int old = ftell(f);
	fseek (f, n.OffsetToPrimitiveArray, SEEK_SET);
	fwrite (tapl, sizeof(TA_Polygon), pm->poly.size(),f);
	fseek (f, old, SEEK_SET);

	delete pm;

	if (parent) {
		if (cur != parent->childs.end()) {
			n.OffsetToSiblingObject = ftell(f);
			save_object (f, parent, cur);
		}
	}

	if (obj->childs.size()) {
		n.OffsetToChildObject = ftell(f);
		save_object (f, obj, obj->childs.begin());
	}

	old=ftell(f);
	fseek (f, header, SEEK_SET);
	fwrite (&n, sizeof(TA_Object), 1, f);
	fseek (f,old, SEEK_SET);
}

// 3DO only has offsets
static inline void ApplyOrientationAndScaling (MdlObject *o) 
{
	o->ApplyTransform(true,true,false);
}

bool Model::Save3DO (const char *fn, IProgressCtl& progctl)
{
	FILE *f = fopen(fn, "wb");

	if (!f)
		return false;

	MdlObject *cl = root->Clone();
	IterateObjects (cl, ApplyOrientationAndScaling);
	
	vector<MdlObject*> tmp;
	tmp.push_back (cl);
	save_object (f, 0, tmp.begin());
	fclose (f);

	delete cl;
	return true;
}

