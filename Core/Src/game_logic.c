/*
 * game_logic.c
 *
 *  Created on: Nov 14, 2025
 *      Author: vovchik
 */

#include "game_logic.h"
#include "display.h"
#include "stddef.h"
#include "stdint.h"
#include "stdlib.h"
#include "lcd.h"
#include "main.h"
#include "stratagems_data.h"

#define TIMEOUTS_COUNT      4
#define INIT_STRATAGEM_NUM  5
#define STRATAGEM_LIST_SIZE 57

typedef struct{
  uint8_t sequence_array_cursor;
  uint8_t sequence_cursor;
  uint8_t last_pressed_button;
  uint8_t user_score;
  uint8_t display_full_stratagem_flag;
  uint8_t sw_unlock_flag;
  uint8_t round_num;
  fourBitSequenceStruct input;
  fourBitSequenceStruct stratagems[15]; //not to say that thats vulnerable , but who tf can go to 10 round and 15 stratagems
  //const char * stratagem_names[5];
}GameDataStruct;

typedef struct{
  const char * stratagem_name;
  uint32_t sequence;
}stratagem_data;


extern RNG_HandleTypeDef hrng;
GameEvents event_queue[255]={0};
uint8_t event_tail=0;
uint8_t event_head=0;
//uint8_t sw_unlock_flag=0;
//uint8_t display_full_stratagem_flag=0;
//uint8_t round_num;
GameSubState GameSubStateMachine=START_COUNTDOWN;
AppStates    AppStateMachine=WAIT_FOR_START;
GameDataStruct GameData;


TimeoutDataStruct CtdownTimeout={.timeout_type=CTDOWN_TIMEOUT,.timespan=3000,.is_active=0};
TimeoutDataStruct GameTimeout={.timeout_type=GAME_TIMEOUT,.timespan=10000,.is_active=0};
TimeoutDataStruct IdleTimeout={.timeout_type=IDLE_TIMEOUT,.timespan=20000,.is_active=0};
TimeoutDataStruct SwBlockTimeout={.timeout_type=SW_BLOCK_TIMEOUT,.timespan=1000,.is_active=0};
TimeoutDataStruct * Timeouts[]={&CtdownTimeout,&GameTimeout,&IdleTimeout,&SwBlockTimeout};

fourBitSequenceStruct REVIVE_STRATAGEM={.sequence=0x14281};//up 1 / down 8 / right 2 / left 4 / up 1
fourBitSequenceStruct RESSUPLY_STRATAGEM={.sequence=0x2188};
fourBitSequenceStruct LASER_CANNON_STRATAGEM={.sequence=0x41848};
fourBitSequenceStruct ORBITAL_LASER_STRATAGEM={.sequence=0x82182};//right down up right down
fourBitSequenceStruct HELLBOMB_STRATAGEM={.sequence=0x81281481};// up down left up down right up down

