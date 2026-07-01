export module MathTypes;

import <math>;
import Logger;

//TODO- just name "Gradient", or maybe another math term if one matches
export struct LinearGradient
{
    float a, b;

    LinearGradient():a(0),b(0){}
    LinearGradient(float v1, float v2):a(v1),b(v2){}
    // returns a number that represents where value is on the gradient. 0 to 1.0 lies on gradient, negative and results > 1.0 lie outside.
    float Percent(float value) const {
        float min = (a < b)? a:b;
        float dist = (a < b)? b-min:a-min;
        return ((value-min)/dist);
    }
    //  returns a float that's located in the same percentile of the supplied gradient as {value} is in this one.
    float ScaleTo(float value, LinearGradient g2) const {
        return g2.Value(Percent(value));
    }
    // returns the value of {percent} percentile in this gradient (0.0 being 100% value "A", 1.0 being 100% value "B")
    float Value(float percent) const {
        return a*percent+b*(1.0f-percent);
    }
};

//TODO- just name "Range" or maybe another math term if one matches
// LinearGradient where min always < max and min != max
export struct FloatRange
{
    float min;
    float max;

    FloatRange():min(0),max(0){};
    FloatRange(float a,float b) {
        min = (a < b)? a:b;
        max = (a < b)? b:a;
        if(a==b){
            logger::exception("Invalid float range: [ %f , %f ] - Range cannot be empty.\n",a,b);
            a=0.0f;b=1.0f;
        }
    }
    float Clamp(float f){
        return (f<min)?min:((f>max)?max:f);
    }
    float Average(){
        return (min+max)/2.0f;
    }
    bool Contains(float f){return(f >= min && f <= max);}
    bool Overlaps(FloatRange range){
        if(min >= range.min && min <= range.max)return true;
        if(max >= range.min && max <= range.max)return true;
        if(range.min >= min && range.min <= max)return true;
        if(range.max >= min && range.min <= max)return true;
        return false;
    }
    FloatRange Clamp(FloatRange range){
        return {(range.min < min)?range.min:min,(range.max > max)?range.max:max};
    }
    FloatRange Union(FloatRange r2){
        return FloatRange( (min < r2.min)?min:r2.min , (max > r2.max)?max:r2.max );
    }
    //See LinearGradient.ScaleTo
    float ScaleTo(float f1, FloatRange r2){
        float f0 = (f1-r2.min)/(r2.max-r2.min);
        return f0*(max-min) + min;
    }
    //See LinearGradient.ScaleTo
    float ScaleTo(float f1, LinearGradient g1){
        float f0 = (f1-g1.a)/(g1.b-g1.a);
        return f0*(max-min) + min;
    }
    //Average value of the overlapping area between this and range r2
    float Overlap_Center(FloatRange r2){
        if(Contains(r2.min)){
            if(Contains(r2.max)){return r2.Average();}
            return (r2.min + max) /2;
        }
        if(r2.Contains(min)){
            if(r2.Contains(max)){return Average();}
            return (min + r2.max) /2;
        }
        return (Average() + r2.Average()) / 2;
    }
};

export struct AABB
{
    vec3 lo_corner;
    vec3 hi_corner;

    float North() { return hi_corner.z; }
    float North(vec3 offset) { return hi_corner.z + offset.z; }
    void SetNorth(float top)
    {
        hi_corner.z = top;
        if (lo_corner.z > hi_corner.z)
        {
            lo_corner.z = hi_corner.z;
        }
    }

    float South() { return lo_corner.z; }
    float South(vec3 offset) { return lo_corner.z + offset.z; }
    void SetSouth(float bottom)
    {
        lo_corner.z = bottom;
        if (hi_corner.z < lo_corner.z)
        {
            hi_corner.z = lo_corner.z;
        }
    }

    float East() { return hi_corner.x; }
    float East(vec3 offset) { return hi_corner.x + offset.x; }
    void SetEast(float east)
    {
        hi_corner.x = east;
        if (lo_corner.x > hi_corner.x)
        {
            lo_corner.x = hi_corner.x;
        }
    }

    float West() { return lo_corner.x; }
    float West(vec3 offset) { return lo_corner.x + offset.x; }
    void SetWest(float west)
    {
        lo_corner.x = west;
        if (hi_corner.x < lo_corner.x)
        {
            hi_corner.x = lo_corner.x;
        }
    }

