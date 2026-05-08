#include <gui.h>

class Transform {
    public:
    Vetor3D pos = Vetor3D(0.0, 0.0, 0.0);
    Vetor3D rot = Vetor3D(0.0, 0.0, 0.0);
    Vetor3D esc = Vetor3D(1.0, 1.0, 1.0);
    bool showLocalOrigin = true;

    void apply();
    void apply2D();
    void updateByMouse();
    void updateByMouse2D();
    void reset();
};
