
#define VERSION F("0.1")

// Pin Definitions
// TS = Turn Signal
#define LEFT_TS     2
#define RIGHT_TS    3

#define LOW_BEAM    7
#define HIGH_BEAM   8
#define MARKER      9

#define WIPER_HIGH  4
#define WIPER_LOW   5
#define WIPER_PARK  6

// millisecond/period
#define INTERMITTENT_ON_1 12
#define INTERMITTENT_ON_2 12
#define INTERMITTENT_ON_3 12
#define INTERMITTENT_ON_4 12

// millisecond/period
#define INTERMITTENT_OFF_1 20
#define INTERMITTENT_OFF_2 16
#define INTERMITTENT_OFF_3 12
#define INTERMITTENT_OFF_4 8

#define WASHER_PUMP 10

#define WIPER_CONTROL_GRN_BLK A0
#define WIPER_CONTROL_GRN     A1

#define LED_PIN     13

#define LEFT_TS_LIGHT_ON  digitalWrite(LEFT_TS, LOW)
#define LEFT_TS_LIGHT_OFF digitalWrite(LEFT_TS, HIGH)

#define RIGHT_TS_LIGHT_ON  digitalWrite(RIGHT_TS, LOW)
#define RIGHT_TS_LIGHT_OFF digitalWrite(RIGHT_TS, HIGH)

#define WIPER_HIGH_ON  digitalWrite(WIPER_HIGH, LOW)
#define WIPER_HIGH_OFF digitalWrite(WIPER_HIGH, HIGH)

#define WIPER_LOW_ON  digitalWrite(WIPER_LOW, LOW)
#define WIPER_LOW_OFF digitalWrite(WIPER_LOW, HIGH)

#define WIPER_PARK_ON  digitalWrite(WIPER_PARK, LOW)
#define WIPER_PARK_OFF digitalWrite(WIPER_PARK, HIGH)


#define TURN_SIGNAL_YELLOW_IN A2

uint16_t command = 0x0000;

// command mode definitions
// bitmask
#define LEFT_TS_CMD    0
#define RIGHT_TS_CMD   1
#define HAZARD_CMD     2

#define TS_SW_STATE_0  8
#define TS_SW_STATE_1  9

#define MARKER_CMD     4
#define LOW_BEAM_CMD   5
#define HIGH_BEAM_CMD  6
#define FOG_CMD        7

#define _bitset_(x,b) ( x = x |  (1<<b) )
#define _bitrst_(x,b) ( x = x & ~(1<<b) )
#define _is_set_(x,b)  ( x & (1<<b) )

#define LEFT_TS_ON       _bitset_(command, LEFT_TS_CMD)
#define LEFT_TS_OFF      _bitrst_(command, LEFT_TS_CMD)
#define LEFT_TS_IS_ON    _is_set_(command, LEFT_TS_CMD)

#define RIGHT_TS_ON      _bitset_(command, RIGHT_TS_CMD)
#define RIGHT_TS_OFF     _bitrst_(command, RIGHT_TS_CMD)
#define RIGHT_TS_IS_ON   _is_set_(command, RIGHT_TS_CMD)

#define HAZARD_ON        _bitset_(command, HAZARD_CMD)
#define HAZARD_OFF       _bitrst_(command, HAZARD_CMD)
#define HAZARD_IS_ON     _is_set_(command, HAZARD_CMD)

#define MARKER_ON        _bitset_(command, MARKER_CMD)
#define MARKER_OFF       _bitrst_(command, MARKER_CMD)
#define MARKER_IS_ON     _is_set_(command, MARKER_CMD)

#define LOW_BEAM_ON      _bitset_(command, LOW_BEAM_CMD)
#define LOW_BEAM_OFF     _bitrst_(command, LOW_BEAM_CMD)
#define LOW_BEAM_IS_ON   _is_set_(command, LOW_BEAM_CMD)

#define HIGH_BEAM_ON     _bitset_(command, HIGH_BEAM_CMD)
#define HIGH_BEAM_OFF    _bitrst_(command, HIGH_BEAM_CMD)
#define HIGH_BEAM_IS_ON  _is_set_(command, HIGH_BEAM_CMD)

