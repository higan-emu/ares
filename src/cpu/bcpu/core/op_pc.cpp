void bCPU::op_bcc() {
  switch(status.cycle_pos++) {
  case 1: {
    if(!!regs.p.c)last_cycle();
    rd.l = op_read();
    if(!regs.p.c) {
      aa.w = regs.pc.d + (int8)rd.l;
      regs.pc.w = aa.w;
    } else {
      status.cycle_pos = 0;
    }
    } break;
  case 2: {
    cpu_c6(aa.w);
    } break;
  case 3: {
    last_cycle();
    cpu_io();
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_bcs() {
  switch(status.cycle_pos++) {
  case 1: {
    if(!regs.p.c)last_cycle();
    rd.l = op_read();
    if(regs.p.c) {
      aa.w = regs.pc.d + (int8)rd.l;
      regs.pc.w = aa.w;
    } else {
      status.cycle_pos = 0;
    }
    } break;
  case 2: {
    cpu_c6(aa.w);
    } break;
  case 3: {
    last_cycle();
    cpu_io();
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_bne() {
  switch(status.cycle_pos++) {
  case 1: {
    if(!!regs.p.z)last_cycle();
    rd.l = op_read();
    if(!regs.p.z) {
      aa.w = regs.pc.d + (int8)rd.l;
      regs.pc.w = aa.w;
    } else {
      status.cycle_pos = 0;
    }
    } break;
  case 2: {
    cpu_c6(aa.w);
    } break;
  case 3: {
    last_cycle();
    cpu_io();
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_beq() {
  switch(status.cycle_pos++) {
  case 1: {
    if(!regs.p.z)last_cycle();
    rd.l = op_read();
    if(regs.p.z) {
      aa.w = regs.pc.d + (int8)rd.l;
      regs.pc.w = aa.w;
    } else {
      status.cycle_pos = 0;
    }
    } break;
  case 2: {
    cpu_c6(aa.w);
    } break;
  case 3: {
    last_cycle();
    cpu_io();
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_bpl() {
  switch(status.cycle_pos++) {
  case 1: {
    if(!!regs.p.n)last_cycle();
    rd.l = op_read();
    if(!regs.p.n) {
      aa.w = regs.pc.d + (int8)rd.l;
      regs.pc.w = aa.w;
    } else {
      status.cycle_pos = 0;
    }
    } break;
  case 2: {
    cpu_c6(aa.w);
    } break;
  case 3: {
    last_cycle();
    cpu_io();
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_bmi() {
  switch(status.cycle_pos++) {
  case 1: {
    if(!regs.p.n)last_cycle();
    rd.l = op_read();
    if(regs.p.n) {
      aa.w = regs.pc.d + (int8)rd.l;
      regs.pc.w = aa.w;
    } else {
      status.cycle_pos = 0;
    }
    } break;
  case 2: {
    cpu_c6(aa.w);
    } break;
  case 3: {
    last_cycle();
    cpu_io();
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_bvc() {
  switch(status.cycle_pos++) {
  case 1: {
    if(!!regs.p.v)last_cycle();
    rd.l = op_read();
    if(!regs.p.v) {
      aa.w = regs.pc.d + (int8)rd.l;
      regs.pc.w = aa.w;
    } else {
      status.cycle_pos = 0;
    }
    } break;
  case 2: {
    cpu_c6(aa.w);
    } break;
  case 3: {
    last_cycle();
    cpu_io();
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_bvs() {
  switch(status.cycle_pos++) {
  case 1: {
    if(!regs.p.v)last_cycle();
    rd.l = op_read();
    if(regs.p.v) {
      aa.w = regs.pc.d + (int8)rd.l;
      regs.pc.w = aa.w;
    } else {
      status.cycle_pos = 0;
    }
    } break;
  case 2: {
    cpu_c6(aa.w);
    } break;
  case 3: {
    last_cycle();
    cpu_io();
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_bra() {
  switch(status.cycle_pos++) {
  case 1: {
    rd.l = op_read();
    aa.w = regs.pc.d + (int8)rd.l;
    regs.pc.w = aa.w;
    } break;
  case 2: {
    cpu_c6(aa.w);
    } break;
  case 3: {
    last_cycle();
    cpu_io();
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_brl() {
  switch(status.cycle_pos++) {
  case 1: {
    rd.l = op_read();
    } break;
  case 2: {
    rd.h = op_read();
    } break;
  case 3: {
    last_cycle();
    cpu_io();
    regs.pc.w = regs.pc.d + (int16)rd.w;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_jmp_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    rd.l = op_read();
    } break;
  case 2: {
    last_cycle();
    rd.h = op_read();
    regs.pc.w = rd.w;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_jmp_long() {
  switch(status.cycle_pos++) {
  case 1: {
    rd.l = op_read();
    } break;
  case 2: {
    rd.h = op_read();
    } break;
  case 3: {
    last_cycle();
    rd.b = op_read();
    regs.pc.d = rd.d & 0xffffff;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_jmp_iaddr() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    rd.l = op_read(OPMODE_ADDR, aa.w);
    } break;
  case 4: {
    last_cycle();
    rd.h = op_read(OPMODE_ADDR, aa.w + 1);
    regs.pc.w = rd.w;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_jmp_iaddrx() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    cpu_io();
    } break;
  case 4: {
    rd.l = op_read(OPMODE_PBR, aa.w + regs.x.w);
    } break;
  case 5: {
    last_cycle();
    rd.h = op_read(OPMODE_PBR, aa.w + regs.x.w + 1);
    regs.pc.w = rd.w;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_jmp_iladdr() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    rd.l = op_read(OPMODE_ADDR, aa.w);
    } break;
  case 4: {
    rd.h = op_read(OPMODE_ADDR, aa.w + 1);
    } break;
  case 5: {
    last_cycle();
    rd.b = op_read(OPMODE_ADDR, aa.w + 2);
    regs.pc.d = rd.d & 0xffffff;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_jsr_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    cpu_io();
    } break;
  case 4: {
    regs.pc.w--;
    stack_write(regs.pc.h);
    } break;
  case 5: {
    last_cycle();
    stack_write(regs.pc.l);
    regs.pc.w = aa.w;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_jsr_long() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    stack_write(regs.pc.b);
    } break;
  case 4: {
    cpu_io();
    } break;
  case 5: {
    aa.b = op_read();
    } break;
  case 6: {
    regs.pc.w--;
    stack_write(regs.pc.h);
    } break;
  case 7: {
    last_cycle();
    stack_write(regs.pc.l);
    regs.pc.d = aa.d & 0xffffff;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_jsr_iaddrx() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    stack_write(regs.pc.h);
    } break;
  case 3: {
    stack_write(regs.pc.l);
    } break;
  case 4: {
    aa.h = op_read();
    } break;
  case 5: {
    cpu_io();
    } break;
  case 6: {
    rd.l = op_read(OPMODE_PBR, aa.w + regs.x.w);
    } break;
  case 7: {
    last_cycle();
    rd.h = op_read(OPMODE_PBR, aa.w + regs.x.w + 1);
    regs.pc.w = rd.w;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_rti() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    } break;
  case 2: {
    cpu_io();
    } break;
  case 3: {
    regs.p = stack_read();
    if(regs.e)regs.p |= 0x30;
    if(regs.p.x) {
      regs.x.h = 0x00;
      regs.y.h = 0x00;
    }
    } break;
  case 4: {
    rd.l = stack_read();
    } break;
  case 5: {
    rd.h = stack_read();
    if(regs.e) {
      regs.pc.w = rd.w;
      status.cycle_pos = 0;
    }
    } break;
  case 6: {
    last_cycle();
    rd.b = stack_read();
    regs.pc.d = rd.d & 0xffffff;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_rts() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    } break;
  case 2: {
    cpu_io();
    } break;
  case 3: {
    rd.l = stack_read();
    } break;
  case 4: {
    rd.h = stack_read();
    } break;
  case 5: {
    last_cycle();
    cpu_io();
    regs.pc.w = rd.w;
    regs.pc.w++;
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_rtl() {
  switch(status.cycle_pos++) {
  case 1: {
    cpu_io();
    } break;
  case 2: {
    cpu_io();
    } break;
  case 3: {
    rd.l = stack_read();
    } break;
  case 4: {
    rd.h = stack_read();
    } break;
  case 5: {
    last_cycle();
    rd.b = stack_read();
    regs.pc.d = rd.d & 0xffffff;
    regs.pc.w++;
    status.cycle_pos = 0;
    } break;
  }
}
