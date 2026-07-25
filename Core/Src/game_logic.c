/*
 * game_logic.c
 *
 *  Created on: Nov 14, 2025
 *      Author: vovchik
 */

#include "game_logic.h"
#include "display.h"
#include "qp.h"
#include "stddef.h"
#include "stdint.h"
#include "stdlib.h"
#include "lcd.h"
#include "main.h"
#include "stratagems_data.h"
#include "qpc.h"



#define INIT_STRATAGEM_NUM  5
#define STRATAGEM_LIST_SIZE 57



typedef struct{
  const char * stratagem_name;
  uint32_t sequence;
}stratagem_data;
extern RNG_HandleTypeDef hrng;
GameDataStruct GameData;

extern QActive *AO_GameData;

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


QState App_initial(GameDataStruct * const me,QEvt const * const e);
QState App_idle(GameDataStruct * const me,QEvt const * const e); //former wait for start
// QState App_waitForStart(GameDataStruct * const me,QEvt const * const e);//deprecated
QState Game_notActive(GameDataStruct * const me,QEvt const * const e);//new hierarchical state for all state when game is not active
QState Game_summary(GameDataStruct * const me,QEvt const * const e);//new state for final round/game info
QState App_gameEnded(GameDataStruct * const me,QEvt const * const e);
QState Game_countdown(GameDataStruct * const me,QEvt const * const e);
QState Game_active(GameDataStruct * const me,QEvt const * const e);
QState Game_roundComplete(GameDataStruct * const me,QEvt const * const e);

// Choice point: decide whether to show the next countdown number or
// start the round. Not a real HSM state - just a guarded transition,
// called from the events that used to transition into it directly
// (transitioning from Q_ENTRY_SIG/Q_INIT_SIG is not allowed by QP).
static QState Game_countdownStep(GameDataStruct * const me){
  QState status;
  if(me->countdown_timer>0){
    DisplayStartCountDownScreen(me->countdown_timer);
    me->countdown_timer--;
    status=Q_TRAN(Game_countdown);
  }
  else{
    uint32_t num;
    for(uint8_t i=0;i<(INIT_STRATAGEM_NUM+GameData.round_num);i++){

      HAL_RNG_GenerateRandomNumber(&hrng, &num);
      num=num%(STRATAGEM_LIST_SIZE-1);
      GameData.stratagems[i].sequence=stratagem_list[num]->sequence;
      //GameData.stratagem_names[i]=stratagem_list[num]->stratagem_name;
    }
    status=Q_TRAN(Game_active);
  }
  return status;
}

void Game_ctor(GameDataStruct * const me){
  QActive_ctor(&me->super, (QStateHandler)&App_initial);
  QTimeEvt_ctorX(&me->countdown_timeout, &me->super, CNTDOWN_TIMEOUT, 0U);
  QTimeEvt_ctorX(&me->game_timeout, &me->super, GAME_TIMEOUT, 0U);
  QTimeEvt_ctorX(&me->idle_timeout, &me->super, IDLE_TIMEOUT, 0U);
  QTimeEvt_ctorX(&me->sw_block_timeout, &me->super, SW_BLOCK_TIMEOUT, 0U);
}

QState App_initial(GameDataStruct * const me, QEvt const * const e){
  Q_UNUSED_PAR(e);
  // тут же можно проинициализировать поля GameData, если нужно
  return Q_TRAN(App_idle);
}

QState Game_summary(GameDataStruct * const me,QEvt const * const e){
  QState status;
  switch(e->sig){
    case IDLE_TIMEOUT: {
      status = Q_TRAN(App_idle);
      break;
    }
    default:{/* game is not active ,only button press processing*/
      status = Q_SUPER(Game_notActive);
    }
  }
  return status;
}

//this state handles only a welcome screen for user to start a game , previous button press is moved to upper state
QState App_idle(GameDataStruct * const me,QEvt const * const e){
  QState status;
  switch(e->sig){
    case Q_ENTRY_SIG: {
      DisplayWaitForStartScreen();
      status = Q_HANDLED();
      break;
    }
    default:{/* game is not active ,only button press processing*/
      status = Q_SUPER(Game_notActive);
    }
  }
  return status;
}

//this state only handles transition to game , since previously countdown was started in 3 different states
QState Game_notActive(GameDataStruct * const me,QEvt const * const e){
  QState status;
  switch(e->sig){
    case ANY_BUTTON_PRESSED:{
      QTimeEvt_disarm(&me->idle_timeout);
      //idle timeout is armed when game is completed and affects game ended / round complete states , 
      // the only possible way out from those states is timeout and button pressed 
      // hence the only place where this timeout should be disarmed
      me->countdown_timer=3;
      status=Game_countdownStep(me);
      break;
    }
    default: {
      status = Q_SUPER(QHsm_top);
      break;
    }
  }
  return status;  
}


QState App_gameEnded(GameDataStruct * const me,QEvt const * const e){
  QState status;
  switch(e->sig){
    case Q_ENTRY_SIG: {
      GameData.round_num=0;
      DisplayFinalRoundInfo(me->user_score);
      status=Q_HANDLED();
      break;
    }
    //default is making a transition to summary state , 
    // both game ended and round completed states displaying its own similar data, 
    // but their timeouts always move to idle
    default:{
      status = Q_SUPER(Game_summary);
    }
  }
  return status;
}

