// #include "C:/Wichtig/System/Static/Library/WindowEngine1.0.h"
// #include "C:/Wichtig/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/ConvexShape.h"

int selected;
Vector shapes;
TransformedView tv;

void Setup(AlxWindow* w){
    tv = TransformedView_Make((Vec2){ GetWidth(),GetHeight() },(Vec2){ 0.0f,0.0f },(Vec2){ 1.0f,1.0f },(float)GetWidth() / (float)GetHeight());
    shapes = Vector_New(sizeof(ConvexShape));
    
    Vector_Push(&shapes,(ConvexShape[]){ ConvexShape_New((Vec2[]){ {0.5f,0.0f},{-0.5f,-0.5f},{-0.5f,0.5f} },3,(Vec2){ 1.0f,0.0f }) });
    Vector_Push(&shapes,(ConvexShape[]){ ConvexShape_New((Vec2[]){ {0.5f,0.0f},{-0.5f,-0.5f},{-0.5f,0.5f} },3,(Vec2){ 2.0f,0.0f }) });
    Vector_Push(&shapes,(ConvexShape[]){ ConvexShape_New((Vec2[]){ {0.5f,0.0f},{-0.5f,-0.5f},{-0.5f,0.5f} },3,(Vec2){ 3.0f,0.0f }) });

    selected = -1;
}
void Update(AlxWindow* w){
    TransformedView_Output(&tv,(Vec2){ GetWidth(),GetHeight() });
    TransformedView_HandlePanZoom(&tv,window.Strokes,GetMouse());
    Vec2 Mouse = TransformedView_ScreenWorldPos(&tv,GetMouse());

	if(Stroke(ALX_MOUSE_L).PRESSED){
        selected = -1;
		for(int i = 0;i<shapes.size;i++){
        	ConvexShape* cs = (ConvexShape*)Vector_Get(&shapes,i);
        	ConvexShape_Update(cs);
			if(ConvexShape_Contains(cs,Mouse)){
				selected = i;
				break;
			}
    	}
    }

	if(selected>=0){
		if(Stroke(ALX_MOUSE_L).DOWN){
			ConvexShape* cs = (ConvexShape*)Vector_Get(&shapes,selected);
			cs->middle = Mouse;
    	}

		if(Stroke(ALX_KEY_LEFT).DOWN){
    	    ConvexShape* cs = (ConvexShape*)Vector_Get(&shapes,selected);
    	    float angle = -F32_PI * w->ElapsedTime;

			M2x2 rot = M2x2_RotateZ(angle);
    		for(int i = 0;i<cs->points.size;i++){
				Vec2* p = (Vec2*)Vector_Get(&cs->points,i);
				*p = M2x2_VecMul(*p,rot);
			}
    	}
    	if(Stroke(ALX_KEY_RIGHT).DOWN){
    	    ConvexShape* cs = (ConvexShape*)Vector_Get(&shapes,selected);
    	    float angle = F32_PI * w->ElapsedTime;

			M2x2 rot = M2x2_RotateZ(angle);
    		for(int i = 0;i<cs->points.size;i++){
				Vec2* p = (Vec2*)Vector_Get(&cs->points,i);
				*p = M2x2_VecMul(*p,rot);
			}
    	}
	}
    
    if(Stroke(ALX_KEY_R).PRESSED){
        Vector_Push(&shapes,(ConvexShape[]){ ConvexShape_New((Vec2[]){ {0.5f,0.0f},{-0.5f,-0.5f},{-0.5f,0.5f} },3,(Vec2){ 0.0f,0.0f }) });
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

    for(int i = 0;i<shapes.size;i++){
        for(int j = i+1;j<shapes.size;j++){
        	ConvexShape* cs1 = (ConvexShape*)Vector_Get(&shapes,i);
        	ConvexShape* cs2 = (ConvexShape*)Vector_Get(&shapes,j);
        	ConvexShape_StaticSat(cs1,cs2);
    	}
    }

    Clear(BLACK);

    for(int i = 0;i<shapes.size;i++){
        ConvexShape* cs = (ConvexShape*)Vector_Get(&shapes,i);
        ConvexShape_Update(cs);

		for(int j = 0;j<cs->targets.size;j++){
			Vec2* p = (Vec2*)Vector_Get(&cs->targets,j);
			*p = TransformedView_WorldScreenPos(&tv,*p);
		}
        
		ConvexShape_Render(WINDOW_STD_ARGS,cs,i != selected ? RED : GREEN);
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