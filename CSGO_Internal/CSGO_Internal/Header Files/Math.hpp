#pragma once

#include <xmmintrin.h>
#include <cmath>

enum
{
	PITCH = 0, // up / down
	YAW,       // left / right
	ROLL       // fall over
};

namespace Math
{
	static constexpr float pi = 3.14159265358979323846f;
	static constexpr float radpi = 57.295779513082f;
	static constexpr float pirad = 0.01745329251f;

	static constexpr float Deg2Rad(const float& x)
	{
		return x * pirad;
	}

	static constexpr float Rad2Deg(const float& x)
	{
		return x * radpi;
	}

	inline void CorrectMovement(vec3 oldAngles, c_user_cmd* pCmd, float flOldForwardMove, float flOldSideMove)
	{
		float flDeltaView = 0.f;
		float f1 = 0.f;
		float f2 = 0.f;

		if (oldAngles.y < 0.f)
			f1 = 360.f + oldAngles.y;
		else
			f1 = oldAngles.y;

		if (pCmd->m_view_angles.y < 0.f)
			f2 = 360.f + pCmd->m_view_angles.y;
		else
			f2 = pCmd->m_view_angles.y;

		if (f2 < f1)
			flDeltaView = abs(f2 - f1);
		else
			flDeltaView = 360.f - abs(f1 - f2);

		flDeltaView = 360.f - flDeltaView;

		pCmd->m_forward_move = cos(Deg2Rad(flDeltaView)) * flOldForwardMove + cos(Deg2Rad(flDeltaView + 90.f)) * flOldSideMove;
		pCmd->m_side_move = sin(Deg2Rad(flDeltaView)) * flOldForwardMove + sin(Deg2Rad(flDeltaView + 90.f)) * flOldSideMove;
	}

	inline float NormalizeAngle(float angle)
	{
		auto revolutions = angle / 360.f;

		if (angle > 180.f || angle < -180.f)
		{
			revolutions = round(abs(revolutions));

			if (angle < 0.f)
				angle = (angle + 360.f * revolutions);
			else
				angle = (angle - 360.f * revolutions);

			return angle;
		}

		return angle;
	}

	inline float GetDistance(vec3 xyz1, vec3 xyz2)
	{
		return sqrt(pow(xyz2.x - xyz1.x, 2) + pow(xyz2.y - xyz1.y, 2) + pow(xyz2.z - xyz1.z, 2));
	}

	inline bool IsInFOV(vec2 screen_size, vec2 target_pos, vec2 radius)
	{
		// should be used for things like calculating the closest player to crosshair.
		float final_w = screen_size.x / 2.f;
		float final_h = screen_size.y / 2.f;

		bool in_fov = target_pos.x >= final_w - radius.x && target_pos.x <= final_w + radius.x && target_pos.y >= final_h - radius.y && target_pos.y <= final_h + radius.y;

		return in_fov;
	}

	inline float GetCrosshairDistanceFromTarget(vec2 target_pos, vec2 screen_size)
	{
		// Note - to do; z axis?
		float dist = GetDistance({ target_pos.x, target_pos.y, NULL }, { screen_size.x / 2.f, screen_size.y / 2.f, NULL });

		return dist;
	}

	// Note - to do; Rework.
	inline float AngleDifference(float start, float end)
	{
		float difference;

		for (; start > 180.0; start = start - 360.0);
		for (; start < -180.0; start = start + 360.0);

		for (; end > 180.0; end = end - 360.0);
		for (; end < -180.0; end = end + 360.0);

		for (difference = end - start; difference > 180.0; difference = difference - 360.0);
		for (; difference < -180.0; difference = difference + 360.0);

		return difference;
	}

	inline float __cdecl DotProduct_ASM(const float _v1[3], const float _v2[3])
	{
		float dotret;

		__asm
		{
			mov ecx, _v1
			mov eax, _v2
			; optimized dot product; 15 cycles
			fld dword ptr[eax + 0]; starts& ends on cycle 0
			fmul dword ptr[ecx + 0]; starts on cycle 1
			fld dword ptr[eax + 4]; starts& ends on cycle 2
			fmul dword ptr[ecx + 4]; starts on cycle 3
			fld dword ptr[eax + 8]; starts& ends on cycle 4
			fmul dword ptr[ecx + 8]; starts on cycle 5
			fxch st(1); no cost
			faddp st(2), st(0); starts on cycle 6, stalls for cycles 7 - 8
			faddp st(1), st(0); starts on cycle 9, stalls for cycles 10 - 12
			fstp dword ptr[dotret]; starts on cycle 13, ends on cycle 14
		}

		return dotret;
	}

