// #include "C:/Wichtig/System/Static/Library/WindowEngine1.0.h"
// #include "C:/Wichtig/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/Geometry.h"

typedef struct ConvexShape {
    Vector points;
    Vector targets;
    Vec2 middle;
    F32 angle;
    Pixel color;
} ConvexShape;

ConvexShape ConvexShape_New(Vec2* points,int count,Vec2 middle,Pixel color){
    ConvexShape cs;
    cs.points = Vector_New(sizeof(Vec2));
    cs.targets = Vector_New(sizeof(Vec2));
    //Vector_PushCount(&cs.points,points,count);
    
    for(int i = 0;i<count;i++)
        Vector_Push(&cs.points,points + i);

    cs.middle = middle;
    cs.angle = 0.0f;
    cs.color = color;
    return cs;
}

char ShapeOverlapSat(ConvexShape* cs1,ConvexShape* cs2) {
	ConvexShape* poly1 = cs1;
	ConvexShape* poly2 = cs2;
	
	for(int shape = 0;shape < 2;shape++) {
		if(shape==1) {
			poly1 = cs2;
			poly2 = cs1;
		}
        
		for(int a = 0;a<poly1->targets.size;a++) {
			int b = (a+1) % poly1->targets.size;

            Vec2 p1 = *(Vec2*)Vector_Get(&poly1->targets,b);
            Vec2 p2 = *(Vec2*)Vector_Get(&poly1->targets,a);

            Vec2 axisProj = Vec2_Perp(Vec2_Sub(p1,p2));
			
			float min_r1 = INFINITY,max_r1 = -INFINITY;
			for(int p = 0;p<poly1->targets.size;p++) {
                Vec2 q1 = *(Vec2*)Vector_Get(&poly1->targets,p);
				float q = Vec2_Dot(q1,axisProj);
				min_r1 = F32_Min(min_r1, q);
				max_r1 = F32_Max(max_r1, q);
			}
			
			float min_r2 = INFINITY, max_r2 = -INFINITY;
			for(int p = 0;p<poly2->targets.size;p++) {
				Vec2 q1 = *(Vec2*)Vector_Get(&poly2->targets,p);
				float q = Vec2_Dot(q1,axisProj);
				min_r2 = F32_Min(min_r2, q);
				max_r2 = F32_Max(max_r2, q);
			}
			
			if(!(max_r2 >= min_r1 && max_r1 >= min_r2))
				return 0;
		}
	}
	return 1;
}
char ShapeOverlapSatStatic(ConvexShape* cs1,ConvexShape* cs2) {
	ConvexShape* poly1 = cs1;
	ConvexShape* poly2 = cs2;
	float overlap = INFINITY;
	
	for(int shape = 0;shape < 2;shape++) {
		if(shape==1) {
			poly1 = cs2;
			poly2 = cs1;
		}
		for(int a = 0;a<poly1->targets.size;a++) {
			int b = (a+1) % poly1->targets.size;

			Vec2 p1 = *(Vec2*)Vector_Get(&poly1->targets,b);
            Vec2 p2 = *(Vec2*)Vector_Get(&poly1->targets,a);

            Vec2 axisProj = Vec2_Perp(Vec2_Sub(p1,p2));
			
			float min_r1 = INFINITY, max_r1 = -INFINITY;
			for(int p = 0;p<poly1->targets.size;p++) {
                Vec2 q1 = *(Vec2*)Vector_Get(&poly1->targets,p);
				float q = Vec2_Dot(q1,axisProj);
				min_r1 = F32_Min(min_r1, q);
				max_r1 = F32_Max(max_r1, q);
			}
			
			float min_r2 = INFINITY, max_r2 = -INFINITY;
			for(int p = 0;p<poly2->targets.size;p++) {
				Vec2 q1 = *(Vec2*)Vector_Get(&poly2->targets,p);
				float q = Vec2_Dot(q1,axisProj);
				min_r2 = F32_Min(min_r2, q);
				max_r2 = F32_Max(max_r2, q);
			}
			
			overlap = F32_Min(F32_Min(max_r1,max_r2) - F32_Max(min_r1,min_r2),overlap);
			
			if(!(max_r2 >= min_r1 && max_r1 >= min_r2))
				return 0;
		}
	}
	Vec2 d = Vec2_Norm(Vec2_Sub(cs2->middle,cs1->middle));
	cs1->middle = Vec2_Sub(cs1->middle,Vec2_Mulf(d,overlap));
	return 0;
}

