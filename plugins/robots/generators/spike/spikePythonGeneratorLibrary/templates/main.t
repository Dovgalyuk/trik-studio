import sys
import time
import random
import math
import motor
import motor_pair
import distance_sensor
import color_sensor
import force_sensor
from hub import port, light_matrix, motion_sensor
import runloop
import color

class Program():
  __interpretation_started_timestamp__ = time.time() * 1000

  @@VARIABLES@@

  @@SUBPROGRAMS_FORWARDING@@

  @@THREADS_FORWARDING@@

  @@SUBPROGRAMS@@

  @@THREADS@@

  async def execMain(self):

  @@MAIN_CODE@@

async def main():
  program = Program()
  await program.execMain()

if __name__ == '__main__':
  runloop.run(main())
