#define LengthOfCube 103
#define LengthOfColumn 1.0
#define StartHeadX (LengthOfCube/2+LengthOfColumn/2)
#define StartViewX (LengthOfCube/2+4*LengthOfColumn)

#define HeadCameDistance (1.732051*2*LengthOfColumn*2)
#define RadiusOfCamera (8*LengthOfColumn)
#define NewCamPosUp StartViewX
#define NewCamPosView (StartHeadX+HeadCameDistance)
#define COLLISION_DISTANCE 5
#define j2h(x) (3.1415926*(x)/180.0)

#define head_size 2.0
#define body_size 2.0
#define tail_size 0.5
#define num_of_pic 5

#define max_body_num 100 

#define obs_num 100

#define PI 3.14159265857