	// Even more FPS friendly :D
	inline void VectorTransformASM(const float* in1, const matrix3x4_t& in2, float* out1)
	{
		out1[0] = DotProduct_ASM(in1, in2[0]) + in2[0][3];
		out1[1] = DotProduct_ASM(in1, in2[1]) + in2[1][3];
		out1[2] = DotProduct_ASM(in1, in2[2]) + in2[2][3];
	}

	inline void VectorTransform(vec3& in1, const matrix3x4_t& in2, vec3& out)
	{
		out.x = in1.dot(in2.m_flMatVal[0]) + in2.m_flMatVal[0][3];
		out.y = in1.dot(in2.m_flMatVal[1]) + in2.m_flMatVal[1][3];
		out.z = in1.dot(in2.m_flMatVal[2]) + in2.m_flMatVal[2][3];
	}

	inline vec3 VectorTransform(vec3 vec, matrix3x4_t matrix)
	{
		vec3 out;

		out.x = vec.dot(matrix[0]) + matrix[0][3];
		out.y = vec.dot(matrix[1]) + matrix[1][3];
		out.z = vec.dot(matrix[2]) + matrix[2][3];

		return out;
	}

	inline vec2 AnglesToPixels(const vec3& angles)
	{
		static const auto m_pitch = g_csgo.m_engine_cvars->FindVar("m_pitch");
		static const auto m_yaw = g_csgo.m_engine_cvars->FindVar("m_yaw");

		float x = angles.x / m_pitch->get_float();
		float y = angles.y / m_yaw->get_float();

		return vec2(-y, x);
	}

	inline vec3 PixelsToAngles(const vec2& pixels)
	{
		static const auto m_pitch = g_csgo.m_engine_cvars->FindVar("m_pitch");
		static const auto m_yaw = g_csgo.m_engine_cvars->FindVar("m_yaw");

		float x = pixels.y * m_pitch->get_float();
		float y = -pixels.x * m_yaw->get_float();

		return vec3(x, y, 0);
	}

	// Rotate 2D by distance.
	inline void FindPosRotation(float& x, float& y, const float& screen_width, const float& screen_height)
	{
		const vec2  delta = vec2(screen_width / 2 - x, screen_height / 2 - y);
		const float hypot = std::hypot(delta.x, delta.y);
		const float rotation = Rad2Deg(std::acos(delta.x / hypot));

		x += std::sin(rotation);
		y += std::cos(rotation);
	}

	inline vec3 MatrixGetPosition(const matrix3x4_t& src)
	{
		return vec3(src[0][3], src[1][3], src[2][3]);
	}

	inline void MatrixSetPosition(matrix3x4_t& src, vec3 position)
	{
		src[0][3] = position.x;
		src[1][3] = position.y;
		src[2][3] = position.z;
	}

	inline void AngleMatrix(vec3 angles, matrix3x4_t& matrix)
	{
		float sr, sp, sy, cr, cp, cy;

		sp = sinf(angles.x * pirad);
		cp = cosf(angles.x * pirad);
		sy = sinf(angles.y * pirad);
		cy = cosf(angles.y * pirad);
		sr = sinf(angles.z * pirad);
		cr = cosf(angles.z * pirad);

		matrix[0][0] = cp * cy;
		matrix[1][0] = cp * sy;
		matrix[2][0] = -sp;

		float crcy = cr * cy;
		float crsy = cr * sy;
		float srcy = sr * cy;
		float srsy = sr * sy;

		matrix[0][1] = sp * srcy - crsy;
		matrix[1][1] = sp * srsy + crcy;
		matrix[2][1] = sr * cp;

		matrix[0][2] = (sp * crcy + srsy);
		matrix[1][2] = (sp * crsy - srcy);
		matrix[2][2] = cr * cp;

		matrix[0][3] = 0.f;
		matrix[1][3] = 0.f;
		matrix[2][3] = 0.f;
	}