QState Game_countdown(GameDataStruct * const me,QEvt const * const e){
  QState status;
  switch(e->sig){
    case Q_ENTRY_SIG: {
      QTimeEvt_armX(&me->countdown_timeout, 1000, 0);
      status = Q_HANDLED();
      break;
    }
    case CNTDOWN_TIMEOUT:{
      GameData.sequence_cursor=0;
      GameData.sequence_array_cursor=0;
      lcd_clear_buffer();
      status=Game_countdownStep(me);
      break;
    }
    default: {
      status = Q_SUPER(QHsm_top);
      break;
    }
  }
  return status;
}

QState Game_active(GameDataStruct * const me,QEvt const * const e){
  QState status;
  switch(e->sig){
    case Q_ENTRY_SIG: {
      DisplayActiveGameScreen(GameData.stratagems[GameData.sequence_array_cursor].sequence);
      //start game timeout
      QTimeEvt_armX(&me->game_timeout, 10*1000, 0);
      //StartTimeout(GAME_TIMEOUT);
      status = Q_HANDLED();
      break;
    }
    case ANY_BUTTON_PRESSED:{
      if(GameData.last_pressed_button!=((GameData.stratagems[GameData.sequence_array_cursor].sequence>>GameData.sequence_cursor)&0xF)){
        //error in input (i think this may be a good event for this state to process in the next iteration)
        static QEvt const wrong_seq_event=QEVT_INITIALIZER(WRONG_SEQUENCE);
        QACTIVE_POST(AO_GameData,&wrong_seq_event,0);
        status=Q_HANDLED();
        break;
      }
      //update stratagem
      uint8_t new_arrow=ParseInvertedKeysToLcdArrows(GameData.last_pressed_button);
      DisplayNextSequenceArrow(new_arrow,GameData.sequence_cursor/4);
      GameData.input.sequence=new_arrow<<GameData.sequence_cursor;
      GameData.sequence_cursor+=4;
      if(((GameData.stratagems[GameData.sequence_array_cursor].sequence>>GameData.sequence_cursor)&0xF)==0x00){
        //single stratagem is complete
        static QEvt const sequence_completed=QEVT_INITIALIZER(SEQUENCE_COMPLETED);
        QACTIVE_POST(AO_GameData,&sequence_completed,0);
        QTimeEvtCtr remaining=QTimeEvt_getCtr(&me->game_timeout);
        QTimeEvt_rearm(&me->game_timeout, remaining+1000);
      }
      status=Q_HANDLED();
      break;
    }
    case WRONG_SEQUENCE:{
      GameData.input.sequence=0;
      GameData.sequence_cursor=0;
      //ClearStratagemOnDisplay();
      DisplayActiveGameScreen(GameData.stratagems[GameData.sequence_array_cursor].sequence);

      status=Q_HANDLED();
      break;
    }
    case SEQUENCE_COMPLETED:{
      GameData.user_score+=(GameData.sequence_cursor/4);
      GameData.sequence_array_cursor++;   //increment cursor for next stratagem
      GameData.sequence_cursor=0;         //clear cursor of each arrow in sequence
      GameData.input.sequence=0;          //clear previous sequence
      //ClearStratagemOnDisplay();
      DisplayActiveGameScreen(GameData.stratagems[GameData.sequence_array_cursor].sequence);
      if(GameData.sequence_array_cursor==(INIT_STRATAGEM_NUM+GameData.round_num)){
        static QEvt const all_sequences_completed=QEVT_INITIALIZER(ALL_SEQUENCE_COMPLETED);
        QACTIVE_POST(AO_GameData,&all_sequences_completed,0);
      }
      status=Q_HANDLED();
      break;
    }
    case ALL_SEQUENCE_COMPLETED:{
      status=Q_TRAN(Game_roundComplete);
      break;
    }
    case GAME_TIMEOUT:{
      status=Q_TRAN(App_gameEnded);
      break;
    }
    case Q_EXIT_SIG:{
      // must disarm unconditionally: the round can also end early
      // (all stratagems done) while the timeout is still armed
      QTimeEvt_disarm(&me->game_timeout);
      //exit is possible only for round complete and game ended -> both are using idle timeout
      QTimeEvt_armX(&me->idle_timeout, 20000, 0);
      status = Q_HANDLED();
      break;
    }
    default: {
      status = Q_SUPER(QHsm_top);
      break;
    }
  }
  return status;
}

QState Game_roundComplete(GameDataStruct * const me,QEvt const * const e){
  QState status;
  switch(e->sig){
    case Q_ENTRY_SIG: {
      GameData.round_num++;
      DisplayAfterRoundInfo(0,GameData.user_score);
      QTimeEvt_armX(&me->sw_block_timeout, 1000, 0);
      status = Q_HANDLED();
      break;
    }
    case SW_BLOCK_TIMEOUT:{
      GameData.sw_unlock_flag=1;
      status=Q_HANDLED();
      break;
    }
    case ANY_BUTTON_PRESSED:{
      if(GameData.sw_unlock_flag){
        //does this transition here will change state to game summary which moves button pressed event to Game not active state?
        status = Q_SUPER(Game_summary);
        GameData.sw_unlock_flag=0;
      }
      else 
        status=Q_HANDLED();
      break;
    }
    default: {
      status = Q_SUPER(Game_summary);
      break;
    }
  }
  return status;
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