const stratagem_data _JUMP_PACK_data={.stratagem_name=stratagem_name_lift_850_jump_pack,.sequence=stratagem_code_lift_850_jump_pack};
const stratagem_data _SUPPLY_PACK_data={.stratagem_name=stratagem_name_b_1_supply_pack,.sequence=stratagem_code_b_1_supply_pack};
const stratagem_data _LASER_GUARD_DOG_data={.stratagem_name=stratagem_name_axlas_5_guard_dog_rover,.sequence=stratagem_code_axlas_5_guard_dog_rover};
const stratagem_data _BALLISTIC_SHIELD_data={.stratagem_name=stratagem_name_sh_20_ballistic_shield_backpack,.sequence=stratagem_code_sh_20_ballistic_shield_backpack};
const stratagem_data _GUARD_DOG_data={.stratagem_name=stratagem_name_axar_23_guard_dog,.sequence=stratagem_code_axar_23_guard_dog};
const stratagem_data _MACHINE_GUN_data={.stratagem_name=stratagem_name_mg_43_machine_gun,.sequence=stratagem_code_mg_43_machine_gun};
const stratagem_data _ANTIMATERIAL_RIFLEdata={.stratagem_name=stratagem_name_apw_1_anti_material_rifle,.sequence=stratagem_code_apw_1_anti_material_rifle};
const stratagem_data _STALWART_data={.stratagem_name=stratagem_name_m_105_stalwart,.sequence=stratagem_code_m_105_stalwart};
const stratagem_data _EXP_ANTI_TANK_data={.stratagem_name=stratagem_name_east_17_expendable_anti_tank,.sequence=stratagem_code_east_17_expendable_anti_tank};
const stratagem_data _RECOILESS_RIFLE_data={.stratagem_name=stratagem_name_gr_8_recoiless_rifle,.sequence=stratagem_code_gr_8_recoiless_rifle};
const stratagem_data _FLAMETHROWER_data={.stratagem_name=stratagem_name_flam_40_flamethrower,.sequence=stratagem_code_flam_40_flamethrower};
const stratagem_data _AUTOCANNON_data={.stratagem_name=stratagem_name_ac_8_autocannon,.sequence=stratagem_code_ac_8_autocannon};
const stratagem_data _HEAVY_MACHINE_GUN_data={.stratagem_name=stratagem_name_mg_206_heavy_machine_gun,.sequence=stratagem_code_mg_206_heavy_machine_gun};
const stratagem_data _RAILGUN_data={.stratagem_name=stratagem_name_rs_422_railgun,.sequence=stratagem_code_rs_422_railgun};
const stratagem_data _SPEAR_LAUNCHER_data={.stratagem_name=stratagem_name_faf_14_spear_launcher,.sequence=stratagem_code_faf_14_spear_launcher};
const stratagem_data _GRENADE_LAUNCHER_data={.stratagem_name=stratagem_name_gl_21_grenade_launcher,.sequence=stratagem_code_gl_21_grenade_launcher};
const stratagem_data _LASER_CANNON_data={.stratagem_name=stratagem_name_las_98_laser_cannon,.sequence=stratagem_code_las_98_laser_cannon};
const stratagem_data _ARC_THROWER_data={.stratagem_name=stratagem_name_arc_3_arc_thrower,.sequence=stratagem_code_arc_3_arc_thrower};
const stratagem_data _QUASAR_CANNON_data={.stratagem_name=stratagem_name_las_99_quasar_cannon,.sequence=stratagem_code_las_99_quasar_cannon};
const stratagem_data _PATRIOT_EXOSUIT_data={.stratagem_name=stratagem_name_exo_45_patriot_exosuit,.sequence=stratagem_code_exo_45_patriot_exosuit};
const stratagem_data _REINFORCE_data={.stratagem_name=stratagem_name_reinforce,.sequence=stratagem_code_reinforce};
const stratagem_data _SOS_data={.stratagem_name=stratagem_name_sos_beacon,.sequence=stratagem_code_sos_beacon};
const stratagem_data _RESUPPLY_data={.stratagem_name=stratagem_name_resupply,.sequence=stratagem_code_resupply};
const stratagem_data _HELLBOMB_data={.stratagem_name=stratagem_name_nux_223_hellbomb,.sequence=stratagem_code_nux_223_hellbomb};
const stratagem_data _SSSD_DELIVERY_data={.stratagem_name=stratagem_name_sssd_delivery,.sequence=stratagem_code_sssd_delivery};
const stratagem_data _SEISMIC_PROBE_data={.stratagem_name=stratagem_name_seismic_probe,.sequence=stratagem_code_seismic_probe};
const stratagem_data _UPLOAD_DATA_data={.stratagem_name=stratagem_name_upload_data,.sequence=stratagem_code_upload_data};
const stratagem_data _EAGLE_RE_ARM_data={.stratagem_name=stratagem_name_eagle_re_arm,.sequence=stratagem_code_eagle_re_arm};
const stratagem_data _ILLUMINATION_FLARE_data={.stratagem_name=stratagem_name_illumination_flare,.sequence=stratagem_code_illumination_flare};
const stratagem_data _SEAF_ARTILLERY_data={.stratagem_name=stratagem_name_seaf_artillery,.sequence=stratagem_code_seaf_artillery};
const stratagem_data _EARTH_FLAG_data={.stratagem_name=stratagem_name_super_earth_flag,.sequence=stratagem_code_super_earth_flag};
const stratagem_data _101HMG_EMPLACEMENT_data={.stratagem_name=stratagem_name_emg_101hmg_emplacement,.sequence=stratagem_code_emg_101hmg_emplacement};
const stratagem_data _SHIELD_GEN_data={.stratagem_name=stratagem_name_fx_12_shield_generator_relay,.sequence=stratagem_code_fx_12_shield_generator_relay};
const stratagem_data _ANTIPERSONNEL_MINEFIELD_data={.stratagem_name=stratagem_name_md_6_anti_personnel_minefield,.sequence=stratagem_code_md_6_anti_personnel_minefield};
const stratagem_data _INCENDIARY_MINES_data={.stratagem_name=stratagem_name_md_14_incendiary_mines,.sequence=stratagem_code_md_14_incendiary_mines};
const stratagem_data _AMG43_MACHINE_SENTRY_data={.stratagem_name=stratagem_name_amg_43_machine_sentry,.sequence=stratagem_code_amg_43_machine_sentry};
const stratagem_data _GATLING_SENTRY_data={.stratagem_name=stratagem_name_ag_16_gatling_sentry,.sequence=stratagem_code_ag_16_gatling_sentry};
const stratagem_data _MORTAR_SENTRY_data={.stratagem_name=stratagem_name_am_12_mortar_sentry,.sequence=stratagem_code_am_12_mortar_sentry};
const stratagem_data _ROCKET_SENTRY_data={.stratagem_name=stratagem_name_amls_4x_rocket_sentry,.sequence=stratagem_code_amls_4x_rocket_sentry};
const stratagem_data _EMS_SENTRY_data={.stratagem_name=stratagem_name_am_23_ems_sentry,.sequence=stratagem_code_am_23_ems_sentry};
const stratagem_data _ORB_GATLING_BARRAGE_data={.stratagem_name=stratagem_name_orbital_gatling_barrage,.sequence=stratagem_code_orbital_gatling_barrage};
const stratagem_data _ORB_AIRBURST_STRIKE_data={.stratagem_name=stratagem_name_orbital_airburst_strike,.sequence=stratagem_code_orbital_airburst_strike};
const stratagem_data _120MM_BARRAGE_data={.stratagem_name=stratagem_name_orbital_120mm_he_barrage,.sequence=stratagem_code_orbital_120mm_he_barrage};
const stratagem_data _380MM_BARRAGE_data={.stratagem_name=stratagem_name_orbital_380mm_hs_barrage,.sequence=stratagem_code_orbital_380mm_hs_barrage};
const stratagem_data _ORB_WALKING_BARRAGE_data={.stratagem_name=stratagem_name_orbital_walking_barrage,.sequence=stratagem_code_orbital_walking_barrage};
const stratagem_data _ORB_LASER_data={.stratagem_name=stratagem_name_orbital_laser,.sequence=stratagem_code_orbital_laser};
const stratagem_data _RAIL_CANNON_STRAILE_data={.stratagem_name=stratagem_name_orbital_rail_cannon_strike,.sequence=stratagem_code_orbital_rail_cannon_strike};
const stratagem_data _ORB_PREC_STRIKE_data={.stratagem_name=stratagem_name_orbital_precision_strike,.sequence=stratagem_code_orbital_precision_strike};
const stratagem_data _ORB_EMS_STRIKE_data={.stratagem_name=stratagem_name_orbital_ems_strike,.sequence=stratagem_code_orbital_ems_strike};
const stratagem_data _ORB_SMOKE_STRIKE_data={.stratagem_name=stratagem_name_orbital_smoke_strike,.sequence=stratagem_code_orbital_smoke_strike};
const stratagem_data _EAGLE_STRAFING_RUN_data={.stratagem_name=stratagem_name_eagle_strafing_run,.sequence=stratagem_code_eagle_strafing_run};
const stratagem_data _EAGLE_AIRSTRIKE_data={.stratagem_name=stratagem_name_eagle_airstrike,.sequence=stratagem_code_eagle_airstrike};
const stratagem_data _EAGLE_CLUSTER_BOMB_data={.stratagem_name=stratagem_name_eagle_cluster_bomb,.sequence=stratagem_code_eagle_cluster_bomb};
const stratagem_data _EAGLE_NAPALM_data={.stratagem_name=stratagem_name_eagle_napalm,.sequence=stratagem_code_eagle_napalm};
const stratagem_data _EAGLE_SMOKE_STRIKE_data={.stratagem_name=stratagem_name_eagle_smoke_strike,.sequence=stratagem_code_eagle_smoke_strike};
const stratagem_data _EAGLE_110MM_ROCKETS_data={.stratagem_name=stratagem_name_eagle_110mm_rockets,.sequence=stratagem_code_eagle_110mm_rockets};
const stratagem_data _EAGLE_500KG_BOMB_data={.stratagem_name=stratagem_name_eagle_500kg_bomb,.sequence=stratagem_code_eagle_500kg_bomb};

