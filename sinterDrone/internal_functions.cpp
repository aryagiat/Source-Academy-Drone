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

static inline sinanbox_t wrap_integer(int v) {
  return v >= NANBOX_INTMIN && v <= NANBOX_INTMAX
    ? NANBOX_OFINT(v) : NANBOX_OFFLOAT(v);
}

static inline float nanboxToFloat(sinanbox_t v) {
  if (NANBOX_ISFLOAT(v)) {
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

static sinanbox_t digital_read(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(1);
  int pin = nanboxToUint(argv[0]);
  int result = digitalRead(static_cast<unsigned int>(pin));
  return NANBOX_OFBOOL(result == HIGH);
}

static sinanbox_t digital_write(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(2);
  if (!NANBOX_ISBOOL(argv[1])) {
    sifault(sinter_fault_type);
  }
  int pin = nanboxToUint(argv[0]);
  bool high = NANBOX_BOOL(argv[1]);
  digitalWrite(static_cast<unsigned int>(pin), high ? HIGH : LOW);
  return NANBOX_OFUNDEF();
}

static sinanbox_t pin_mode(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(2);
  static const unsigned int modeArg[] = { INPUT, OUTPUT, INPUT_PULLUP };

  unsigned int pin = nanboxToUint(argv[0]);
  unsigned int mode = nanboxToUint(argv[1]);
  if (mode > sizeof(modeArg)/sizeof(*modeArg)) {
    sifault(sinter_fault_type);
  }

  pinMode(pin, modeArg[mode]);
  return NANBOX_OFUNDEF();
}

static sinanbox_t analog_read(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(1);
  unsigned int pin = nanboxToUint(argv[0]);
  return NANBOX_OFINT(analogRead(pin));
}

static sinanbox_t analog_reference(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(1);
  unsigned int ref = nanboxToUint(argv[0]);
  analogReference(ref);
  return NANBOX_OFUNDEF();
}

static sinanbox_t analog_write(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(2);
  unsigned int pin = nanboxToUint(argv[0]);
  unsigned int value = nanboxToUint(argv[1]);
  analogWrite(pin, value);
  return NANBOX_OFUNDEF();
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

static sinanbox_t attach_interrupt(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  // TODO this needs special interpreter support
  return NANBOX_OFUNDEF();
}

static sinanbox_t detach_interrupt(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(1);
  unsigned int pin = nanboxToUint(argv[0]);
  detachInterrupt(pin);
  return NANBOX_OFUNDEF();
}

static sinanbox_t enable_interrupts(uint8_t argc, sinanbox_t *argv) {
  interrupts();
  return NANBOX_OFUNDEF();
}

static sinanbox_t disable_interrupts(uint8_t argc, sinanbox_t *argv) {
  noInterrupts();
  return NANBOX_OFUNDEF();
}

static sinanbox_t serial_begin(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  // TODO
  return NANBOX_OFUNDEF();
}

static sinanbox_t serial_end(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  // TODO
  return NANBOX_OFUNDEF();
}

static sinanbox_t serial_settimeout(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  // TODO
  return NANBOX_OFUNDEF();
}           
 
static sinanbox_t serial_print(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  // TODO
  return NANBOX_OFUNDEF();
}

static sinanbox_t serial_println(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  // TODO
  return NANBOX_OFUNDEF();
}

static sinanbox_t serial_read(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  // TODO
  return NANBOX_OFUNDEF();
}

static sinanbox_t serial_write(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  // TODO
  return NANBOX_OFUNDEF();
}

static sinanbox_t serial_flush(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  // TODO
  return NANBOX_OFUNDEF();
}

static sinanbox_t drone_pair(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  CoDrone.begin(115200);
  CoDrone.pair(Nearest);
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
  unsigned int duration = nanboxToUint(argv[0]);
  CoDrone.hover(duration);
  return NANBOX_OFUNDEF();
}

static sinanbox_t get_pitch(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  int pitch = CoDrone.getPitch();
  return NANBOX_OFINT(pitch);
}

static sinanbox_t get_roll(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  int roll = CoDrone.getRoll();
  return NANBOX_OFINT(roll);
}

static sinanbox_t get_throttle(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  int throttle = CoDrone.getThrottle();
  return NANBOX_OFINT(throttle);
}

static sinanbox_t get_yaw(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  int yaw = CoDrone.getYaw();
  return NANBOX_OFINT(yaw);
}

static sinanbox_t move(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  CoDrone.move();
  return NANBOX_OFUNDEF();
}

static sinanbox_t move_for(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(1);
  unsigned int duration = nanboxToFloat(argv[0]);
  CoDrone.move(duration);
  return NANBOX_OFUNDEF();
}

static sinanbox_t set_pitch(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(1);
  int pitch = nanboxToInt(argv[0]);
  CoDrone.setPitch(pitch);
  return NANBOX_OFUNDEF();
}

static sinanbox_t set_roll(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(1);
  int roll = nanboxToInt(argv[0]);
  CoDrone.setRoll(roll);
  return NANBOX_OFUNDEF();
}

static sinanbox_t set_throttle(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(1);
  int throttle = nanboxToInt(argv[0]);
  CoDrone.setThrottle(throttle);
  return NANBOX_OFUNDEF();
}

static sinanbox_t set_yaw(uint8_t argc, sinanbox_t *argv) {
  CHECK_ARGS(1);
  int yaw = nanboxToInt(argv[0]);
  CoDrone.setYaw(yaw);
  return NANBOX_OFUNDEF();
}

static sinanbox_t is_flying(uint8_t argc, sinanbox_t *argv) {
  (void) argc; (void) argv;
  return NANBOX_OFBOOL(CoDrone.isFlying());
}

static const sivmfnptr_t internals[] = {
  digital_read,
  digital_write,
  pin_mode,
  analog_read,
  analog_reference,
  analog_write,
  fn_delay,
  fn_micros,
  fn_millis,
  attach_interrupt,
  detach_interrupt,
  enable_interrupts,
  disable_interrupts,
  serial_begin,
  serial_end,
  serial_settimeout,
  serial_print,
  serial_println,
  serial_read,
  serial_write,
  serial_flush,
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
