#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

// AM_DEVREG( 9, GPU_CONFIG,   RD, bool present, has_accel; int width, height, vmemsz); // 111
// AM_DEVREG(10, GPU_STATUS,   RD, bool ready);
// AM_DEVREG(11, GPU_FBDRAW,   WR, int x, y; void *pixels; int w, h; bool sync); // 111
// AM_DEVREG(12, GPU_MEMCPY,   WR, uint32_t dest; void *src; int size);
// AM_DEVREG(13, GPU_RENDER,   WR, uint32_t root);

void __am_gpu_init() {
  // int i;
  // int w = (uint32_t)inw(VGACTL_ADDR + 2);  // TODO: get the correct width
  // int h = (uint32_t)inw(VGACTL_ADDR);  // TODO: get the correct height
  // uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  // for (i = 0; i < w * h; i ++) fb[i] = i;
  // outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  int w = (uint32_t)inw(VGACTL_ADDR + 2);
  int h = (uint32_t)inw(VGACTL_ADDR);
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = w, .height = h,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  // putstr("called __am_gpu_fbdraw\n");
  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  if (!ctl->sync && (w == 0 || h == 0)) return;
  uint32_t *pixels = ctl->pixels;
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  int s_w = (uint32_t)inw(VGACTL_ADDR + 2);
  // int s_h = (uint32_t)inw(VGACTL_ADDR);
  for (int i = y; i < y+h; i++) {
    for (int j = x; j < x+w; j++) {
      fb[s_w*i+j] = pixels[w*(i-y)+(j-x)]; // Row-major order -- refresh
    }
  }
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
