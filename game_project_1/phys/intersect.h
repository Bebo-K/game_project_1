#ifndef INTERSECT_H
#define INTERSECT_H

#include<game_project_1/types/primitives.hpp>

bool CylinderVCylinderIntersect(float r1, float h1,float r2, float h2,vec3 offset,vec3& intersect_point);

bool AABBVAABBIntersect(vec3 size_1,vec3 size_2,vec3 offset,vec3& intersect_point);

bool SphereVSphereIntersect(float r1, float r2,vec3 offset,vec3& intersect_point);

bool CapsuleVCapsuleIntersect(vec3 size_1,vec3 size_2,quaternion rotation_1, quaternion rotation_2, vec3 offset,vec3& intersect_point);

bool ArcVArcIntersect(vec3 size_1,vec3 size_2,float y_rotation_1, float y_rotation_2, vec3 offset,vec3& intersect_point);

bool ArcVSphereIntersect(vec3 size_1,float r2,float y_rotation_1, vec3 offset,vec3& intersect_point);

bool ArcVCylinderIntersect(vec3 size_1,vec3 size_2,float y_rotation_1, vec3 offset,vec3& intersect_point);




#endif