#include "StdAfx.h"
#include "CollisionVolume.h"
#include "LogOutput.h"
#include "mmgr.h"


static CLogSubsystem LOG_COLVOL("CollisionVolume");


CR_BIND(CollisionVolume, );
	CR_REG_METADATA(CollisionVolume, (
		CR_MEMBER(axisScales),
		CR_MEMBER(axisHScales),
		CR_MEMBER(axisHScalesSq),
		CR_MEMBER(axisHIScales),
		CR_MEMBER(axisOffsets),
		CR_MEMBER(volumeBoundingRadius),
		CR_MEMBER(volumeBoundingRadiusSq),
		CR_MEMBER(volumeType),
		CR_MEMBER(testType),
		CR_MEMBER(primaryAxis),
		CR_MEMBER(secondaryAxes),
		CR_MEMBER(disabled)
	));


CollisionVolume::CollisionVolume(const CollisionVolume* src)
{
	//logOutput.Print(LOG_COLVOL, "CollisionVolume::CollisionVolume(src = 0x%p)", src);

	axisScales             = src? src->axisScales:             float3(2.0f, 2.0f, 2.0f);
	axisHScales            = src? src->axisHScales:            float3(1.0f, 1.0f, 1.0f);
	axisHScalesSq          = src? src->axisHScalesSq:          float3(1.0f, 1.0f, 1.0f);
	axisHIScales           = src? src->axisHIScales:           float3(1.0f, 1.0f, 1.0f);
	axisOffsets            = src? src->axisOffsets:            ZeroVector;
	volumeBoundingRadius   = src? src->volumeBoundingRadius:   1.0f;
	volumeBoundingRadiusSq = src? src->volumeBoundingRadiusSq: 1.0f;
	volumeType             = src? src->volumeType:             COLVOL_TYPE_ELLIPSOID;
	testType               = src? src->testType:               COLVOL_TEST_DISC;
	primaryAxis            = src? src->primaryAxis:            COLVOL_AXIS_Z;
	secondaryAxes[0]       = src? src->secondaryAxes[0]:       COLVOL_AXIS_X;
	secondaryAxes[1]       = src? src->secondaryAxes[1]:       COLVOL_AXIS_Y;
	disabled               = src? src->disabled:               false;
}


CollisionVolume::CollisionVolume(const std::string& typeStr, const float3& scales, const float3& offsets, int testType)
{
	primaryAxis = COLVOL_AXIS_Z;

	if (typeStr.size() > 0) {
		std::string typeStrLC(StringToLower(typeStr));

		if (typeStrLC.find("ell") != std::string::npos) {
			logOutput.Print(LOG_COLVOL, "New ellipsoid");
			volumeType = COLVOL_TYPE_ELLIPSOID;
		}

		if (typeStrLC.find("cyl") != std::string::npos) {
			logOutput.Print(LOG_COLVOL, "New cylinder");
			volumeType = COLVOL_TYPE_CYLINDER;

			if (typeStrLC.size() == 4) {
				if (typeStrLC[3] == 'x') { primaryAxis = COLVOL_AXIS_X; }
				if (typeStrLC[3] == 'y') { primaryAxis = COLVOL_AXIS_Y; }
				if (typeStrLC[3] == 'z') { primaryAxis = COLVOL_AXIS_Z; }
			}
		}

		if (typeStrLC.find("box") != std::string::npos) {
			logOutput.Print(LOG_COLVOL, "New box");
			volumeType = COLVOL_TYPE_BOX;
		}

		if (typeStrLC.find("footprint") != std::string::npos) {
			logOutput.Print(LOG_COLVOL, "New footprint");
			volumeType = COLVOL_TYPE_FOOTPRINT;
		}

	} else {
		logOutput.Print(LOG_COLVOL, "New footprint (default)");
		volumeType = COLVOL_TYPE_FOOTPRINT;
	}

	Init(scales, offsets, volumeType, testType, primaryAxis);
}


void CollisionVolume::SetDefaultScale(const float s)
{
	//logOutput.Print(LOG_COLVOL, "SetDefaultScale(s = %g)", s);

	// called iif unit or feature defines no custom volume,
	// <s> is the object's default RADIUS (not its diameter)
	// so we need to double it to get the full-length scales
	const float3 scales(s * 2.0f, s * 2.0f, s * 2.0f);

	Init(scales, ZeroVector, volumeType, testType, primaryAxis);
}