#define FOG_ON           _bitset_(command, FOG_CMD)
#define FOG_OFF          _bitrst_(command, FOG_CMD)
#define FOG_IS_ON        _is_set_(command, FOG_CMD)

#define TS_SW_OFF        _bitrst( command. (TS_SW_STATE_1 | TS_SW_STATE_0) )
#define TS_SW_LEFT       ( TS_SW_OFF; _bitset(command, TS_SW_STATE_0); )
#define TS_SW_RIGHT      ( TS_SW_OFF; _bitset(command, TS_SW_STATE_1); )
 

enum turn_signal_modes {
  TURN_SIGNAL_OFF,
  TURN_SIGNAL_LEFT,
  TURN_SIGNAL_RIGHT
};

// timing information
unsigned long tic_toc_time = 400UL;
unsigned long time_stamp;

struct light_status_t {
  
  bool left_ts;
  bool right_ts;

  bool marker;
  
  bool tictoc;  //phase of turn signal
  
};

struct light_status_t light_status = {false, false, false, false};

struct wiper_status_t {
  
  bool park;
  bool high;
  bool low;

  uint8_t state;
  uint8_t count;

  uint8_t intermittent_on_count;
  uint8_t intermittent_off_count;
  
  uint16_t grn_blk;
  uint16_t grn;
  uint8_t  sw_mode;
  
};

void init_wiper() {
  
  WIPER_HIGH_OFF;
  WIPER_LOW_OFF;
  WIPER_PARK_OFF;

}
void set_wiper_mode(wiper_status_t *ws)
{
  
  uint8_t switch_mode = ws->sw_mode;
  
  switch(ws->state) {
    case 0:
      WIPER_HIGH_OFF;
      WIPER_LOW_OFF;
      WIPER_PARK_OFF;
      if(switch_mode == 1) ws->state = 1;
      else if(switch_mode == 2) ws->state = 2;
      else if(switch_mode == 3) ws->state = 3;
      else if(switch_mode == 4) ws->state = 4;
      else if(switch_mode == 5) ws->state = 5;
      else if(switch_mode == 6) ws->state = 6;
      break;
      
    case 1:
      ws->intermittent_on_count = INTERMITTENT_ON_1;
      ws->intermittent_off_count = INTERMITTENT_OFF_1;
      ws->state = 20;
      break;
    case 2:
      ws->intermittent_on_count = INTERMITTENT_ON_2;
      ws->intermittent_off_count = INTERMITTENT_OFF_2;
      ws->state = 20;
      break;
    case 3:
      ws->intermittent_on_count = INTERMITTENT_ON_3;
      ws->intermittent_off_count = INTERMITTENT_OFF_3;
      ws->state = 20;
      break; 
    case 4:
      ws->intermittent_on_count = INTERMITTENT_ON_4;
      ws->intermittent_off_count = INTERMITTENT_OFF_4;
      ws->state = 20;
      break;
    case 5:  // low speed on
      WIPER_HIGH_OFF;
      WIPER_LOW_ON;
      WIPER_PARK_OFF;
      if(switch_mode == 0) ws->state = 10;
      else if(switch_mode == 1) ws->state = 1;
      else if(switch_mode == 2) ws->state = 2;
      else if(switch_mode == 3) ws->state = 3;
      else if(switch_mode == 4) ws->state = 4;
      else if(switch_mode == 5) ws->state = 5;
      else if(switch_mode == 6) ws->state = 6;

      break;
      
    case 10:  // park delay
      WIPER_HIGH_OFF;
      WIPER_LOW_OFF;
      WIPER_PARK_ON;
      if(ws->count >= 10) {
        ws->state = 0;
        ws->count = 0;
      }
      else {
        ws->count++;
      }
      break;
  
    case 6:  // high speed
      WIPER_HIGH_ON;
      WIPER_LOW_OFF;
      WIPER_PARK_OFF;
      if(switch_mode == 0) ws->state = 10;
      else if(switch_mode == 1) ws->state = 1;
      else if(switch_mode == 2) ws->state = 2;
      else if(switch_mode == 3) ws->state = 3;
      else if(switch_mode == 4) ws->state = 4;
      else if(switch_mode == 5) ws->state = 5;
      break;
    case 20: // all intermittent modes start here
      WIPER_HIGH_OFF;
      WIPER_LOW_OFF;
      WIPER_PARK_OFF;
      ws->state = 21;
      break;
    case 21:  // intermittent modes
      WIPER_HIGH_OFF;
      WIPER_LOW_ON;
      WIPER_PARK_OFF;
      if(ws->intermittent_on_count > 0) {
      
        ws->intermittent_on_count--;
        
      }
      else {
        ws->state = 22;
      }
      break;
    case 22: // intermittent mode park and wait
      WIPER_HIGH_OFF;
      WIPER_LOW_OFF;
      WIPER_PARK_ON;
      if(ws->intermittent_off_count > 0) {
        
        ws->intermittent_off_count--;
      
      }
      else {
        ws->state = 0;  // check switch position.
      }
      
      //  if non-intermittent mode selected immediately start
      if(switch_mode == 5)      ws->state = 5;
      else if(switch_mode == 6) ws->state = 6;
      break;
    default:
      ws->state = 0;
      break;
    
  }

}

