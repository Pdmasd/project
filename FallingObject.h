#ifndef FALLINGOBJECT_H_DEFINED
#define FALLINGOBJECT_H_DEFINED

#include "gameObject.h"

/// Lớp FallingObject, đại diện cho đối tượng rơi trong game.
/// Đối tượng này có trạng thái vận tốc, gia tốc và có thể xác định vị trí mong muốn sau mỗi frame.
class FallingObject : public GameObject {
public:
    /// Constructor khởi tạo với vị trí (x, y) và kích thước (w, h).
    FallingObject(float x, float y, int w, int h);

    /// Destructor ảo.
    virtual ~FallingObject();

    /// Cập nhật trạng thái (ví dụ: gia tốc, vận tốc) dựa trên delta time.
    void preUpdate(float dt);

    /// Thực hiện di chuyển đối tượng theo trạng thái đã cập nhật.
    void commitMovement();

    /// Vị trí (bounding box) mà đối tượng mong muốn đạt được sau mỗi frame.
    /// GameState sẽ kiểm tra và xác định xem có cho phép di chuyển này hay không.
    Rectangle desiredPosition;

protected:
    float vx;       ///< Thành phần tốc độ theo trục X.
    float vy;       ///< Thành phần tốc độ theo trục Y.
    float ax;       ///< Gia tốc hiện tại theo trục X.
    float ay;       ///< Gia tốc hiện tại theo trục Y.
    float targetVx; ///< Tốc độ mong muốn trên trục X của frame hiện tại.
    bool  inAir;    ///< Cờ xác định đối tượng đang trong không khí (nhảy hoặc rơi).
};

#endif // FALLINGOBJECT_H_DEFINED