char ShapeOverlapDia(ConvexShape* cs1,ConvexShape* cs2) {
	ConvexShape* poly1 = cs1;
	ConvexShape* poly2 = cs2;
	
	for(int shape = 0;shape < 2;shape++) {
		if(shape==1) {
			poly1 = cs2;
			poly2 = cs1;
		}
		for(int p = 0;p<poly1->points.size;p++) {
			Vec2 line_r1s = poly1->middle;
			Vec2 line_r1e = *(Vec2*)Vector_Get(&poly1->targets,p);
			
			for(int q = 0;q<poly2->points.size;q++) {
				Vec2 line_r2s = *(Vec2*)Vector_Get(&poly2->targets,q);
				Vec2 line_r2e = *(Vec2*)Vector_Get(&poly2->targets,(q+1) % poly2->points.size);
				
				float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
				float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
				float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;
				
				if(t1>=0.0f&&t1<1.0f&&t2>=0.0f&&t2<1.0f) {
					return 1;
				}
			}
		}
	}
	return 0;
}
char ShapeOverlapDiaStatic(ConvexShape* cs1,ConvexShape* cs2) {
	ConvexShape* poly1 = cs1;
	ConvexShape* poly2 = cs2;
	
	for(int shape = 0;shape < 2;shape++) {
		if(shape==1) {
			poly1 = cs2;
			poly2 = cs1;
		}
		
		for(int p = 0;p<poly1->points.size;p++) {
			Vec2 line_r1s = poly1->middle;
			Vec2 line_r1e = *(Vec2*)Vector_Get(&poly1->targets,p);
			
			Vec2 displace = { 0.0f,0.0f };
			
			for(int q = 0;q<poly2->points.size;q++) {
				Vec2 line_r2s = *(Vec2*)Vector_Get(&poly2->targets,q);
				Vec2 line_r2e = *(Vec2*)Vector_Get(&poly2->targets,(q+1) % poly2->points.size);
				
				float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
				float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
				float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;
				
				if(t1>=0.0f&&t1<1.0f&&t2>=0.0f&&t2<1.0f) {
					displace.x += (1.0f - t1) * (line_r1e.x - line_r1s.x);
					displace.y += (1.0f - t1) * (line_r1e.y - line_r1s.y);
				}
			}
			
			cs1->middle.x += displace.x * (shape==0?-1:1);
			cs1->middle.y += displace.y * (shape==0?-1:1);
		}
	}
	return 0;
}

void ConvexShape_Update(ConvexShape* cs){
    Vector_Clear(&cs->targets);
    M2x2 rot = M2x2_RotateZ(cs->angle);

    for(int i = 0;i<cs->points.size;i++){
		Vec2 p = *(Vec2*)Vector_Get(&cs->points,i);
		Vec2 t = Vec2_Add(cs->middle,M2x2_VecMul(p,rot));
        Vector_Push(&cs->targets,&t);
	}
}
void ConvexShape_Resolve(ConvexShape* cs1,ConvexShape* cs2){
    //ShapeOverlapSatStatic(cs1,cs2);
	ShapeOverlapDiaStatic(cs1,cs2);
}
void ConvexShape_Render(Pixel* Target,int Target_Width,int Target_Height,ConvexShape* cs,TransformedView* tv){
    Vec2 prev = *(Vec2*)Vector_Get(&cs->targets,cs->targets.size - 1);
    for(int i = 0;i<cs->targets.size;i++){
        Vec2 p = *(Vec2*)Vector_Get(&cs->targets,i);

        Vec2 pos = TransformedView_WorldScreenPos(tv,prev);
        Vec2 tar = TransformedView_WorldScreenPos(tv,p);
        RenderLine(pos,tar,cs->color,1.0f);

        prev = p;
    }
}
void ConvexShape_Free(ConvexShape* cs){
    Vector_Free(&cs->points);
    Vector_Free(&cs->targets);
}


int selected;
Vector shapes;
TransformedView tv;