const stratagem_data * const stratagem_list[]={&_JUMP_PACK_data,&_SUPPLY_PACK_data,&_LASER_GUARD_DOG_data,&_BALLISTIC_SHIELD_data,
                                        &_GUARD_DOG_data,&_MACHINE_GUN_data,&_ANTIMATERIAL_RIFLEdata,&_STALWART_data,
                                        &_EXP_ANTI_TANK_data,&_RECOILESS_RIFLE_data,&_FLAMETHROWER_data,&_AUTOCANNON_data,
                                        &_HEAVY_MACHINE_GUN_data,&_RAILGUN_data,&_SPEAR_LAUNCHER_data,&_GRENADE_LAUNCHER_data,
                                        &_LASER_CANNON_data,&_ARC_THROWER_data,&_QUASAR_CANNON_data,&_PATRIOT_EXOSUIT_data,
                                        &_REINFORCE_data,&_SOS_data,&_RESUPPLY_data,&_HELLBOMB_data,&_SSSD_DELIVERY_data,
                                        &_SEISMIC_PROBE_data,&_UPLOAD_DATA_data,&_EAGLE_RE_ARM_data,&_ILLUMINATION_FLARE_data,
                                        &_SEAF_ARTILLERY_data,&_EARTH_FLAG_data,&_101HMG_EMPLACEMENT_data,&_SHIELD_GEN_data,
                                        &_ANTIPERSONNEL_MINEFIELD_data,&_INCENDIARY_MINES_data,&_AMG43_MACHINE_SENTRY_data,
                                        &_GATLING_SENTRY_data,&_MORTAR_SENTRY_data,&_ROCKET_SENTRY_data,&_EMS_SENTRY_data,
                                        &_ORB_GATLING_BARRAGE_data,&_ORB_AIRBURST_STRIKE_data,&_120MM_BARRAGE_data,&_380MM_BARRAGE_data,
                                        &_ORB_WALKING_BARRAGE_data,&_ORB_LASER_data,&_RAIL_CANNON_STRAILE_data,&_ORB_PREC_STRIKE_data,
                                        &_ORB_EMS_STRIKE_data,&_ORB_SMOKE_STRIKE_data,&_EAGLE_STRAFING_RUN_data,&_EAGLE_AIRSTRIKE_data,
                                        &_EAGLE_CLUSTER_BOMB_data,&_EAGLE_NAPALM_data,&_EAGLE_SMOKE_STRIKE_data,&_EAGLE_110MM_ROCKETS_data,
                                        &_EAGLE_500KG_BOMB_data};
