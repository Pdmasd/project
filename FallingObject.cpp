#include "FallingObject.h"
#include "defs.h"

FallingObject::FallingObject(float x, float y, int w, int h)
    : GameObject(x, y, w, h),
      desiredPosition(box), // khởi tạo desiredPosition với giá trị hiện tại của box
      vx(0), vy(0),
      ax(0), ay(0),
      targetVx(0),
      inAir(false)
{
    // Không cần cấp phát động vì desiredPosition là thành viên giá trị
}

FallingObject::~FallingObject() {
    // Không cần xóa gì vì không có cấp phát động
}

void FallingObject::preUpdate(float dt) {
    // Cập nhật vị trí mong muốn bằng cách copy box hiện tại
    desiredPosition = box;

    // Reset gia tốc theo trục Y và tốc độ mục tiêu theo trục X mỗi frame
    ay = 0;
    targetVx = 0;

    // Áp dụng trọng lực (sử dụng hằng số từ defs.h)
    vy += GRAVITY_ACCELERATION * dt;

    // Tính toán vị trí mong muốn dựa trên vận tốc
    desiredPosition.x += static_cast<int>(vx * dt);
    desiredPosition.y += static_cast<int>(vy * dt);
    desiredPosition.update();
}

void FallingObject::commitMovement() {
    // Cập nhật vị trí thực tế bằng vị trí mong muốn
    box = desiredPosition;
    box.update();

    // Kiểm tra và xử lý ranh giới
    boundaryStatus = actOnBoundaries();
}
