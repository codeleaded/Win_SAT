// #include "C:/Wichtig/System/Static/Library/WindowEngine1.0.h"
// #include "C:/Wichtig/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"


Vector shapes;
TransformedView tv;

void Setup(AlxWindow* w){
    tv = TransformedView_Make((Vec2){ GetWidth(),GetHeight() },(Vec2){ 0.0f,0.0f },(Vec2){ 1.0f,1.0f },(float)GetWidth() / (float)GetHeight());
    shapes = Vector_New(sizeof(int));
    

}
void Update(AlxWindow* w){
    TransformedView_Output(&tv,(Vec2){ GetWidth(),GetHeight() });
    TransformedView_HandlePanZoom(&tv,window.Strokes,GetMouse());
    Vec2 Mouse = TransformedView_ScreenWorldPos(&tv,GetMouse());

    if(Stroke(ALX_MOUSE_L).PRESSED){
        
    }

    Clear(BLACK);

    //Vec2 BorderP = TransformedView_WorldScreenPos(&tv,Border.p);
    //Vec2 BorderD = TransformedView_WorldScreenLength(&tv,Border.d);
    //RenderRectWire(BorderP.x,BorderP.y,BorderD.x,BorderD.y,WHITE,1.0f);
    
    for(int i = 0;i<shapes.size;i++){
        
    }
}
void Delete(AlxWindow* w){
    for(int i = 0;i<shapes.size;i++){
        
    }
	Vector_Free(&shapes);
}

int main(){
    if(Create("SAT",1920,1080,1,1,Setup,Update,Delete))
        Start();
    return 0;
}