void PlaceEventInQueue(GameEvents event){
//  if(event_tail!=event_head){
    event_queue[event_head]=event;
    if(event_head!=10)
      event_head++;
    else
      event_head=0;
//  }
}

GameEvents GetLastEvent(void){
  if(event_tail!=event_head){
    GameEvents return_event=event_queue[event_tail];
    if(event_tail!=10)
      event_tail++;
    else
      event_tail=0;
    return return_event;
  }
  else
    return NO_EVENT;
}

void AppStateProcessor(void){
  //get last event from queue
  GameEvents last_event=GetLastEvent();
  switch(AppStateMachine){
    case WAIT_FOR_START:
      //check if event is any
      DisplayWaitForStartScreen();
      //display smth (press any key to start)
      if(last_event==ANY_BUTTON_PRESSED){
        GameData.round_num=0;
        //place event GAME_IS_ACTIVE
        AppStateMachine=GAME_IS_ACTIVE;
        StartTimeout(CTDOWN_TIMEOUT);
      }
      break;
    case GAME_IS_ACTIVE:
      GameProcessor(last_event);
      // GameProcessor and
      break;
    case GAME_ENDED:
      if(last_event==IDLE_TIMEOUT){
        AppStateMachine=WAIT_FOR_START;
      }
      if(last_event==SW_BLOCK_TIMEOUT){
        GameData.sw_unlock_flag=1;
      }
      if(last_event==ANY_BUTTON_PRESSED && GameData.sw_unlock_flag){
        StartTimeout(CTDOWN_TIMEOUT);
        GameSubStateMachine=START_COUNTDOWN;
        AppStateMachine=GAME_IS_ACTIVE;
        GameData.sw_unlock_flag=0;
      }
      break;
  }
}

