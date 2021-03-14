#include <Arduino.h>
#include <CoDrone.h>
#include <stdint.h>

#include <sinter/vm.h>
#include <sinter.h>

#define CHECK_ARGS(n) do { \
  if (argc < n) { \
    sifault(sinter_fault_function_arity); \
  } \
} while (0)

/*
 * Configuration of the drone 
   ==============================================
*/

int roll = 0;
int pitch = 0;
int yaw = 0;
int throttle = 0;

/* ============================================== */

static inline sinanbox_t wrap_integer(int v) {
  return v >= NANBOX_INTMIN && v <= NANBOX_INTMAX
    ? NANBOX_OFINT(v) : NANBOX_OFFLOAT(v);
}

static inline float nanboxToFloat(sinanbox_t v) {
  if (NANBOX_ISINT(v)) {
    return static_cast<float>(NANBOX_INT(v));
  } else if (NANBOX_ISFLOAT(v)) {
    return NANBOX_FLOAT(v);
  } else {
    sifault(sinter_fault_type);
  }

  return 0;
}

static inline int nanboxToInt(sinanbox_t v) {
  if (NANBOX_ISINT(v)) {
    return NANBOX_INT(v);
  } else if (NANBOX_ISFLOAT(v)) {
    return static_cast<int>(NANBOX_FLOAT(v));
  } else {
    sifault(sinter_fault_type);
  }

  return 0;
}

static inline unsigned int nanboxToUint(sinanbox_t v) {
  int r = nanboxToInt(v);
  if (r < 0) {
    sifault(sinter_fault_type);
  }
  return static_cast<unsigned int>(r);
}

static sinanbox_t fn_delay(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(1);
  unsigned int ms = nanboxToUint(argv[0]);
  delay(ms);
  return NANBOX_OFUNDEF();
}

static sinanbox_t fn_micros(uint8_t argc, sinanbox_t *argv) {
  return wrap_integer(micros());
}

static sinanbox_t fn_millis(uint8_t argc, sinanbox_t *argv) {
  return wrap_integer(millis());
}
       
static sinanbox_t drone_pair(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  CoDrone.begin();
  return NANBOX_OFUNDEF();
}

static sinanbox_t emergency_stop(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  CoDrone.emergencyStop();
  return NANBOX_OFUNDEF();
}

static sinanbox_t land(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  CoDrone.land();
  return NANBOX_OFUNDEF();
}

static sinanbox_t takeoff(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  CoDrone.takeoff();
  return NANBOX_OFUNDEF();
}

static sinanbox_t hover(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(1);
  float duration = nanboxToFloat(argv[0]);
  CoDrone.hover(duration);
  return NANBOX_OFUNDEF();
}

static sinanbox_t get_pitch(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  return NANBOX_OFINT(pitch);
}

static sinanbox_t get_roll(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  return NANBOX_OFINT(roll);
}

static sinanbox_t get_throttle(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  return NANBOX_OFINT(throttle);
}

static sinanbox_t get_yaw(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  return NANBOX_OFINT(yaw);
}

static sinanbox_t move(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  CoDrone.move(roll, pitch, yaw, throttle);
  return NANBOX_OFUNDEF();
}

static sinanbox_t move_for(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(1);
  float duration = nanboxToFloat(argv[0]);
  CoDrone.move(duration, roll, pitch, yaw, throttle);
  return NANBOX_OFUNDEF();
}

static sinanbox_t move_setting(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(4);
  int roll = nanboxToInt(argv[0]);
  int pitch = nanboxToInt(argv[1]);
  int yaw = nanboxToInt(argv[2]);
  int throttle = nanboxToInt(argv[3]);
  CoDrone.move(roll, pitch, yaw, throttle);
  return NANBOX_OFUNDEF();
}

static sinanbox_t move_setting_for(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(5);
  float duration = nanboxToFloat(argv[0]);
  int roll = nanboxToInt(argv[1]);
  int pitch = nanboxToInt(argv[2]);
  int yaw = nanboxToInt(argv[3]);
  int throttle = nanboxToInt(argv[4]);
  CoDrone.move(duration, roll, pitch, yaw, throttle);
  return NANBOX_OFUNDEF();
}

static sinanbox_t set_pitch(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(1);
  pitch = nanboxToInt(argv[0]);
  return NANBOX_OFUNDEF();
}

static sinanbox_t set_roll(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(1);
  roll = nanboxToInt(argv[0]);
  return NANBOX_OFUNDEF();
}

static sinanbox_t set_throttle(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(1);
  throttle = nanboxToInt(argv[0]);
  return NANBOX_OFUNDEF();
}

static sinanbox_t set_yaw(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(1);
  yaw = nanboxToInt(argv[0]);
  return NANBOX_OFUNDEF();
}

static sinanbox_t is_flying(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  return NANBOX_OFBOOL(CoDrone.isFlying());
}

static const sivmfnptr_t internals[] = {
  fn_delay,
  fn_micros,
  fn_millis,
  drone_pair,
  emergency_stop,
  land,
  takeoff,
  hover,
  get_pitch,
  get_roll,
  get_throttle,
  get_yaw,
  move,
  move_for,
  move_setting,
  move_setting_for,
  set_pitch,
  set_roll,
  set_throttle,
  set_yaw,
  is_flying
};

void setupInternals() {
  sivmfn_vminternals = internals;
  sivmfn_vminternal_count = sizeof(internals)/sizeof(*internals);
}
