//
// treasure; a derived class of walker
//

//#include "graph.h"
#include <math.h>
#include "obmap.h"
#include "loader.h"
#include "screen.h"
#include "treasure.h"
#include "text.h"
#include "stats.h"
#include "guy.h"

treasure::treasure(unsigned char  *data, screen  *myscreen) : walker(data, myscreen)
{
  ignore =(char) 0;
  dead =  (char) 0;
}

treasure::~treasure()
{
  walker::~walker();
}

short treasure::act()
{
  // Abort all later code for now ..
  return 1;
}

short treasure::eat_me(walker  * eater)
{
  short guys_here;
  oblink *here;
  static text eattext(screenp);
  char message[80];
  char * eatkeys;
  static short dumbcount = 0;
  long distance;
  walker  *target, *flash;
  static char exitname[40];
  long leftside, rightside;

  switch (family)
  {
         case FAMILY_DRUMSTICK:
                if (eater->stats->hitpoints >= eater->stats->max_hitpoints)
                  return 1;
                else
                {
                  eater->stats->hitpoints += (short) ((10*stats->level) + (short) random((short) 10*stats->level));
                  if (eater->stats->hitpoints > eater->stats->max_hitpoints)
                         eater->stats->hitpoints = eater->stats->max_hitpoints;
                  dead = 1;
                  //#ifdef REGISTERED
                    if (on_screen())
                      screenp->soundp->play_sound(SOUND_EAT);
                  //#endif
                  return 1;
                }
         case FAMILY_GOLD_BAR:
                if (eater->team_num == 0 || eater->myguy)
                {
                  myscreen->m_score[eater->team_num] += (200*stats->level);
                  dead = 1;
                  //#ifdef REGISTERED
                    if (on_screen())
                      screenp->soundp->play_sound(SOUND_MONEY);
                  //#endif
                }
                return 1;
         case FAMILY_SILVER_BAR:
                if (eater->team_num == 0 || eater->myguy)
                {
                  myscreen->m_score[eater->team_num] += (50*stats->level);
                  dead = 1;
                  //#ifdef REGISTERED
                    if (on_screen())
                      screenp->soundp->play_sound(SOUND_MONEY);
                  //#endif
                }
                return 1;
         case FAMILY_FLIGHT_POTION:
                if (!eater->stats->query_bit_flags(BIT_FLYING) )
                {
                  eater->flight_left += (150*stats->level);
                  if (eater->user != -1)
                  {
                    sprintf(message, "Potion of Flight(%d)!", stats->level);
                    screenp->do_notify(message, eater);
                  }
                  dead = 1;
                }
                return 1;
         case FAMILY_MAGIC_POTION:
                if (eater->stats->magicpoints < eater->stats->max_magicpoints)
                  eater->stats->magicpoints = eater->stats->max_magicpoints;
                eater->stats->magicpoints += (50*stats->level);
                dead = 1;
                if (eater->user != -1)
                {
                  sprintf(message, "Potion of Mana(%d)!", stats->level);
                  screenp->do_notify(message, eater);
                }
                return 1;
         case FAMILY_INVULNERABLE_POTION:
                if (!eater->stats->query_bit_flags(BIT_INVINCIBLE) )
                {
                  eater->invulnerable_left += (150*stats->level);
                  dead = 1;
                  if (eater->user != -1)
                  {
                    sprintf(message, "Potion of Invulnerability(%d)!", stats->level);
                    screenp->do_notify(message, eater);
                  }
                }
                return 1;
         case FAMILY_INVIS_POTION:
           eater->invisibility_left += (150*stats->level);
           if (eater->user != -1)
           {
             sprintf(message, "Potion of Invisibility(%d)!", stats->level);
             screenp->do_notify(message, eater);
           }
           dead = 1;
           return 1;
         case FAMILY_SPEED_POTION:
           eater->speed_bonus_left += 50*stats->level;
           eater->speed_bonus = stats->level;
           if (eater->user != -1)
           {
             sprintf(message, "Potion of Speed(%d)!", stats->level);
             screenp->do_notify(message, eater);
           }
           dead = 1;
           return 1;
         case FAMILY_EXIT: // go to another level, possibly
                if (eater->query_act_type()!= ACT_CONTROL || (eater->skip_exit > 1))
                  return 1;
                eater->skip_exit = 10;
                // See if there are any enemies left ...
                if (screenp->level_done == 0)
                  guys_here = 1;
                else
                  guys_here = 0;
                // Get the name of our exit..
                sprintf(message, "scen%d", stats->level);
                strcpy(exitname, myscreen->get_scen_title(message, myscreen) );
                if (!stricmp(exitname, "none"))
                  sprintf(exitname, "Level %d", stats->level);
                leftside  = 160 - ( (strlen(exitname) + 18) * 3);
                rightside = 160 + ( (strlen(exitname) + 18) * 3);
                // First check to see if we're withdrawing into 
                //    somewhere we've been, in which case we abort
                //    this level, and set our current level to
                //    that pointed to by the exit ...
                if ( (screenp->levelstatus[stats->level] == 1)
                     && (screenp->levelstatus[screenp->scen_num] == 0)
                     && (guys_here != 0)
                   ) // okay to leave
                {
                  leftside -= 12;
                  rightside += 12;
                  screenp->draw_dialog(leftside, 20, rightside, 54, "Exit Field");
                  sprintf(message, "Withdraw to %s? (Y/N)", exitname);
                  eattext.write_y(42, message, DARK_BLUE, 1);
                  screenp->buffer_to_screen(0, 0, 320, 200);
                  eatkeys = query_keyboard();
                  clear_keyboard();
                  while (!eatkeys[SCAN_Y] && !eatkeys[SCAN_N])
                         dumbcount++;
                  // Redraw screen ..
                  screenp->redrawme = 1;

                  if (eatkeys[SCAN_Y]) // accepted level change
                  {
                    clear_keyboard();
                    screenp->levelstatus[screenp->scen_num] = 0;  // NOT done
                    // Delete all of our current information and abort ..
                    here = myscreen->oblist;
                    while (here)
                    {
                      if (here->ob && here->ob->query_order() == ORDER_LIVING)
                      {
                        //myscreen->remove_ob(here->ob);
                        here->ob->dead = 1;
                        myscreen->myobmap->remove(here->ob);
                        //myscreen->remove_obmap(here->ob);
                      }
                      here = here->next;
                    }
                    //here = myscreen->fxlist;
                    //while (here)
                    //{
                    //  if (here->ob)
                    //    myscreen->remove_fx_ob(here->ob);
                    //  here = here->next;
                    //}
                    // Now load the game as it was ...
                    load_saved_game("save0", myscreen);
                    myscreen->scen_num = (short) (stats->level-1);  
                    myscreen->end = 1;
                    save_game("save0", myscreen);

                    return screenp->endgame(1, stats->level); // retreat
                  }  // end of accepted withdraw to new level ..
                  clear_keyboard();
                } // end of checking for withdrawal to completed level  

                if (!guys_here) // nobody evil left, so okay to exit level ..
                {
                  screenp->draw_dialog(30, 20, 290, 54, "Exit Field");
                  sprintf(message, "Exit to %s? (Y/N)", exitname);
                  eattext.write_y(42, message, DARK_BLUE, 1);
                  screenp->buffer_to_screen(0, 0, 320, 200);
                  eatkeys = query_keyboard();
                  clear_keyboard();
                  while (!eatkeys[SCAN_Y] && !eatkeys[SCAN_N])
                         dumbcount++;
                  // Redraw screen ..
                  screenp->redrawme = 1;

                  if (eatkeys[SCAN_Y]) // accepted level change
                  {
                         clear_keyboard();
                         //screenp->levelstatus[screenp->scen_num] = 1;
                         return screenp->endgame(0, stats->level);
                  }
                  clear_keyboard();
                  return 1;
                }
         case FAMILY_TELEPORTER:
                if (eater->skip_exit > 1)
                  return 1;
                distance = distance_to_ob_center(eater); // how  away?
                if (distance > 21)
                  return 1;
                if (distance < 4 && eater->skip_exit)
                {
                  //eater->skip_exit++;
                  eater->skip_exit = 8;
                  return 1;
                }
                // If we're close enough, teleport ..
                eater->skip_exit += 20;
                if (!leader)
                  target = find_teleport_target();
                else
                  target = leader;
                if (!target)
                  return 1;
                leader = target;
                eater->center_on(target);
                if (!screenp->query_passable(eater->xpos, eater->ypos, eater))
                {
                  eater->center_on(this);
                  return 1;
                }
                // Now do special effects
                flash = screenp->add_ob(ORDER_FX, FAMILY_FLASH);
                flash->ani_type = ANI_EXPAND_8;
                flash->center_on(this);
                return 1;
         case FAMILY_LIFE_GEM: // get back some of lost man's xp ..
                if (eater->team_num != team_num) // only our team can get these
                  return 1;
                myscreen->m_score[eater->team_num] += stats->hitpoints;
                flash = screenp->add_ob(ORDER_FX, FAMILY_FLASH);
                flash->ani_type = ANI_EXPAND_8;
                flash->center_on(this);
                dead = 1;
                death();
                return 1;
         case FAMILY_KEY: // get the key to this door ..
           if (!(eater->keys & (long)(pow(2, stats->level)) )) // just got it?
           {
             eater->keys |= (long) (pow(2, stats->level)); // ie, 2, 4, 8, 16...
             if (eater->myguy)
               sprintf(message, "%s picks up key %d", eater->myguy->name,
                 stats->level);
             else
               sprintf(message, "%s picks up key %d", eater->stats->name, stats->level);
             if (eater->team_num == 0) // only show players picking up keys
             {
               myscreen->do_notify(message, eater);
               if (eater->on_screen())
                 myscreen->soundp->play_sound(SOUND_MONEY);
             }
           }
           return 1;
         default: return 1;
         } // end of treasure-check
}