	inline void AngleIMatrix(vec3 angles, matrix3x4_t& matrix)
	{
		float sr, sp, sy, cr, cp, cy;

		sp = sinf(angles.x * pirad);
		cp = cosf(angles.x * pirad);
		sy = sinf(angles.y * pirad);
		cy = cosf(angles.y * pirad);
		sr = sinf(angles.z * pirad);
		cr = cosf(angles.z * pirad);

		matrix[0][0] = cp * cy;
		matrix[0][1] = cp * sy;
		matrix[0][2] = -sp;
		matrix[1][0] = sr * sp * cy + cr * -sy;
		matrix[1][1] = sr * sp * sy + cr * cy;
		matrix[1][2] = sr * cp;
		matrix[2][0] = (cr * sp * cy + -sr * -sy);
		matrix[2][1] = (cr * sp * sy + -sr * cy);
		matrix[2][2] = cr * cp;
		matrix[0][3] = 0.f;
		matrix[1][3] = 0.f;
		matrix[2][3] = 0.f;
	}

	inline void AngleMatrix(vec3 angles, matrix3x4_t& matrix, vec3 origin)
	{
		AngleMatrix(angles, matrix);
		MatrixSetPosition(matrix, origin);
	}

	inline vec3 VectorRotate(vec3& vec, float rot)
	{
		rot = -rot + 90.f;

		float radius = sqrtf(vec.x * vec.x + vec.y * vec.y);
		float radian = rot * pirad;

		vec.x = sin(radian) * radius;
		vec.y = cos(radian) * radius;

		return vec;
	}

	inline vec3 VectorRotate(const vec3& vec, const vec3& in2)
	{
		vec3		out;
		matrix3x4_t rotate;

		AngleMatrix(in2, rotate);

		out.x = DotProduct_ASM(reinterpret_cast<const float*>(&vec), rotate[0]);
		out.y = DotProduct_ASM(reinterpret_cast<const float*>(&vec), rotate[1]);
		out.z = DotProduct_ASM(reinterpret_cast<const float*>(&vec), rotate[2]);

		return out;
	}


	inline void MatrixCopy(const matrix3x4_t& src, matrix3x4_t& dst)
	{
		dst[0][0] = src[0][0];
		dst[1][0] = src[1][0];
		dst[2][0] = src[2][0];

		dst[0][1] = src[0][1];
		dst[1][1] = src[1][1];
		dst[2][1] = src[2][1];

		dst[0][2] = src[0][2];
		dst[1][2] = src[1][2];
		dst[2][2] = src[2][2];

		dst[0][3] = src[0][3];
		dst[1][3] = src[1][3];
		dst[2][3] = src[2][3];
	}

	inline void MatrixAngles(const matrix3x4_t& matrix, vec3& angles)
	{
		float forward[3];
		float left[3];
		float up[3];

		// Extract the basis vectors from the matrix. Since we only need the Z
		// component of the up vector, we don't get X and Y.
		forward[0] = matrix[0][0];
		forward[1] = matrix[1][0];
		forward[2] = matrix[2][0];

		left[0] = matrix[0][1];
		left[1] = matrix[1][1];
		left[2] = matrix[2][1];

		up[2] = matrix[2][2];

		float xyDist = sqrtf(forward[0] * forward[0] + forward[1] * forward[1]);

		// enough here to get angles?
		if (xyDist > 0.001f)
		{
			// (yaw)	y = ATAN( forward.y, forward.x );		-- in our space, forward is the X axis
			angles.y = Rad2Deg(atan2f(forward[1], forward[0]));

			// (pitch)	x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
			angles.x = Rad2Deg(atan2f(-forward[2], xyDist));

			// (roll)	z = ATAN( left.z, up.z );
			angles.z = Rad2Deg(atan2f(left[2], up[2]));
		}
		else	// forward is mostly Z, gimbal lock-
		{
			// (yaw)	y = ATAN( -left.x, left.y );			-- forward is mostly z, so use right for yaw
			angles.y = Rad2Deg(atan2f(-left[0], left[1]));

			// (pitch)	x = ATAN( -forward.z, sqrt(forward.x*forward.x+forward.y*forward.y) );
			angles.x = Rad2Deg(atan2f(-forward[2], xyDist));

			// Assume no roll in this case as one degree of freedom has been lost (i.e. yaw == roll)
			angles.z = 0;
		}
	}

