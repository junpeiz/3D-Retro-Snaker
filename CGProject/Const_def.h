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
#define num_of_galaxy 6
#define galaxy_size 200
#define lantern_size 3

#define change_plane_speed 0.5
#define change_dir_speed 0.7

#define max_body_num 30 

#define obs_num 80

#define PI 3.14159265857

#define window_length 1200
#define window_width 800

#define CHECK(m, n) if (m != n) throw std::invalid_argument("")
#define CHECK_GREAT(m, n) if (m <= n) throw std::invalid_argument("")
#define CHECK_LESS(m, n) if (m >= n) throw std::invalid_argument("")