void CollisionVolume::Init(const float3& scales, const float3& offsets, int vType, int tType, int pAxis)
{
	//logOutput.Print(LOG_COLVOL, "Init(scales={%g,%g,%g}, offsets={%g,%g,%g}, vType=%d, tType=%d, pAxis=%d)",
	//                scales.x, scales.y, scales.z, offsets.x, offsets.y, offsets.z, vType, tType, pAxis);

	disabled = (tType < 0);

	// assign these here, since we can be
	// called from outside the constructor
	primaryAxis = std::max(pAxis, 0) % COLVOL_NUM_AXES;
	volumeType = std::max(vType, 0) % COLVOL_NUM_TYPES;
	testType = std::max(tType, 0) % COLVOL_NUM_TESTS;

	axisScales.x = (scales.x < 1.0f)? 1.0f: scales.x;
	axisScales.y = (scales.y < 1.0f)? 1.0f: scales.y;
	axisScales.z = (scales.z < 1.0f)? 1.0f: scales.z;

	axisHScales.x = axisScales.x * 0.5f;  axisHScalesSq.x = axisHScales.x * axisHScales.x;
	axisHScales.y = axisScales.y * 0.5f;  axisHScalesSq.y = axisHScales.y * axisHScales.y;
	axisHScales.z = axisScales.z * 0.5f;  axisHScalesSq.z = axisHScales.z * axisHScales.z;

	axisHIScales.x = 1.0f / axisHScales.x;  axisOffsets.x = offsets.x;
	axisHIScales.y = 1.0f / axisHScales.y;  axisOffsets.y = offsets.y;
	axisHIScales.z = 1.0f / axisHScales.z;  axisOffsets.z = offsets.z;

	// if all axes (or half-axes) are equal in scale, volume is a sphere
	const bool spherical =
		((volumeType == COLVOL_TYPE_ELLIPSOID) &&
		(streflop::fabsf(axisHScales.x - axisHScales.y) < EPS) &&
		(streflop::fabsf(axisHScales.y - axisHScales.z) < EPS));

	if (spherical) {
		logOutput.Print(LOG_COLVOL, "Auto converting spherical ellipsoid to sphere");
		volumeType = COLVOL_TYPE_SPHERE;
	}

	// secondaryAxes[0] = (primaryAxis + 1) % 3;
	// secondaryAxes[1] = (primaryAxis + 2) % 3;

	switch (primaryAxis) {
		case COLVOL_AXIS_X: {
			secondaryAxes[0] = COLVOL_AXIS_Y;
			secondaryAxes[1] = COLVOL_AXIS_Z;
		} break;
		case COLVOL_AXIS_Y: {
			secondaryAxes[0] = COLVOL_AXIS_X;
			secondaryAxes[1] = COLVOL_AXIS_Z;
		} break;
		case COLVOL_AXIS_Z: {
			secondaryAxes[0] = COLVOL_AXIS_X;
			secondaryAxes[1] = COLVOL_AXIS_Y;
		} break;
	}

	// set the radius of the minimum bounding sphere
	// that encompasses this custom collision volume
	// (for early-out testing)
	switch (volumeType) {
		case COLVOL_TYPE_BOX: {
			// would be an over-estimation for cylinders
			volumeBoundingRadiusSq = axisHScalesSq.x + axisHScalesSq.y + axisHScalesSq.z;
			volumeBoundingRadius = streflop::sqrt(volumeBoundingRadiusSq);
		} break;
		case COLVOL_TYPE_CYLINDER: {
			const float prhs = axisHScales[primaryAxis     ];   // primary axis half-scale
			const float sahs = axisHScales[secondaryAxes[0]];   // 1st secondary axis half-scale
			const float sbhs = axisHScales[secondaryAxes[1]];   // 2nd secondary axis half-scale
			const float mshs = std::max(sahs, sbhs);            // max. secondary axis half-scale

			volumeBoundingRadiusSq = prhs * prhs + mshs * mshs;
			volumeBoundingRadius = streflop::sqrtf(volumeBoundingRadiusSq);
		} break;
		case COLVOL_TYPE_ELLIPSOID: {
			volumeBoundingRadius = std::max(axisHScales.x, std::max(axisHScales.y, axisHScales.z));
			volumeBoundingRadiusSq = volumeBoundingRadius * volumeBoundingRadius;
		} break;
		case COLVOL_TYPE_FOOTPRINT:
			// fall through, this is intersection of footprint-prism
			// and sphere, so it has same bounding radius as sphere.
		case COLVOL_TYPE_SPHERE: {
			// max{x, y, z} would suffice here too (see ellipsoid)
			volumeBoundingRadius = axisHScales.x;
			volumeBoundingRadiusSq = volumeBoundingRadius * volumeBoundingRadius;
		} break;
	}
}
