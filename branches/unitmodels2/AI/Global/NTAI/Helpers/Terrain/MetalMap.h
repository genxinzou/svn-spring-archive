#define M_CLASS_VERSION "4"

class CMetalMap{
public:
	CMetalMap(Global* ai);
	virtual ~CMetalMap();
	void Init();
	int NumSpotsFound;
	float AverageMetal;
	vector<float3> VectoredSpots;
	bool IsMetalMap(){
		if(AverageMetal != 0){
			if(NumSpotsFound == 0){
				return true;
			}
		}
		return false;
	}
	
	void SaveMetalMap();
	bool LoadMetalMap();
	void GetMetalPoints();
	
private:
	float3 BufferSpot;
	bool Stopme;
	int MaxSpots;
	int MetalMapHeight;
	int MetalMapWidth;
	int TotalCells;
	int SquareRadius;
	int DoubleSquareRadius;
	int TotalMetal;
	int MaxMetal;
	int TempMetal;
	int coordx;
	int coordy;
	int Minradius;
	int MinMetalForSpot;
	int XtractorRadius;
	int DoubleRadius;
	unsigned char* MexArrayA;	
	unsigned char* MexArrayB;
	unsigned char* MexArrayC; 
	int* TempAverage;
	Global *ai;
};