struct wiper_status_t wiper_status = {false, false, false, 0, 0, 0, 0, 0, 0};

uint8_t virtual_wiper_control_mode = 0;
//bool    virtual_wiper_control_on = true;
bool    virtual_wiper_control_on = false;
  
void setup() {

  Serial.begin(115200);
  Serial.println("Light Control Module");

  pinMode(LEFT_TS, OUTPUT);
  pinMode(RIGHT_TS, OUTPUT);

  pinMode(WIPER_HIGH, OUTPUT);
  pinMode(WIPER_LOW, OUTPUT);
  pinMode(WIPER_PARK, OUTPUT);
  
  pinMode(LED_PIN, OUTPUT);
  
  LEFT_TS_LIGHT_OFF;
  RIGHT_TS_LIGHT_OFF;

  init_wiper();
  
  digitalWrite(LED_PIN, HIGH);
  
  //get a time stamp
  time_stamp = millis();
  
}

void loop() {

  unsigned long current = millis();
  unsigned long dt = current - time_stamp;
  
  //uint16_t grn_blk = 0;
  //uint16_t grn = 0;
  //uint8_t sw_mode = 0;
   
  if(dt > tic_toc_time) {
    
    //renew time stamp
    time_stamp = current;

    // update lights
    
    if(light_status.left_ts)
      light_status.tictoc ? LEFT_TS_LIGHT_ON : LEFT_TS_LIGHT_OFF;

    else {
      if(light_status.marker) LEFT_TS_LIGHT_ON;
      else LEFT_TS_LIGHT_OFF;  
    }

    if(light_status.right_ts)
      light_status.tictoc ? RIGHT_TS_LIGHT_ON : RIGHT_TS_LIGHT_OFF;
     
    else {
      if(light_status.marker) RIGHT_TS_LIGHT_ON;
      else RIGHT_TS_LIGHT_OFF;   
    }
    
    if(light_status.tictoc) {
      digitalWrite(LED_PIN, HIGH);
    }
    else {
      digitalWrite(LED_PIN, LOW); 
    }
    // update phase of light
    light_status.tictoc = !light_status.tictoc;


    // wiper 
    wiper_status.grn_blk = analogRead(WIPER_CONTROL_GRN_BLK);
    wiper_status.grn     = analogRead(WIPER_CONTROL_GRN);

    if(virtual_wiper_control_on == false)
      wiper_status.sw_mode = decode_wiper_mode(wiper_status.grn_blk, wiper_status.grn);
    else
      wiper_status.sw_mode = virtual_wiper_control_mode;

      
    set_wiper_mode(&wiper_status);

    // turn signal control stick mode
    uint16_t yellow_read = analogRead(TURN_SIGNAL_YELLOW_IN);
    enum turn_signal_modes ts_stick_position =  decode_turn_signal_mode(yellow_read);
    Serial.print("Yellow: ");
    Serial.println(ts_stick_position);
    
    if(ts_stick_position == TURN_SIGNAL_OFF) {
        LEFT_TS_OFF;
        RIGHT_TS_OFF;
    }
    else if(ts_stick_position == TURN_SIGNAL_LEFT) {
        LEFT_TS_ON;
        RIGHT_TS_OFF;
    }
    else if(ts_stick_position == TURN_SIGNAL_RIGHT) {
        LEFT_TS_OFF;
        RIGHT_TS_ON;
    }  
    
  }// tictoc

  int input = 0;
  if(Serial.available() > 0) {
    
      input = Serial.read();
      
  }
  if(input) {
    
    switch(input) {
      
      case 'L':
        LEFT_TS_ON;
        RIGHT_TS_OFF;
        break;
      case 'l':
        LEFT_TS_OFF;
        break;
      case 'R':
        RIGHT_TS_ON;
        LEFT_TS_OFF;
        break;
      case 'r':
        RIGHT_TS_OFF;
        break;
      case 'M':
        MARKER_ON;
        break;
      case 'm':
        MARKER_OFF;
        break;  
      case 'H':
        HAZARD_ON;
        break;
      case 'h':
        HAZARD_OFF;
        break;
      case 'v':
        Serial.println(VERSION);
        break;
      case 'p':
        Serial.print(F("state : "));
        Serial.println(wiper_status.state);
        Serial.println(wiper_status.sw_mode);
        Serial.println(wiper_status.grn_blk);
        Serial.println(wiper_status.grn);
        break;
      case 'w':
        virtual_wiper_control_on = false;
        break;
      case 'W':
        virtual_wiper_control_on = true;
        break;  
      case '0':
        virtual_wiper_control_mode = 0;
        break;
      case '1':
        virtual_wiper_control_mode = 1;
        break;
      case '2':
        virtual_wiper_control_mode = 2;
        break;
      case '3':
        virtual_wiper_control_mode = 3;
        break;
      case '4':
        virtual_wiper_control_mode = 4;
        break;
      case '5':
        virtual_wiper_control_mode = 5;
        break;
      case '6':
        virtual_wiper_control_mode = 6;
        break;

      default:
        break;
        
    }  
    
  }

  // logic
  if(HAZARD_IS_ON) {
    light_status.left_ts  = true;
    light_status.right_ts = true;   
  }
  else {
  
    if(LEFT_TS_IS_ON) light_status.left_ts  = true;
    else light_status.left_ts = false;
    
    if(RIGHT_TS_IS_ON) light_status.right_ts = true;
    else light_status.right_ts = false;
    
  }

  if(MARKER_IS_ON) {
    light_status.marker = true;
  }
  else {
    light_status.marker = false;
  }

  
}