void Setup(AlxWindow* w){
    tv = TransformedView_Make((Vec2){ GetWidth(),GetHeight() },(Vec2){ 0.0f,0.0f },(Vec2){ 1.0f,1.0f },(float)GetWidth() / (float)GetHeight());
    shapes = Vector_New(sizeof(ConvexShape));
    
    Vector_Push(&shapes,(ConvexShape[]){ ConvexShape_New((Vec2[]){ {0.5f,0.0f},{-0.5f,-0.5f},{-0.5f,0.5f} },3,(Vec2){ 1.0f,0.0f },RED) });
    Vector_Push(&shapes,(ConvexShape[]){ ConvexShape_New((Vec2[]){ {0.5f,0.0f},{-0.5f,-0.5f},{-0.5f,0.5f} },3,(Vec2){ 2.0f,0.0f },BLUE) });
    Vector_Push(&shapes,(ConvexShape[]){ ConvexShape_New((Vec2[]){ {0.5f,0.0f},{-0.5f,-0.5f},{-0.5f,0.5f} },3,(Vec2){ 3.0f,0.0f },GREEN) });

    selected = 0;
}
void Update(AlxWindow* w){
    TransformedView_Output(&tv,(Vec2){ GetWidth(),GetHeight() });
    TransformedView_HandlePanZoom(&tv,window.Strokes,GetMouse());
    Vec2 Mouse = TransformedView_ScreenWorldPos(&tv,GetMouse());

    if(Stroke(ALX_KEY_W).PRESSED){
        selected++;
        if(selected>=shapes.size)
            selected = 0;
    }
    if(Stroke(ALX_KEY_S).PRESSED){
        selected--;
        if(selected<0)
            selected = shapes.size - 1;
    }

    if(Stroke(ALX_KEY_LEFT).DOWN){
        ConvexShape* cs = (ConvexShape*)Vector_Get(&shapes,selected);
        cs->angle -= F32_PI * w->ElapsedTime;
    }
    if(Stroke(ALX_KEY_RIGHT).DOWN){
        ConvexShape* cs = (ConvexShape*)Vector_Get(&shapes,selected);
        cs->angle += F32_PI * w->ElapsedTime;
    }

    if(Stroke(ALX_KEY_UP).DOWN){
        ConvexShape* cs = (ConvexShape*)Vector_Get(&shapes,selected);
        Vec2 target = Vec2_Mulf(Vec2_OfAngle(cs->angle),w->ElapsedTime);
        cs->middle = Vec2_Add(cs->middle,target);
    }
    if(Stroke(ALX_KEY_DOWN).DOWN){
        ConvexShape* cs = (ConvexShape*)Vector_Get(&shapes,selected);
        Vec2 target = Vec2_Mulf(Vec2_OfAngle(cs->angle),w->ElapsedTime);
        cs->middle = Vec2_Sub(cs->middle,target);
    }

    if(Stroke(ALX_KEY_R).PRESSED){
        Vector_Push(&shapes,(ConvexShape[]){ ConvexShape_New((Vec2[]){ {0.5f,0.0f},{-0.5f,-0.5f},{-0.5f,0.5f} },3,(Vec2){ 0.0f,0.0f },YELLOW) });
    }
    if(Stroke(ALX_KEY_F).PRESSED){
        ConvexShape* cs = (ConvexShape*)Vector_Get(&shapes,shapes.size - 1);
        ConvexShape_Free(cs);
        Vector_PopTop(&shapes);
    }
    
	for(int i = 0;i<shapes.size;i++){
        ConvexShape* cs = (ConvexShape*)Vector_Get(&shapes,i);
        ConvexShape_Update(cs);
    }

	if(shapes.size>1){
        for(int i = 0;i<shapes.size;i++){
            for(int j = i+1;j<shapes.size;j++){
            	ConvexShape* cs1 = (ConvexShape*)Vector_Get(&shapes,i);
            	ConvexShape* cs2 = (ConvexShape*)Vector_Get(&shapes,j);
            	ConvexShape_Resolve(cs1,cs2);
       		}
        }
    }

    Clear(BLACK);

    for(int i = 0;i<shapes.size;i++){
        ConvexShape* cs = (ConvexShape*)Vector_Get(&shapes,i);
        ConvexShape_Update(cs);
        ConvexShape_Render(WINDOW_STD_ARGS,cs,&tv);
    }
}
void Delete(AlxWindow* w){
    for(int i = 0;i<shapes.size;i++){
        ConvexShape* cs = (ConvexShape*)Vector_Get(&shapes,i);
        ConvexShape_Free(cs);
    }
	Vector_Free(&shapes);
}

int main(){
    if(Create("SAT",1920,1080,1,1,Setup,Update,Delete))
        Start();
    return 0;
}