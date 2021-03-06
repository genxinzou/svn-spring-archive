// Actions

class CActions {
public:
	CActions(Global* GL);
	void Update(); // check points of interest in LOS and remove accordingly

	bool RandomSpiral(int unit,bool water =false); // send the unit off in a random direction
	bool SeekOutInterest(int unit, float range=1000.0f); // send a unit out to look for points of interest
	bool SeekOutNearestInterest(int unit);
	bool AddPoint(float3 pos); // adds a point of interest

	bool Attack(int uid, float3 pos);
	bool Attack(int unit, int target,bool mobile);
	bool AttackNear(int unit,float LOSmultiplier=3.0f); // Attack nearby enemies...
	bool AttackNearest(int unit); // Attack nearby enemies...
	bool DGunNearby(int uid); // Makes the unit fire dgun at nearby objects if it has a dgun type weapon
	bool DGun(int uid,int enemy); // Makes 'uid' DGun 'enemy'
	bool Capture(int uid,int enemy); // Makesunit a capture unit b
	bool Repair(int uid,int unit); // Makes unit a repair unit b
	bool Move(int unit,float3 pos); // makes the unit move to that position
	bool MoveToStrike(int unit,float3 pos);
	// makes the unit move to that position but keepign its range as if its the position of an enemy
	bool Guard(int unit,int guarded); // Makes the unit guard the other unit
	bool Reclaim(int uid,int enemy); // reclaim a unti or feature
	bool Retreat(int uid);
	bool SeekOutLastAssault(int unit);
	bool Trajectory(int unit,int traj);
	void UnitDamaged(int damaged,int attacker,float damage,float3 dir);

	bool CopyAttack(int unit,set<int> tocopy);
	bool CopyMove(int unit,set<int> tocopy);

	bool IfNobodyNearMoveToNearest(int uid, set<int> units);
	/*void UnitIdle(int unit){
		attackers.erase(unit);
	}
	void EnemyDestroyed(int attacker, int enemy){
		attackers.erase(attacker);
		map<int,int>::iterator qw = attacking.find(attacker);
		if(qw != attacking.end()){
			attacking.erase(qw);
		}
	}
	void UnitDestroyed(int unit){
		attackers.erase(unit);
		map<int,int>::iterator qw = attacking.find(unit);
		if(qw != attacking.end()){
			attacking.erase(qw);
		}
	}

	void KeepOutOfRange();
	set<int> attackers;
	map<int,int> attacking;*/

	void ScheduleIdle(int unit); // This unit needs it's UnitIdle() called soon so schedule a unitIdle command
	bool RepairNearby(int uid,float radius); // repair a nearby unit or unfinished building
	bool ReclaimNearby(int uid); // reclaim the nearest feature within a 700 pixel radius
	bool RessurectNearby(int uid); // issues an area resurrect if there are any features in a vicinity of 700 ticks
private:
	Global* G;
	vector<float3> points;
	float3 last_attack;
};