void treasure::set_direct_frame(short whatframe)
{
  unsigned char  * data;
  frame = whatframe;

  data = screenp->myloader->graphics[PIX(order, family)];
  bmp = (unsigned char  *) (data+3 + frame*size);

}

walker  * treasure::find_teleport_target()
{
  walker  *target = NULL;
  oblink  *here = screenp->fxlist;
  short keep_looking = 1;
  short number = 0;

  // First find where we are in the list ...
  while (here && keep_looking)
  {
         if (here->ob && here->ob == this) // found us
                keep_looking = 0;
         else
         {
                here = here->next;
                number++;
         }
  }

  if (keep_looking) // didn't find us?
         return NULL;

  //printf("Teleporting from #%d ..", number);

  // Now search the rest of the loop ..
  keep_looking = 1;
  here = here->next;
  number++;
  while (here && keep_looking)
  {
         if (here->ob && !here->ob->dead)
         {
                if (here->ob->query_order() == ORDER_TREASURE &&
                         here->ob->query_family() == FAMILY_TELEPORTER &&
                         here->ob->stats->level == stats->level)
                {
                  target = here->ob;
                  keep_looking = 0;
                  //printf(" to target %d\n", number);
                  return target;
                }
         }
         here = here->next;
         number++;
  }

  // Hit the end of the list, look from top down now ..
  here = screenp->fxlist;
  number = 0;
  while (here)
  {
         if (here->ob && !here->ob->dead)
         {
                if (here->ob->query_order() == ORDER_TREASURE &&
                         here->ob->query_family() == FAMILY_TELEPORTER &&
                         here->ob->stats->level == stats->level)
                {
                  target = here->ob;
                  //printf(" to looped target %d\n", number);
                  return target;
                }
         }
         here = here->next;
         number++;
  }

  return target;
}
