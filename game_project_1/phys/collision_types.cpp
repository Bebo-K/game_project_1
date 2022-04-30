#include <game_project_1/phys/collision_types.hpp>
#include <game_project_1/types/str.hpp>
#include <math.h>


Ellipse_t::Ellipse_t(float h,float w){
    height=h;
    radius = w/2.0f;
}

Triangle::Triangle(){
    verts[0] = verts[1] = verts[2] = {0,0,0};
    face.distance=0;
    face.normal={0,1,0};
}
Triangle::Triangle(Triangle& t2){
    verts[0] = t2.verts[0];verts[1] = t2.verts[1];verts[2] = t2.verts[2];
    edges[0] = t2.edges[0];edges[1] = t2.edges[1];edges[2] = t2.edges[2];
    face = t2.face;
}
void Triangle::operator=(Triangle* t2){
    verts[0] = t2->verts[0];verts[1] = t2->verts[1];verts[2] = t2->verts[2];
    edges[0] = t2->edges[0];edges[1] = t2->edges[1];edges[2] = t2->edges[2];
    face = t2->face;
}

void Triangle::SetFromVertices(float vertices[9]){
    verts[0].x = vertices[0];
    verts[0].y = vertices[1];
    verts[0].z = vertices[2];
    verts[1].x = vertices[3];
    verts[1].y = vertices[4];
    verts[1].z = vertices[5];
    verts[2].x = vertices[6];
    verts[2].y = vertices[7];
    verts[2].z = vertices[8];
        
    edges[0] = verts[1]-verts[0];
    edges[1] = verts[2]-verts[0];
    edges[2] = verts[1]-verts[2];
    face.SetFromPointNormal(verts[0],edges[0].normalized().cross(edges[1].normalized()));
}

bool Triangle::Intersects(vec3 position,vec3* out_intersection_point){
    //returns true if the point projected onto the triangle plane is
    vec3 contact_point = ClosestPointToPlane(position);
    if(PointInTriangle(contact_point)){return true;}
    return false;
}

/*
vec3 Triangle::ClosestPoint(vec3 position){
    return ClosestPoint(position, nullptr);
}

vec3 Triangle::ClosestPoint(vec3 position,vec3* out_contact_normal){
    //If the closest point on the triangle's plane is inside the triangle itself we found our answer.
    vec3 plane_contact_point = ClosestPointToPlane(position);
    if(PointInTriangle(plane_contact_point)){
        if(out_contact_normal != nullptr){out_contact_normal->set(face.normal.x,face.normal.y,face.normal.z);}
        return plane_contact_point;
    }
    //Otherwise, check around the perimeter of the triangle
    else{return ClosestEdgePoint(position);}
}
*/

vec3 Triangle::ClosestEdgePoint(vec3 position){
    float closest_distance_2 = INFINITY;
    vec3 ret = {0,0,0};
    for(int i=0;i<3;i++){
        vec3 edge_point = ClosestPointOnEdge(position,verts[i],verts[(i+1)%3]);
        float edge_distance_2 = position.dist_sqr(edge_point);
        if(edge_distance_2 < closest_distance_2){
            closest_distance_2 = edge_distance_2;
            ret = edge_point;
        }
    }
    return ret;
}

vec3 Triangle::ClosestPointToPlane(vec3 center) {
    float dist = face.DistanceTo(center);
    return (face.normal*(-dist)) + center;
}

bool Triangle::PointInTriangle(vec3 point) {		
    /*Using "Same Side technique" from http://blackpawn.com/texts/pointinpoly/ */
    //If the point is inside the correct half of the plane bisected by each edge of the triangle, it's in the triangle.
    return 	SameSide(point,verts[2],verts[0],verts[1]) &&   //
            SameSide(point,verts[0],verts[2],verts[1]) &&   //
            SameSide(point,verts[1],verts[2],verts[0]);     //
}

bool Triangle::IsZeroArea(){
    return(face.normal.length_sqr() < 0.25);
}

bool Triangle::SameSide(vec3 point,vec3 reference,vec3 origin,vec3 axis){
    //Assuming both a point and an axis are coplanar, the cross product of the point and axis will point out of the plane 1 of 2 ways
    // If the cross products of 2 points are facing within 90 degrees (dot product > 0), two points lie on the same side of that plane.
    vec3 A = (axis - origin).normalized();
    vec3 vpoint = (point-origin).normalized();
    vec3 vreference = (reference-origin).normalized();
    
    vec3 point_cross_axis = vpoint.cross(A);
    vec3 reference_cross_axis = vreference.cross(A);
    bool same_side = point_cross_axis.dot(reference_cross_axis) >= 0.0f;
    if(same_side == false){
        return false;
    }
    return same_side;
}

vec3 Triangle::ClosestPointOnEdge(vec3 point,vec3 v1,vec3 v2){
    vec3 P = point - v1; 
    vec3 Edge = v2 - v1;
    vec3 E = Edge.normalized();//unit length vector of line
    
    float paralell_component = P.dot(E);//cosine between the unit vectors * length of P(v1 to point)
    
    if(paralell_component < 0){
        return v1;
    }
    if(paralell_component*paralell_component > Edge.length_sqr()){
        return v2;
    }
        
    return (E * paralell_component) + v1;
}