// grn_blk has a different resistor to ground(white) for each position
// grn is either open or ground
// position        grn          grn_blk      240ohm ratio
// =======================================================
//    0            OPEN          OPEN        6/6
//    1            OPEN          1200        5/6 
//    2            OPEN           480        4/6
//    3            OPEN           240        3/6  
//    4            OPEN           120        2/6
//    5            OPEN            48        1/6
//    6             GND          OPEN        6/6
//
//  Decision values are based on a 240 ohm pull up
  
uint8_t decode_wiper_mode(uint16_t grn_blk_value, uint16_t grn_value) 
{

  uint16_t mode[6] = {938,767,597,426,256,85};
  uint8_t i;
  
  for(i=0;i<6;i++) {
    
    if(grn_blk_value > mode[i]) break;
    
  }

  if(i==0 && grn_value < 256) i = 6;
  
  return i;

}

// yellow connects 910 ohm resistor to ground for left ts
//                 310 ohm to gnd for right ts
//                 open for off
//                 adc values for 500 ohm upper resistor divider

enum turn_signal_modes decode_turn_signal_mode(uint16_t yellow_value)
{
  uint16_t mode[3] = {841, 526, 196};
  enum turn_signal_modes i = TURN_SIGNAL_OFF;

  if(yellow_value > mode[0]) i = TURN_SIGNAL_OFF;

  else if(yellow_value > mode[1]) i = TURN_SIGNAL_LEFT;

  else if(yellow_value > mode[2]) i = TURN_SIGNAL_RIGHT;

  return i;
}