void GameProcessor(GameEvents last_event){
  switch(GameSubStateMachine){
    case START_COUNTDOWN:
      DisplayStartCountDownScreen(1+(((Timeouts[0]->timespan+Timeouts[0]->start_timestamp-HAL_GetTick()))/1000));
      if(last_event==CTDOWN_TIMEOUT){
        GameSubStateMachine=ACTIVE_ROUND;
        GameData.display_full_stratagem_flag=1;
        StartTimeout(GAME_TIMEOUT);
        GameData.sequence_cursor=0;
        GameData.sequence_array_cursor=0;

        //prepare stratagem list
        uint32_t num;
        for(uint8_t i=0;i<(INIT_STRATAGEM_NUM+GameData.round_num);i++){

          HAL_RNG_GenerateRandomNumber(&hrng, &num);
          num=num%(STRATAGEM_LIST_SIZE-1);
          GameData.stratagems[i].sequence=stratagem_list[num]->sequence;
          //GameData.stratagem_names[i]=stratagem_list[num]->stratagem_name;
        }

        lcd_clear_buffer();
      }
      //place event 3..2..1 and display it
      break;
    case ACTIVE_ROUND:
      if(GameData.display_full_stratagem_flag==1){
        DisplayActiveGameScreen(GameData.stratagems[GameData.sequence_array_cursor].sequence);
        GameData.display_full_stratagem_flag=0;
      }
      DisplayInGameTimeout(1+(((Timeouts[1]->timespan+Timeouts[1]->start_timestamp-HAL_GetTick()))/1000));
      if(last_event==ANY_BUTTON_PRESSED){
        //uint8_t result=0;//0x1,0x2,0x4,0x8 only
        if(GameData.last_pressed_button==((GameData.stratagems[GameData.sequence_array_cursor].sequence>>GameData.sequence_cursor)&0xF)){
          //update stratagem
          uint8_t new_arrow=ParseInvertedKeysToLcdArrows(GameData.last_pressed_button);
          DisplayNextSequenceArrow(new_arrow,GameData.sequence_cursor/4);
          GameData.input.sequence=new_arrow<<GameData.sequence_cursor;
          GameData.sequence_cursor+=4;
          if(((GameData.stratagems[GameData.sequence_array_cursor].sequence>>GameData.sequence_cursor)&0xF)==0x00){
            //single stratagem is complete
            Timeouts[1]->start_timestamp+=1000;//-1sec
            GameData.display_full_stratagem_flag=1;
            GameData.user_score+=(GameData.sequence_cursor/4);
            GameData.sequence_array_cursor++;   //increment cursor for next stratagem
            GameData.sequence_cursor=0;         //clear cursor of each arrow in sequence
            GameData.input.sequence=0;          //clear previous sequence
            ClearStratagemOnDisplay();
            if(GameData.sequence_array_cursor==(INIT_STRATAGEM_NUM+GameData.round_num)){
              GameSubStateMachine=ROUND_COMPLETE; //change state
              DisplayAfterRoundInfo(0,GameData.user_score);            //update lcd
              StartTimeout(IDLE_TIMEOUT);         //
              StartTimeout(SW_BLOCK_TIMEOUT);
            }
          }
        }
        else{
          //error in input
          GameData.display_full_stratagem_flag=1;
          GameData.input.sequence=0;
          GameData.sequence_cursor=0;
          ClearStratagemOnDisplay();
          //clear displayed stratagem
        }
      }
      if(last_event==GAME_TIMEOUT){
        StartTimeout(IDLE_TIMEOUT);
        DisplayFinalRoundInfo(GameData.user_score);
        GameData.user_score=0;
        GameSubStateMachine=START_COUNTDOWN;
        AppStateMachine=GAME_ENDED;
        StartTimeout(SW_BLOCK_TIMEOUT);
        GameData.round_num=0;
      }
      break;
    case ROUND_COMPLETE:
      if(last_event==IDLE_TIMEOUT){
        GameSubStateMachine=START_COUNTDOWN;
        AppStateMachine=WAIT_FOR_START;
      }
      if(last_event==SW_BLOCK_TIMEOUT){
        GameData.sw_unlock_flag=1;
      }
      if(last_event==ANY_BUTTON_PRESSED && GameData.sw_unlock_flag){
        StartTimeout(CTDOWN_TIMEOUT);
        GameSubStateMachine=START_COUNTDOWN;
        GameData.round_num++;
        GameData.sw_unlock_flag=0;
      }
      //wait for button - start countdown , start idle
      break;
  }

}