	// mfn epic compiler
	inline vec3 _VectorAngles(const vec3& start, const vec3& end)
	{
		const vec3 delta = end - start;

		return vec3(Rad2Deg(std::atan2(-delta.z, std::sqrt(delta.x * delta.x + delta.y * delta.y))),
					Rad2Deg(std::atan2(delta.y, delta.x)),
					0.0f);
	}

	inline void VectorAngles(const vec3& forward, vec3& angles)
	{
		angles = vec3(Rad2Deg(std::atan2(-forward.z, std::sqrt(forward.x * forward.x + forward.y * forward.y))),
					  Rad2Deg(std::atan2(forward.y, forward.x)),
					  0.0f);
	}

	inline void __VectorAngles(const vec3& forward, vec3& angles)
	{
		angles.x += Rad2Deg(std::atan(std::sqrt(forward.x * forward.x + forward.y * forward.y)));
		angles.z = -Rad2Deg(std::atan2(forward.x, forward.y));
	}

	inline vec3 VectorMa(const vec3& start, float scale, const vec3& dir)
	{
		return start + dir * scale;
	}

	inline void SinCos(float radians, float* sine, float* cosine)
	{
		*sine = sin(radians);
		*cosine = cos(radians);
	}

	inline void AngleVectors(const vec3& angles, vec3* forward = nullptr, vec3* right = nullptr, vec3* up = nullptr)
	{
		float sr, sp, sy, cr, cp, cy;

		SinCos(Deg2Rad(angles[YAW]), &sy, &cy);
		SinCos(Deg2Rad(angles[PITCH]), &sp, &cp);
		SinCos(Deg2Rad(angles[ROLL]), &sr, &cr);

		if (forward)
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
			right->y = (-1 * sr * sp * sy + -1 * cr * cy);
			right->z = -1 * sr * cp;
		}

		if (up)
		{
			up->x = (cr * sp * cy + -sr * -sy);
			up->y = (cr * sp * sy + -sr * cy);
			up->z = cr * cp;
		}
	}

	inline float NormalizeFloat(float val, float min, float max)
	{
		float step = max - min;

		while (val > max)
			val -= step;

		while (val < min)
			val += step;

		return val;
	}

	namespace Import
	{
		__forceinline uint32_t PseudoRandomMD5(uint32_t uSeed)
		{
			static const auto PseudoRandom = reinterpret_cast<uint32_t(__cdecl*)(uint32_t)>(Memory::FindSignature("client.dll", "55 8B EC 83 E4 F8 83 EC 70 6A 58"));

			if (PseudoRandom)
				return PseudoRandom(uSeed);

			return 0;
		}

		__forceinline float RandomFloat(float flMin, float flMax)
		{
			static auto RandomFloat = reinterpret_cast<float(__cdecl*)(float, float)>(GetProcAddress(GetModuleHandle(("vstdlib.dll")), ("RandomFloat")));

			if (RandomFloat)
				return RandomFloat(flMin, flMax);

			return flMin;
		}

		__forceinline int RandomInt(int flMin, int flMax)
		{
			static auto RandomInt = reinterpret_cast<int(__cdecl*)(int, int)>(GetProcAddress(GetModuleHandle(("vstdlib.dll")), ("RandomInt")));

			if (RandomInt)
				return RandomInt(flMin, flMax);

			return flMin;
		}

		__forceinline void RandomSeed(uint32_t uSeed)
		{
			// Signature can be used as well.
			static auto RandomSeed = reinterpret_cast<void(__cdecl*)(uint32_t)>(GetProcAddress(GetModuleHandle(("vstdlib.dll")), ("RandomSeed")));

			if (RandomSeed)
				return RandomSeed(uSeed);
		}
	}

	namespace Time
	{
		static inline float GetTickInterval()
		{
			return g_csgo.m_globals->m_interval_per_tick;
		}

		static inline int ToTicks(const float& time)
		{
			return static_cast<int>(0.5f + time / GetTickInterval());
		}

		static inline float ToTime(const int& ticks)
		{
			return GetTickInterval() * ticks;
		}

		static constexpr int iTickNeverThink = -1;
	}
}
