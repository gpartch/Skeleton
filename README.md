# Skeleton Model 
SMPL Research Project
Gabrielle Partch 
Jan/Feb 2025

Poseable skeleton model using SKEL object files
Object PLY files can be obtained from the [SKEL website](https://skel.is.tue.mpg.de/download.php)

## Controls
- `1` - move skeleton in -x direction
- `2` - move skeleton in +x direction
- `3` - move skeleton in -y direction
- `4` - move skeleton in +y direction
- `5` - move skeleton in -z direction
- `6` - move skeleton in +z direction

- `UP-ARROW` - move bone latitudinal angle up
- `DOWN-ARROW` - move bone latitudinal angle down
- `LEFT-ARROW` - move bone longitudinal angle up
- `RIGHT-ARROW` - move bone longitudinal angle up

- `A` - pelvis + tailbone + lower spine (not moveable)
- `B` - torso + thorax + mid-spine
- `C` - head + neck/ upper spine
- `D` - left scapula/ shoulder blade (not moveable)
- `E` - left humerus/ upper arm
- `F` - left ulna/ lower arm (not moveable - moves with left radius)
- `G` - left radius/ lower arm
- `H` - left hand
- `I` - right scapula/ shoulder blade (not moveable)
- `J` - right humerus/ upper arm
- `K` - right ulna/ lower arm (not moveable - moves with right radius)
- `L` - right radius/ lower arm
- `M` - right hand
- `N` - left femur/ upper leg
- `O` - left fibula + tibia/ lower leg
- `P` - left talus/ ball of foot
- `Q` - left tarsals + metatarsals/ foot
- `R` - left phalanges/ toes
- `S` - right femur/ upper leg
- `T` - right fibula + tibia/ lower leg
- `U` - right talus/ ball of foot
- `V` - right tarsals + metatarsals/ foot 
- `W` - right phalanges/ toes

- `MOUSE LEFT CLICK + MOVE` - adjust viewing angle
- `MOUSE SCROLL IN/OUT` - adjust zoom

- `ESC` - exit
- `x` - toggle axes

---

TO-DO:
- implement joint angle reset
- try to find lower spine + knees
- clean up/ remove any unecessary comments/ code