void StartTimeout(GameEvents TimeoutType){
  TimeoutDataStruct * timeout_local=NULL;
  uint8_t i=0;
  while(i<TIMEOUTS_COUNT){
    if(Timeouts[i]->timeout_type==TimeoutType){
      timeout_local=Timeouts[i];
      break;
    }
    else
      i++;
  }
  if(timeout_local==NULL)
    return;
  timeout_local->start_timestamp=HAL_GetTick();
  //timeout_local->stop_timestamp+=timeout_local->start_timestamp;
  timeout_local->is_active=1;
}

void TimeoutProcessor(void){
  for(uint8_t i=0;i<TIMEOUTS_COUNT;i++){
    if((HAL_GetTick()-Timeouts[i]->start_timestamp)>Timeouts[i]->timespan && Timeouts[i]->is_active){
      PlaceEventInQueue(Timeouts[i]->timeout_type);
      Timeouts[i]->is_active=0;
    }
  }
}

uint8_t ParseKeysToLcdArrows(uint8_t result){
  switch(result){
    case KEY_UP_Pin:
      return ARROW_UP;
    case KEY_LEFT_Pin:
      return ARROW_LEFT;
    case KEY_DOWN_Pin:
      return ARROW_DOWN;
    case KEY_RIGHT_Pin:
      return ARROW_RIGHT;
    default:
      return ' ';
  }
}

uint8_t ParseInvertedKeysToLcdArrows(uint8_t result){
  switch(result){
    case KEY_UP_Pin:
      return INV_ARROW_UP;
    case KEY_LEFT_Pin:
      return INV_ARROW_LEFT;
    case KEY_DOWN_Pin:
      return INV_ARROW_DOWN;
    case KEY_RIGHT_Pin:
      return INV_ARROW_RIGHT;
    default:
      return ' ';
  }
}

void UpdateLastPressedKey(uint8_t last_key){
  GameData.last_pressed_button=last_key;
}