    float Top() { return hi_corner.y; }
    float Top(vec3 offset) { return hi_corner.y + offset.y; }
    void SetTop(float top)
    {
        hi_corner.y = top;
        if (lo_corner.y > hi_corner.y)
        {
            lo_corner.y = hi_corner.y;
        }
    }

    float Bottom() { return lo_corner.y; }
    float Bottom(vec3 offset) { return lo_corner.y + offset.y; }
    void SetBottom(float bottom)
    {
        lo_corner.y = bottom;
        if (hi_corner.y < lo_corner.y)
        {
            hi_corner.y = lo_corner.y;
        }
    }

    void Union(AABB box2){
        if(box2.East() > East()){hi_corner.x = box2.hi_corner.x;}
        if(box2.Top() > Top()){hi_corner.y = box2.hi_corner.y;}
        if(box2.North() > North()){hi_corner.z = box2.hi_corner.z;}
        if(box2.West() < West()){lo_corner.x = box2.lo_corner.x;}
        if(box2.Bottom() < Bottom()){lo_corner.y = box2.lo_corner.y;}
        if(box2.South() < South()){lo_corner.z = box2.lo_corner.z;}
    }
    void Move(vec3 offset){
        lo_corner = lo_corner+offset;
        hi_corner = hi_corner+offset;
    }
    bool PointInside(vec3 point){
        float T = Top();
        float B = Bottom();
        float N = North();
        float S = South();
        float E = East();
        float W = West();

        if(	point.x > E &&  point.x < W &&
            point.y > B &&	point.y < T &&
            point.z > S &&	point.z < N) {
            return true;
            }

        return false;
    }
    //Technically this is just a box V AABB check, TODO: actual axis-aligned ellipse math
    bool EllipseInside(vec3 point, vec3 ellipse){
        float T = Top();
        float B = Bottom();
        float N = North();
        float S = South();
        float E = East();
        float W = West();

        if(	point.x+ellipse.x > W &&	point.x-ellipse.x < E &&
            point.y+ellipse.y > B &&	point.y-ellipse.y < T &&
            point.z+ellipse.z > S &&	point.z-ellipse.z < N) {
            return true;
            }
        return false;
    }
    bool ContainsCircle_XZ(vec3 position, float width){
        float N = North();
        float S = South();
        float E = East();
        float W = West();

        if(	position.x+width >= W &&	position.x-width <= E &&
            position.z+width >= S &&	position.z-width <= N) {
            return true;
            }
        return false;
    }
    vec3 Center(){
        return {(hi_corner.x+lo_corner.x / 2.0f),(hi_corner.y+lo_corner.y / 2.0f),(hi_corner.z+lo_corner.z / 2.0f)};
    }
    //Expand this AABB if required for {point} to exist inside of it
    void EncompassPoint(vec3 point){
        if(!PointInside(point)){
            if(point.x > East()){SetEast(point.x);}
            if(point.x < West()){SetWest(point.x);}
            if(point.y > Top()){SetTop(point.y);}
            if(point.y < Bottom()){SetBottom(point.y);}
            if(point.z > North()){SetNorth(point.z);}
            if(point.z < South()){SetSouth(point.z);}
        }
    }
};

export struct Plane
{
    vec3 normal;
    float distance;

    Plane():normal({0,1,0}),distance(0){}
    Plane(float a, float b, float c, float d):normal({a,b,c}),distance(d){normal = normal.normalized();}
    Plane(vec3 tangent_point,vec3 normal_vec) {SetFromPointNormal(tangent_point,normal_vec);}
    void SetFromPointNormal(vec3 tangent_point, vec3 normal_vec){
        normal = normal_vec.normalized();
        distance = -(normal.x*tangent_point.x + normal.y*tangent_point.y + normal.z*tangent_point.z);
    }
    float DistanceTo(vec3 pos){
        return normal.x*pos.x + normal.y*pos.y + normal.z*pos.z + distance;
    }
    void Invert(){
        distance *= -1;
        normal.x *= -1;
        normal.y *= -1;
        normal.z *= -1;
    }
    float YIntersect(float x, float z){
        if(normal.y*normal.y == 0){
            return NAN;
        }
        return (normal.x*X + normal.z*Z + distance)/-normal.y;
    }
};